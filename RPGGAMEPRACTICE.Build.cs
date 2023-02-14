// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPGGAMEPRACTICE : ModuleRules
{
	public RPGGAMEPRACTICE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
