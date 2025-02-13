// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SuraS : ModuleRules
{
	public SuraS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AnimGraphRuntime",
			"AIModule",			// Added by Yoony for AI
			"NavigationSystem",	// Added by Yoony for AI Navigation
            "UMG",			 //Added by Boranaga for the use of UI
            "EngineCameras", //Added by Boranaga for the use of CameraShake
			"Niagara"        //Added by Boranaga for the use of ParticleSystem
        });
	}
}
