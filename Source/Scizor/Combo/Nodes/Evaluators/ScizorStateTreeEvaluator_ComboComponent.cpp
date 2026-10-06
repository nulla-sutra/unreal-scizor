// Copyright 2019-Present tarnishablec. All Rights Reserved.

#include "ScizorStateTreeEvaluator_ComboComponent.h"

#include "GameFramework/Actor.h"
#include "Scizor/Combo/Components/ScizorComboComponent.h"
#include "StateTreeExecutionContext.h"

void FScizorStateTreeEvaluator_ComboComponent::Refresh(FInstanceDataType& Data)
{
    Data.ComboComponent = IsValid(Data.Actor) ? Data.Actor->FindComponentByClass<UScizorComboComponent>() : nullptr;
    if (Data.ComboComponent)
    {
        Data.ComboComponent->RefreshAnimationBindings();
    }
}

void FScizorStateTreeEvaluator_ComboComponent::TreeStart(FStateTreeExecutionContext& Context) const
{
    Refresh(Context.GetInstanceData(*this));
}

void FScizorStateTreeEvaluator_ComboComponent::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    Refresh(Context.GetInstanceData(*this));
}

void FScizorStateTreeEvaluator_ComboComponent::TreeStop(FStateTreeExecutionContext& Context) const
{
    Context.GetInstanceData(*this).ComboComponent = nullptr;
}
