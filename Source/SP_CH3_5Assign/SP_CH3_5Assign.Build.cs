// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SP_CH3_5Assign : ModuleRules
{
	public SP_CH3_5Assign(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","UMG" });
	}
}
