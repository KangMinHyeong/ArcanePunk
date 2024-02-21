// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/APHUD.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"
#include "UserInterface/APTuTorialUserWidget.h"
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"

AAPHUD::AAPHUD()
{

	// Minhyeong
	EnhanceChoiceClasses.SetNum(3);
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

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (TutorialWidgetClass)
	{
		TutorialWidget = CreateWidget<UAPTuTorialUserWidget>(GetWorld(), TutorialWidgetClass);
		TutorialWidget->AddToViewport(-2);
		TutorialWidget->SetVisibility(ESlateVisibility::Visible);

		TutorialDone = true;
		//TutorialDone = false;
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

	//Minhyeong
	if(!StatusBarUIClass) return;
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
		// Character->MouseArr.Pop();
		// if(!Character->MouseArr.IsEmpty()) return;
		// const FInputModeGameOnly InputMode;
		// GetOwningPlayerController()->SetInputMode(InputMode);
		// GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		// Character->MouseArr.Add(0);
		// const FInputModeGameAndUI InputMode;
		// GetOwningPlayerController()->SetInputMode(InputMode);
		// GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AAPHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAPHUD::HideInteractionWidget() const
{

	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AAPHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void AAPHUD::UpdateTutorialWidget(const FString PressedKey)
{
	if (TutorialWidget)
	{
		if (TutorialWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			TutorialWidget->SetVisibility(ESlateVisibility::Visible);
		}

		TutorialDone = TutorialWidget->UpdateWidget(PressedKey);

		if (TutorialDone) HideTutorialWidget();
	}
}

void AAPHUD::HideTutorialWidget() const
{
	if (TutorialWidget)
	{
		TutorialWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// Minhyeong
void AAPHUD::SetBossHPUI()
{
	if(!BossHPUIClass) return;
	BossHPWidget = CreateWidget<UUserWidget>(GetWorld(), BossHPUIClass);
	BossHPWidget->AddToViewport(-1);
}

void AAPHUD::DisplayEnhanceChoice(EEnhanceCategory EnhanceCategory, EEnHanceType UpdateEnHanceType)
{
	if(!EnhanceChoiceClasses[(uint8)UpdateEnHanceType]) return;

	auto EnhanceUI = CreateWidget<UAPEnhanceChoice>(GetWorld(), EnhanceChoiceClasses[(uint8)UpdateEnHanceType]);
	EnhanceUI->InitType(EnhanceCategory, UpdateEnHanceType);
	// ESkillTypeState, EnHanceType 의 정보를 EnhanceUI에게 넘겨주기
	EnhanceUI->AddToViewport(-1);
}
