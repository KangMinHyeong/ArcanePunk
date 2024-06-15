// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/APTitleUI.h"

#include "Save/APSaveGame.h"
#include "Components/Button.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/Save/APSaveSlotUI.h"
#include "PlayerController/APTitlePlayerController.h"
#include "GameInstance/APGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UAPTitleUI::NativeConstruct()
{
    Super::NativeConstruct();

	SetIsFocusable(true);
    SetKeyboardFocus();
	
	Button_NewGame->OnClicked.AddDynamic(this, &UAPTitleUI::NewGame);
	Button_Continue->OnClicked.AddDynamic(this, &UAPTitleUI::Continue);
	Button_Setting->OnClicked.AddDynamic(this, &UAPTitleUI::Setting);
	Button_Exit->OnClicked.AddDynamic(this, &UAPTitleUI::Exit);
}

void UAPTitleUI::NewGame()
{
	UAPSaveGame* SaveGameData = NewObject<UAPSaveGame>();
	if(SaveGameData == nullptr) 
	{
		return;
	}
    
    SaveGameData->InitSaveData();

    auto PS = GetDefault<AArcanePunkPlayerState>();
    if(!PS) return;

	if (!UGameplayStatics::SaveGameToSlot(SaveGameData, PS->PlayerTotalStatus.SaveSlotName, 0))
	{
		return;
	}

    auto GS = GetDefault<AAPGameState>();
    if(!GS) return;

    if (!UGameplayStatics::SaveGameToSlot(SaveGameData, GS->GameData.SaveSlotName, 0))
	{
		return;
	}

	auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;
	GI->SetNewGame(true);

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Intro"));
}

void UAPTitleUI::Continue()
{
	auto SelectSlotUI =  Cast<UAPSaveSlotUI>(CreateWidget(this, SelectSaveSlotClass)); if(!SelectSlotUI) return;
	SelectSlotUI->BindButton();
	SelectSlotUI->BindSlot();
    SelectSlotUI->AddToViewport();
	// RemoveFromParent();
}

void UAPTitleUI::Setting()
{
	auto TitlePC = Cast<AAPTitlePlayerController>(GetOwningPlayer()); if(!TitlePC) return;

	TitlePC->OptionSetting();
}

void UAPTitleUI::Exit()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
