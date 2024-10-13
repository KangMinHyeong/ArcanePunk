#include "FArcanePunkEditorTools.h"

#include "LevelEditor.h"
#include "SkillTest/FSkillEditorTool.h"

IMPLEMENT_MODULE(FArcanePunkEditorTools, ArcanePunkEditorTools)

void FArcanePunkEditorTools::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("ArcanePunkEditorTools module has started!"));
	
	RegisterCustomizations();
}

void FArcanePunkEditorTools::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("ArcanePunkEditorTools module is shutting down."));
	
	UnregisterCustomizations();
}

void FArcanePunkEditorTools::RegisterCustomizations()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	MenuExtender = CreateMenuExtender();
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	RegisterSkillEditorTool();
}

void FArcanePunkEditorTools::UnregisterCustomizations()
{
	if (FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		UnregisterSkillEditorTool();
		FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");

		LevelEditorModule.GetMenuExtensibilityManager()->RemoveExtender(MenuExtender);
	}
}

TSharedPtr<FExtender> FArcanePunkEditorTools::CreateMenuExtender()
{
	TSharedPtr<FExtender> Extender = MakeShareable(new FExtender);
	
	Extender->AddMenuBarExtension(
		"Help",
		EExtensionHook::Before,
		nullptr,
		FMenuBarExtensionDelegate::CreateRaw(this, &FArcanePunkEditorTools::AddMenuBarEntry)
	);

	return Extender;
}

void FArcanePunkEditorTools::AddMenuBarEntry(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(
		FText::FromString("Arcane Punk Tool"),
		FText::FromString("Arcane Punk Editor Tools"),
		FNewMenuDelegate::CreateRaw(this, &FArcanePunkEditorTools::FillSkillEditorMenu),
		"Arcane Punk Tool",
		FName(TEXT("Arcane Punk Tool"))
	);
}

void FArcanePunkEditorTools::FillSkillEditorMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString("Open Skill Editor"),
		FText::FromString("Opens the Skill Editor tool."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FArcanePunkEditorTools::OpenSkillEditorTool))
	);
}

void FArcanePunkEditorTools::OpenSkillEditorTool()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FName("SkillEditorTool"));
}

void FArcanePunkEditorTools::RegisterSkillEditorTool()
{
	if (!FGlobalTabmanager::Get()->HasTabSpawner(FName("SkillEditorTool")))
	{
		FGlobalTabmanager::Get()->RegisterTabSpawner(FName("SkillEditorTool"), FOnSpawnTab::CreateRaw(this, &FArcanePunkEditorTools::SpawnSkillEditorTab));
	}
}

void FArcanePunkEditorTools::UnregisterSkillEditorTool()
{
	if (FGlobalTabmanager::Get()->HasTabSpawner(FName("SkillEditorTool")))
	{
		FGlobalTabmanager::Get()->UnregisterTabSpawner(FName("SkillEditorTool"));
	}
}

TSharedRef<SDockTab> FArcanePunkEditorTools::SpawnSkillEditorTab(const FSpawnTabArgs& TabArgs)
{
	return SNew(SDockTab)
		.Label(FText::FromString("Skill Editor"))
		[
			SNew(FSkillEditorTool)
		];
}