// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/HUD/APHUD.h"
#include "UserInterface/Inventory/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"
#include "UserInterface/Tutorial/APTuTorialUserWidget.h"
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"
#include "UserInterface/Inform/Stage/APStageInformationUI.h"
#include "UserInterface/EnhanceUI/EnhanceGauge.h"
#include "UserInterface/HUD/Status/Bar/ImitatorSkillSlot.h"
#include "APHUD.h"

AAPHUD::AAPHUD()
{
}

void AAPHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(-4);
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}

	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController)
	{
		int32 ViewportSizeX;
		int32 ViewportSizeY;
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		ViewportSize.X = static_cast<double>(ViewportSizeX);
		ViewportSize.Y = static_cast<double>(ViewportSizeY);
	}
}

void AAPHUD::Init()
{
	//Minhyeong
	
	auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if(!StatusBarUIClass || !GM) return;
	for()
	StatusBarWidget = CreateWidget<UUserWidget>(GetWorld(), StatusBarUIClass);
	StatusBarWidget->AddToViewport(-1);
}

void AAPHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAPHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	}
}

void AAPHUD::ToggleMenu()
{
	auto Character = Cast<AArcanePunkCharacter>(GetOwningPawn());
	if(!Character) return;

	if (bIsMenuVisible)
	{
		HideMenu();
	}
	else
	{
		DisplayMenu();
	}
}


// Minhyeong
void AAPHUD::SetBossHPUI()
{
	if(!BossHPUIClass) return;
	BossHPWidget = CreateWidget<UUserWidget>(GetWorld(), BossHPUIClass);
	BossHPWidget->AddToViewport(-1);
}

void AAPHUD::DisplayEnhanceChoice(EEnhanceCategory EnhanceCategory, EEnHanceType UpdateEnHanceType, bool bOnlyEnhance, uint8 EnhanceSkillNum)
{
	if(!EnhanceChoiceClass) return;

	auto EnhanceUI = CreateWidget<UAPEnhanceChoice>(GetWorld(), EnhanceChoiceClass); if(!EnhanceUI) return;
	if(bOnlyEnhance) EnhanceUI->SetSkillAppearPercent(0.0f);
	EnhanceUI->InitType(EnhanceCategory, UpdateEnHanceType, EnhanceSkillNum);

	// ESkillTypeState, EnHanceType 의 정보를 EnhanceUI에게 넘겨주기
	EnhanceUI->AddToViewport(20);
}

void AAPHUD::DisplayEnhanceGauge(int32 TargetNum, int32 MaxNum)
{
	auto EnhanceGaugeUI = CreateWidget<UEnhanceGauge>(GetWorld(), EnhanceGaugeUIClass); if(!EnhanceGaugeUI) return;
	EnhanceGaugeUI->DisplayEnhanceGauge(TargetNum, MaxNum);
	EnhanceGaugeUI->AddToViewport(20);
}

void AAPHUD::OpenWorldMap()
{
	if(StageInformationUI.IsValid())
	{
		StageInformationUI->RemoveFromParent();
		StageInformationUI = nullptr;
	}
	else
	{
		StageInformationUI = CreateWidget<UAPStageInformationUI>(GetWorld(), StageInformationUIClass);
		StageInformationUI->AddToViewport();
	}
}

// void AAPHUD::StartImitatorSlot()
// {
// 	ImitatorSlotUI = CreateWidget<UImitatorSkillSlot>(GetWorld(), ImitatorSlotUIClass);
// 	if(ImitatorSlotUI.IsValid()) ImitatorSlotUI->AddToViewport();
// }

// void AAPHUD::EndImitatorSlot()
// {
// 	if(ImitatorSlotUI.IsValid()) ImitatorSlotUI->RemoveFromParent();
// }

// void AAPHUD::UpdateImitatorSlot(uint8 SkillNumber)
// {
// 	if(!ImitatorSlotUI.IsValid()) return;
// 	ImitatorSlotUI->UpdateSkillSlot(SkillNumber);
// }

void AAPHUD::SetStatusVisibility(bool bHide)
{
	if(!StatusBarWidget.IsValid()) return;

	if(bHide)
	{
		StatusBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		StatusBarWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
