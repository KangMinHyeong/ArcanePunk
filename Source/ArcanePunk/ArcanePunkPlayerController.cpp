// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanePunkPlayerController.h"
#include "Blueprint/UserWidget.h"

AArcanePunkPlayerController::AArcanePunkPlayerController()
{
}

void AArcanePunkPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction(TEXT("Status"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::LookStatus);
}

void AArcanePunkPlayerController::LookStatus()
{
	if(!bLookStatus)
	{
        bLookStatus = true;
		StatusWidget = CreateWidget(this, StatusWidgetClass);
        if(StatusWidget != nullptr)
        {
            StatusWidget->AddToViewport();

            SetInputMode(UIInputMode);
            bShowMouseCursor = true;
        }
	}
	else
	{
        bLookStatus = false;
        StatusWidget->RemoveFromParent();
        SetInputMode(GameInputMode);
        bShowMouseCursor = false;
	}
}