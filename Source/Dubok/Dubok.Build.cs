using UnrealBuildTool;

public class Dubok : ModuleRules
{
    public Dubok(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "UMG",
            "AIModule",
            "NavigationSystem",
            "PhysicsCore",
            "GeometryCollectionEngine"
        });
    }
}
