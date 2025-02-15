// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Dead/APDeadUI.h"

#include "Components/Button.h"
#include "GameInstance/APGameInstance.h"

void UAPDeadUI::NativeConstruct()
{
    Super::NativeConstruct();
    Town_Button->OnClicked.AddDynamic(this, &UAPDeadUI::OnChoice_TownButton);
    Title_Button->OnClicked.AddDynamic(this, &UAPDeadUI::OnChoice_TitleButton);
}

void UAPDeadUI::OnChoice_TownButton()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;
    GI->OpenLevel(GetWorld(), LevelName_Town);
}

void UAPDeadUI::OnChoice_TitleButton()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;
    GI->OpenLevel(GetWorld(), LevelName_Title);
}
