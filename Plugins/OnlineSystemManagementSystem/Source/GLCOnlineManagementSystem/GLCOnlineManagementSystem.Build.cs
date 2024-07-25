
using UnrealBuildTool;

public class GLCOnlineManagementSystem : ModuleRules
{
	public GLCOnlineManagementSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
			}
			);

        PrivateIncludePaths.AddRange(
			new string[] {
			}
			);

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"OnlineSubsystem",
				"UMG",
				"Slate",
				"SlateCore",
                "GLCRuntimeGamePrinting",
                // ... add other public dependencies that you statically link with here ...
			}
			);

        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine", 
				"EOSVoiceChat",
				"OnlineSubsystemEOS",
			}
			);

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
