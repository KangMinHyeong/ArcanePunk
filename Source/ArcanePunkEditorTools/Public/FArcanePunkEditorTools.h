#pragma once

#include "Modules/ModuleManager.h"

class FArcanePunkEditorTools : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterCustomizations();
	void UnregisterCustomizations();
	
	TSharedPtr<FExtender> CreateMenuExtender();
	void AddMenuBarEntry(FMenuBarBuilder& MenuBuilder);
	void FillSkillEditorMenu(FMenuBuilder& MenuBuilder);
	void OpenSkillEditorTool();
	void UnregisterSkillEditorTool();
	void RegisterSkillEditorTool();
	TSharedRef<SDockTab> SpawnSkillEditorTab(const FSpawnTabArgs& TabArgs);

private:
	TSharedPtr<FExtender> MenuExtender;
};
