// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeExecutionContext.h"
#include "StateTreePropertyFunctionBase.h"
#include "Scizor/Combo/ComboTypes.h"
#include "Scizor/Combo/Components/ScizorComboComponent.h"
#include "ScizorStateTreePropertyFunction_GetComboInfoSummary.generated.h"

class UScizorComboComponent;

USTRUCT()
struct SCIZOR_API FScizorStateTreePropertyFunction_GetComboInfoSummary_InstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UScizorComboComponent> ComboComponent;

    UPROPERTY(EditAnywhere, Category = Output)
    FScizorComboInfoSummary Summary;
};

/**
 * 
 */
USTRUCT(DisplayName = "Combo Info Summary (Scizor)")
struct SCIZOR_API FScizorPropertyFunction_GetComboInfoSummary : public FStateTreePropertyFunctionCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FScizorStateTreePropertyFunction_GetComboInfoSummary_InstanceData;

    virtual const UStruct* GetInstanceDataType() const override
    {
        return FInstanceDataType::StaticStruct();
    };

    virtual void Execute(FStateTreeExecutionContext& Context) const override
    {
        auto& [ComboComponent, Summary] = Context.GetInstanceData(*this);
        if (!ComboComponent)
        {
            return;
        }
        Summary = ComboComponent->GetComboInfoSummary();
    };
};
