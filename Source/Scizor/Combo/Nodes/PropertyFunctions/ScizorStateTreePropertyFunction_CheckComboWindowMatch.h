// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeExecutionContext.h"
#include "StateTreePropertyFunctionBase.h"
#include "Scizor/Combo/Components/ScizorComboComponent.h"
#include "ScizorStateTreePropertyFunction_CheckComboWindowMatch.generated.h"

USTRUCT()
struct SCIZOR_API FScizorStateTreePropertyFunction_CheckComboWindowMatch_InstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UScizorComboComponent> StateTreeComponent;

    UPROPERTY(EditAnywhere, Category = Parameter,
        meta=(Bitmask, BitmaskEnum="/Script/Scizor.EScizorComboWindowState"))

    int32 MatchState = 0;

    UPROPERTY(EditAnywhere, Category = Output)
    bool bMatched = false;
};


/**
 * 
 */
USTRUCT(DisplayName = "Match Combo Window (Scizor)")
struct SCIZOR_API FScizorStateTreePropertyFunction_CheckComboWindowMatch : public FStateTreePropertyFunctionCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FScizorStateTreePropertyFunction_CheckComboWindowMatch_InstanceData;

    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

    virtual void Execute(FStateTreeExecutionContext& Context) const override
    {
        auto& [ComboComponent,MatchState,bMatched] = Context.GetInstanceData(*this);
        if (!ComboComponent)
        {
            return;
        }
        const auto CurrentState = ComboComponent->GetComboInfoSummary().ComboWindowState;
        bMatched = (static_cast<int32>(CurrentState) & MatchState) == static_cast<int>(CurrentState);
    };
};
