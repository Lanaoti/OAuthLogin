// Some copyright should be here...

using System.IO;
using UnrealBuildTool;
#if UE_5_0_OR_LATER
using EpicGames.Core;
#else
using Tools.DotNETCommon;
#endif

public class TapCommon : ModuleRules
{
	public TapCommon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		FileReference fileRef = new FileReference(Path.Combine(PluginDirectory, Name + ".uplugin"));
		PluginInfo plugin = new PluginInfo(fileRef, PluginType.Project);
		PublicDefinitions.Add(Name + "_UE_VERSION_NUMBER=TEXT(\"" + plugin.Descriptor.Version + "\")");
		PublicDefinitions.Add(Name + "_UE_VERSION=TEXT(\"" + plugin.Descriptor.VersionName + "\")");


		PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));
		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));
		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/Android")));
		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/iOS")));
		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Tools")));
		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "TDSNet")));

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"HTTPServer",
				"UMG",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"ApplicationCore",

				"Slate",
				"SlateCore",

				"Json",
				"JsonUtilities",

				"HTTP",
				"Sockets",
				"WebBrowser", 
				
				"ImageWrapper",
			}
		);

		AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");

		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicFrameworks.AddRange(
				new string[]
				{
					"SystemConfiguration",
					"WebKit",
					"SystemConfiguration",
					"CoreTelephony",
					"MobileCoreServices",
					"Security"
				});

			PublicSystemLibraries.Add("sqlite3");

			PublicAdditionalFrameworks.Add(
				new Framework(
					"TapCommonSDK",
					"../ThirdParty/iOS/Frameworks/TapCommonSDK.zip",
					"TapCommonResource.bundle"
				)
			);
		}

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Launch"
				}
			);
			AdditionalPropertiesForReceipt.Add(
				"AndroidPlugin",
				Path.Combine(ModuleDirectory, "TapCommon_Android_UPL.xml")
			);
		}
	}
}