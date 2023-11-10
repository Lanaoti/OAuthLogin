// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
using Tools.DotNETCommon;

public class TapDB : ModuleRules
{
    public TapDB(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
        FileReference fileRef = new FileReference(Path.Combine(PluginDirectory, Name + ".uplugin"));
        PluginInfo plugin = new PluginInfo(fileRef, PluginType.Project);	
        PublicDefinitions.Add(Name + "_UE_VERSION_NUMBER=TEXT(\"" + plugin.Descriptor.Version + "\")");
        PublicDefinitions.Add(Name + "_UE_VERSION=TEXT(\"" + plugin.Descriptor.VersionName + "\")");


        PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "TapCommon",
                "Json",
                "ApplicationCore",
                "JsonUtilities",
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicFrameworks.AddRange(
                new string[]{
                    "AdSupport",
                    "CoreMotion",
                    "Security",
                    "SystemConfiguration",
                    "CoreTelephony"
                });

            PublicWeakFrameworks.AddRange(
                new string[]{
                    "AppTrackingTransparency",
                    "iAd",
                    "AdServices"
                }
            );

            PublicAdditionalFrameworks.Add(
                new Framework(
                    "TapDB",
                    "../ThirdParty/iOS/Frameworks/TapDB.zip"
                )
            );
        }

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            AdditionalPropertiesForReceipt.Add(
                "AndroidPlugin",
                Path.Combine(ModuleDirectory, "TapDB_Android_UPL.xml")
            );
        }
    }
}
