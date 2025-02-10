// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SuraS : ModuleRules
{
	public SuraS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"AnimGraphRuntime",
			"UMG",
            "EngineCameras"
        });
	}
}
