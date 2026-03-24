// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Proyecto404TNF : ModuleRules
{
	public Proyecto404TNF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
