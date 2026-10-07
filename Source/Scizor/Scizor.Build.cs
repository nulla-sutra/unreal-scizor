// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Scizor : ModuleRules
{
    public Scizor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "AIModule",
                "AnimGraphRuntime",
                "EnhancedInput",
                "GameplayTags",
                "StateTreeModule",
                "GameplayStateTreeModule"
            }
        );
        PrivateDependencyModuleNames.AddRange(
            new[]
            {
                "GameplayAbilities"
            }
        );
    }
}
