// Copyright 2019-Present tarnishablec. All Rights Reserved.

#include "ScizorComboComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreGlobals.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "Scizor/Combo/Animation/AnimNotifyState_ScizorComboWindow.h"

namespace
{
    UAbilitySystemComponent* ResolveAbilitySystemComponent(AActor* Actor)
    {
        if (!IsValid(Actor))
        {
            return nullptr;
        }

        if (auto* Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
        {
            return Asc;
        }

        // The ASC may live on PlayerState while its avatar owns this component.
        if (const auto* Pawn = Cast<APawn>(Actor))
        {
            return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn->GetPlayerState());
        }

        if (const auto* Controller = Cast<AController>(Actor))
        {
            if (auto* Asc = ResolveAbilitySystemComponent(Controller->GetPawn()))
            {
                return Asc;
            }
            return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Controller->PlayerState);
        }

        return nullptr;
    }
}

UE_DEFINE_GAMEPLAY_TAG(Tag_StateTreeEvent_BachComboInput, SCIZOR_INPUT_TAG_LITERAL);
namespace Scizor
{
    const FGameplayTag DefaultComboEventTag = Tag_StateTreeEvent_BachComboInput;
}

UScizorComboComponent::UScizorComboComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ComboWindowClass = UAnimNotifyState_ScizorComboWindow::StaticClass();
}

void UScizorComboComponent::BeginPlay()
{
    // Bind montage notifications before the inherited component can start its tree.
    RefreshAnimationBindings();
    Super::BeginPlay();
}

void UScizorComboComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    UnbindAnimation();
    if (auto* Mesh = BoundMesh.Get())
    {
        Mesh->OnAnimInitialized.RemoveDynamic(this, &ThisClass::HandleMeshAnimInitialized);
    }
    BoundMesh.Reset();
}

void UScizorComboComponent::RefreshAnimationBindings()
{
    const auto* Asc = ResolveAbilitySystemComponent(GetOwner());
    auto* Mesh = Asc && Asc->AbilityActorInfo.IsValid()
        ? Asc->AbilityActorInfo->SkeletalMeshComponent.Get() : nullptr;
    if (Mesh != BoundMesh.Get())
    {
        UnbindAnimation();
        if (auto* PreviousMesh = BoundMesh.Get())
        {
            PreviousMesh->OnAnimInitialized.RemoveDynamic(this, &ThisClass::HandleMeshAnimInitialized);
        }
        BoundMesh = Mesh;
        if (Mesh)
        {
            Mesh->OnAnimInitialized.AddUniqueDynamic(this, &ThisClass::HandleMeshAnimInitialized);
        }
        HandleMeshAnimInitialized();
    }
    else if (Mesh && Mesh->GetAnimInstance() != BoundAnimInstance.Get())
    {
        HandleMeshAnimInitialized();
    }
}

void UScizorComboComponent::UnbindAnimation()
{
    if (auto* Anim = BoundAnimInstance.Get())
    {
        Anim->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ThisClass::HandleComboWindowBegin);
        Anim->OnPlayMontageNotifyEnd.RemoveDynamic(this, &ThisClass::HandleComboWindowEnd);
    }
    BoundAnimInstance.Reset();
    SummaryCache = {};
    LastSummaryFrameCount = MAX_uint64;
}

void UScizorComboComponent::HandleMeshAnimInitialized()
{
    UnbindAnimation();
    if (auto* Mesh = BoundMesh.Get())
    {
        BoundAnimInstance = Mesh->GetAnimInstance();
    }
    if (auto* Anim = BoundAnimInstance.Get())
    {
        Anim->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ThisClass::HandleComboWindowBegin);
        Anim->OnPlayMontageNotifyEnd.AddUniqueDynamic(this, &ThisClass::HandleComboWindowEnd);
    }
}

FScizorComboInfoSummary UScizorComboComponent::GetComboInfoSummary()
{
    RefreshAnimationBindings();
    if (LastSummaryFrameCount == GFrameCounter)
    {
        return SummaryCache;
    }
    LastSummaryFrameCount = GFrameCounter;
    SummaryCache = {};

    auto* Anim = BoundAnimInstance.Get();
    const auto* MontageInstance = Anim ? Anim->GetActiveMontageInstance() : nullptr;
    if (!MontageInstance || !MontageInstance->Montage)
    {
        return SummaryCache;
    }

    auto* Montage = MontageInstance->Montage.Get();
    const auto Section = MontageInstance->GetCurrentSection();
    const auto SectionIndex = Montage->GetSectionIndex(Section);
    if (SectionIndex == INDEX_NONE)
    {
        return SummaryCache;
    }

    float SectionStart = 0.f;
    float SectionEnd = 0.f;
    Montage->GetSectionStartAndEndTime(SectionIndex, SectionStart, SectionEnd);
    FAnimNotifyContext NotifyContext;
    Montage->GetAnimNotifiesFromDeltaPositions(SectionStart + 0.001f, SectionEnd, NotifyContext);
    const auto* Window = NotifyContext.ActiveNotifies.FindByPredicate(
        [this](const FAnimNotifyEventReference& Event)
        {
            const auto* Notify = Event.GetNotify();
            return Notify && Notify->NotifyStateClass && Notify->NotifyStateClass.IsA(ComboWindowClass);
        });
    if (!Window)
    {
        return SummaryCache;
    }

    const auto* Notify = Window->GetNotify();
    const auto Position = MontageInstance->GetPosition();
    const auto Start = Notify->GetTriggerTime();
    const auto End = Notify->GetEndTriggerTime();

    // Query montage time directly; private ActiveStateBranchingPoints is not an API.
    SummaryCache.ComboWindowState = Position < Start ? EScizorComboWindowState::BeforeComboWindow
        : Position > End ? EScizorComboWindowState::AfterComboWindow
        : EScizorComboWindowState::InsideComboWindow;
    SummaryCache.MontageAsset = Montage;
    SummaryCache.CurrentSection = Section;
    SummaryCache.CurrentPosition = Position;
    SummaryCache.CurrentComboWindowRemainTime = End - Position;
    SummaryCache.CurrentComboWindowDuration = Notify->Duration;
    return SummaryCache;
}

void UScizorComboComponent::SendComboInputEvent(const FGameplayTag Tag,
    const TInstancedStruct<FScizorComboInputEventPayload>& Payload)
{
    RefreshAnimationBindings();
    if (Payload.IsValid())
    {
        SendStateTreeEvent(Tag, FConstStructView::Make(Payload.Get()), GetFName());
    }
    else
    {
        SendStateTreeEvent(Tag, FConstStructView(), GetFName());
    }
}

bool UScizorComboComponent::IsComboWindow(const FBranchingPointNotifyPayload& Payload) const
{
    return Payload.NotifyEvent && Payload.NotifyEvent->NotifyStateClass
        && Payload.NotifyEvent->NotifyStateClass.IsA(ComboWindowClass);
}

void UScizorComboComponent::HandleComboWindowBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
    if (IsComboWindow(Payload))
    {
        LastSummaryFrameCount = MAX_uint64;
        OnCrossComboWindow.Broadcast(true);
    }
}

void UScizorComboComponent::HandleComboWindowEnd(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
    if (IsComboWindow(Payload))
    {
        LastSummaryFrameCount = MAX_uint64;
        OnCrossComboWindow.Broadcast(false);
    }
}
