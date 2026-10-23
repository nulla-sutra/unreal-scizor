// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Scizor : ModuleRules
{
    public Scizor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.Default;

        // Resolve module-qualified headers through Unreal's parent include paths.
        bLegacyParentIncludePaths = true;
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
