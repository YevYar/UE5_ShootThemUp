// "Shoot Them Up" Game by YevYar, All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpEditorTarget : TargetRules
{
	public ShootThemUpEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );

        switch (Target.Configuration)
        {
            case UnrealTargetConfiguration.Debug:
            case UnrealTargetConfiguration.DebugGame:
            case UnrealTargetConfiguration.Development:
            case UnrealTargetConfiguration.Test:
                ProjectDefinitions.Add("STU_DEBUG=1");
                break;
            default:
                ProjectDefinitions.Add("STU_DEBUG=0");
                break;
        }
    }
}
