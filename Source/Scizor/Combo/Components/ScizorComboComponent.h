// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "NativeGameplayTags.h"
#include "Scizor/Combo/ComboTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "ScizorComboComponent.generated.h"

#define SCIZOR_INPUT_TAG_LITERAL "Scizor.Combo.ComboInput"
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_StateTreeEvent_BachComboInput);

class UAnimInstance;
class UAnimNotify_PlayMontageNotifyWindow;
class USkeletalMeshComponent;
struct FBranchingPointNotifyPayload;

namespace Scizor
{
    extern const FGameplayTag DefaultComboEventTag;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScizorCrossComboWindowDelegate, bool, bWindowOpen);

/** The official StateTree component with montage combo-window queries and typed input events. */
UCLASS(ClassGroup=(Scizor), meta=(BlueprintSpawnableComponent))
class SCIZOR_API UScizorComboComponent : public UStateTreeComponent
{
    GENERATED_BODY()

public:
    UScizorComboComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combo")
    TSubclassOf<UAnimNotify_PlayMontageNotifyWindow> ComboWindowClass;

    UPROPERTY(BlueprintAssignable, Category="Combo")
    FScizorCrossComboWindowDelegate OnCrossComboWindow;

    UFUNCTION(BlueprintPure, Category="Combo")
    FScizorComboInfoSummary GetComboInfoSummary();

    UFUNCTION(BlueprintCallable, Category="Combo")
    void RefreshAnimationBindings();

    UFUNCTION(BlueprintCallable, Category="Combo",
        meta=(CPP_Default_Tag="Scizor.Combo.ComboInput", CPP_Default_Payload, AutoCreateRefTerm="Payload"))
    void SendComboInputEvent(const FGameplayTag Tag = Scizor::DefaultComboEventTag,
        const TInstancedStruct<FScizorComboInputEventPayload>& Payload = {});

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UFUNCTION()
    void HandleComboWindowBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
    UFUNCTION()
    void HandleComboWindowEnd(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
    UFUNCTION()
    void HandleMeshAnimInitialized();

    void UnbindAnimation();
    bool IsComboWindow(const FBranchingPointNotifyPayload& Payload) const;

    TWeakObjectPtr<USkeletalMeshComponent> BoundMesh;
    TWeakObjectPtr<UAnimInstance> BoundAnimInstance;
    uint64 LastSummaryFrameCount = MAX_uint64;
    FScizorComboInfoSummary SummaryCache;
};
