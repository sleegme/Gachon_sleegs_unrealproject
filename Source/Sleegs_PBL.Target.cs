// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Sleegs_PBLTarget : TargetRules
{
	public Sleegs_PBLTarget(TargetInfo Target) : base(Target)
	{
        AdditionalCompilerArguments = "/wd4668 /wd4067";
        Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Sleegs_PBL" } );
	}
}
