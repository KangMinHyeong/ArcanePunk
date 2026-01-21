using UnrealBuildTool;

public class ArcanePunk : ModuleRules
{
	public ArcanePunk(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"SlateCore", 
			"Slate", 
			"UMG", 
			"GameplayTasks", 
			"Niagara", 
			"NavigationSystem", 
			"GeometryCollectionEngine", 
			"FieldSystemEngine", 
			"MotionWarping",
			"GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEd",           // FAssetRegistryModule, PIE 관련 인터페이스
				"EditorSubsystem",    // (선택) 에디터 내 동작 처리시
				"AssetRegistry"       // FAssetRegistryModule
			});
		}
	}
}