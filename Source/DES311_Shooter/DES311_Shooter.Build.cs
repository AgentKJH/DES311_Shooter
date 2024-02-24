// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DES311_Shooter : ModuleRules
{
	public DES311_Shooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
