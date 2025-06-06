// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArcanePunk : ModuleRules
{
	public ArcanePunk(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"SlateCore", 
			"Slate", 
<<<<<<< HEAD
			"UMG" , "GameplayTasks", "Niagara", "NavigationSystem", "GeometryCollectionEngine", "FieldSystemEngine"});
=======
			"UMG" , 
			"GameplayTasks", 
			"Niagara", 
			"NavigationSystem", 
			"GeometryCollectionEngine", 
			"FieldSystemEngine", 
			"MotionWarping",
			"GameplayTags",});
>>>>>>> origin

		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
