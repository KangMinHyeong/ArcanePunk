// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/APTitlePlayerController.h"

#include "Blueprint/UserWidget.h"

void AAPTitlePlayerController::BeginPlay()
{
    Super::BeginPlay();

    TitleUI = CreateWidget<UUserWidget>(this, UIWidgetClass);
    if(TitleUI != nullptr)
    {
        TitleUI->AddToViewport();

        FInputModeUIOnly Mode;
        Mode.SetWidgetToFocus(TitleUI->GetCachedWidget());
        SetInputMode(Mode);
        bShowMouseCursor = true;
    }
}

