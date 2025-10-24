// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ST_EditorCrashHighscore : ModuleRules
{
	public ST_EditorCrashHighscore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
	}
}
