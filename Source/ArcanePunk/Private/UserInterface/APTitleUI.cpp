// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/APTitleUI.h"

#include "Save/APSaveGame.h"
#include "Components/Button.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Kismet/GameplayStatics.h"

void UAPTitleUI::NativeConstruct()
{
    Super::NativeConstruct();

	NewGameBtn = Cast<UButton>(GetWidgetFromName(TEXT("btnNewGame")));
	if (NewGameBtn != nullptr)
	{
		NewGameBtn->OnClicked.AddDynamic(this, &UAPTitleUI::NewGame);
	}
	ContinueBtn = Cast<UButton>(GetWidgetFromName(TEXT("btnContinueGame")));
	if (ContinueBtn != nullptr)
	{
		ContinueBtn->OnClicked.AddDynamic(this, &UAPTitleUI::Continue);
	}
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

	if (!UGameplayStatics::SaveGameToSlot(SaveGameData, PS->SaveSlotName, 0))
	{
		return;
	}

    auto GS = GetDefault<AAPGameState>();
    if(!GS) return;

    if (!UGameplayStatics::SaveGameToSlot(SaveGameData, GS->SaveSlotName, 0))
	{
		return;
	}

    UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestMap1"));
}

void UAPTitleUI::Continue()
{
    AAPGameState* GS = Cast<AAPGameState>(UGameplayStatics::GetGameState(this));
	if(GS != nullptr)
	{
        UGameplayStatics::OpenLevel(this, GS->GameData.LevelName);
    }	
}
