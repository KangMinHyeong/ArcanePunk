// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Title/APTitleUI.h"

#include "Save/APSaveGame.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
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
	
	Button_NewGame->OnClicked.AddDynamic(this, &UAPTitleUI::OnNewGame);
	Button_Continue->OnClicked.AddDynamic(this, &UAPTitleUI::OnContinue);
	Button_Setting->OnClicked.AddDynamic(this, &UAPTitleUI::OnSetting);
	Button_Exit->OnClicked.AddDynamic(this, &UAPTitleUI::OnExit);

	auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  

	APGI->SetTextBlock(TextBlock_NewGame, EStringRowName::NewGame);
	APGI->SetTextBlock(TextBlock_Continue, EStringRowName::Continue);
	APGI->SetTextBlock(TextBlock_GameSetting, EStringRowName::GameSetting);
	APGI->SetTextBlock(TextBlock_GameExit, EStringRowName::GameExit);
	APGI->SetTextBlock(TextBlock_MainTitle, EStringRowName::MainTitle);
	APGI->SetTextBlock(TextBlock_SubTitle, EStringRowName::SubTitle);
}

void UAPTitleUI::OnNewGame()
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

void UAPTitleUI::OnContinue()
{
	auto SelectSlotUI =  Cast<UAPSaveSlotUI>(CreateWidget(this, SelectSaveSlotClass)); if(!SelectSlotUI) return;
	SelectSlotUI->BindButton();
	SelectSlotUI->BindSlot();
    SelectSlotUI->AddToViewport();
	// RemoveFromParent();
}

void UAPTitleUI::OnSetting()
{
	auto TitlePC = Cast<AAPTitlePlayerController>(GetOwningPlayer()); if(!TitlePC) return;

	TitlePC->OptionSetting();
}

void UAPTitleUI::OnExit()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
