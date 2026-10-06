// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "ScizorStateTreeEvaluator_ComboComponent.generated.h"

class UScizorComboComponent;
class AActor;

USTRUCT()
struct SCIZOR_API FScizorStateTreeEvaluator_ComboComponent_InstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<AActor> Actor;

    UPROPERTY(VisibleAnywhere, Category = Output)
    TObjectPtr<UScizorComboComponent> ComboComponent;
};

USTRUCT(DisplayName = "Combo Component (Scizor)")
struct SCIZOR_API FScizorStateTreeEvaluator_ComboComponent : public FStateTreeEvaluatorCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FScizorStateTreeEvaluator_ComboComponent_InstanceData;
    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
    virtual void TreeStart(FStateTreeExecutionContext& Context) const override;
    virtual void Tick(FStateTreeExecutionContext& Context, float DeltaTime) const override;
    virtual void TreeStop(FStateTreeExecutionContext& Context) const override;

private:
    static void Refresh(FInstanceDataType& Data);
};
