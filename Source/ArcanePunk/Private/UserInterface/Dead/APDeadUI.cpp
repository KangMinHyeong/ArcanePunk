// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Dead/APDeadUI.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UAPDeadUI::NativeConstruct()
{
    Super::NativeConstruct();
    Town_Button->OnClicked.AddDynamic(this, &UAPDeadUI::OnChoice_TownButton);
    Title_Button->OnClicked.AddDynamic(this, &UAPDeadUI::OnChoice_TitleButton);
}

void UAPDeadUI::OnChoice_TownButton()
{
    UGameplayStatics::OpenLevel(GetWorld(), LevelName_Town);
}

void UAPDeadUI::OnChoice_TitleButton()
{
    UGameplayStatics::OpenLevel(GetWorld(), LevelName_Title);
}
