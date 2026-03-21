// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Proyecto404TNF : ModuleRules
{
	public Proyecto404TNF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Proyecto404TNF",
			"Proyecto404TNF/Variant_Platforming",
			"Proyecto404TNF/Variant_Platforming/Animation",
			"Proyecto404TNF/Variant_Combat",
			"Proyecto404TNF/Variant_Combat/AI",
			"Proyecto404TNF/Variant_Combat/Animation",
			"Proyecto404TNF/Variant_Combat/Gameplay",
			"Proyecto404TNF/Variant_Combat/Interfaces",
			"Proyecto404TNF/Variant_Combat/UI",
			"Proyecto404TNF/Variant_SideScrolling",
			"Proyecto404TNF/Variant_SideScrolling/AI",
			"Proyecto404TNF/Variant_SideScrolling/Gameplay",
			"Proyecto404TNF/Variant_SideScrolling/Interfaces",
			"Proyecto404TNF/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
