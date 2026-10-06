// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeConditionBase.h"
#include "StateTreeExecutionContext.h"
#include "Scizor/Combo/ComboTypes.h"
#include "Scizor/Combo/Components/ScizorComboComponent.h"
#include "ScizorStateTreeCondition_ComboWindowCheck.generated.h"

USTRUCT()
struct FScizorStateTreeCondition_ComboWindowCheck_InstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category="Input")
    TObjectPtr<UScizorComboComponent> StateTreeComponent;

    UPROPERTY(EditAnywhere, meta=(Bitmask, BitmaskEnum="/Script/Scizor.EScizorComboWindowState"))
    int32 MatchState = 0;
};


/**
 * 
 */
USTRUCT(DisplayName = "Combo Window Check (Scizor)")
struct SCIZOR_API FScizorStateTreeCondition_ComboWindowCheck : public FStateTreeConditionCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FScizorStateTreeCondition_ComboWindowCheck_InstanceData;
    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); };

protected:
    virtual bool TestCondition(FStateTreeExecutionContext& Context) const override
    {
        const auto& [Component,MatchState] = Context.GetInstanceData(*this);

        if (!Component)
        {
            return false;
        }

        const auto CurrentState = Component->GetComboInfoSummary().ComboWindowState;
        return (static_cast<int32>(CurrentState) & MatchState) == static_cast<int>(CurrentState);
    };
};
