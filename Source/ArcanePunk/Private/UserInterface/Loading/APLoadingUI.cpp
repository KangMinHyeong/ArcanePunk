// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Loading/APLoadingUI.h"

#include "Components/TextBlock.h"
#include "GameInstance/APGameInstance.h"

void UAPLoadingUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    // auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  
    // APGI->SetTextBlock(Text_Loading, Loading);
}