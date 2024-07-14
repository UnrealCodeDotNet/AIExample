// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AIExample : ModuleRules
{
	public AIExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {  "AIModule", "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
