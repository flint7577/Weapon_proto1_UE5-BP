using UnrealBuildTool;
using System.Collections.Generic;

public class DubokEditorTarget : TargetRules
{
    public DubokEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
        ExtraModuleNames.Add("Dubok");
    }
}
