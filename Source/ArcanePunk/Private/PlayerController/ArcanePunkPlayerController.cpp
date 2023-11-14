// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ArcanePunkPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"

AArcanePunkPlayerController::AArcanePunkPlayerController()
{
}

void AArcanePunkPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction(TEXT("Status"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::LookStatus);
    InputComponent->BindAction(TEXT("FreeCameraMode"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::FreeCameraMode);
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

void AArcanePunkPlayerController::FreeCameraMode()
{  
    if(!bFreeCameraMode)
    {
        MyCharacter = Cast<AArcanePunkCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
        if(!MyCharacter) return;
        FreeCamera = GetWorld()->SpawnActor<APawn>(FreeCameraClass, MyCharacter->ReturnCameraTransform());
        if(!FreeCamera) return;
        Possess(FreeCamera);
        bFreeCameraMode = true;
        UE_LOG(LogTemp, Display, TEXT("Your c"));
    }
    else
    {
        TArray<AActor*> Actors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);
        
        for(AActor* Actor : Actors)
        {
            MyCharacter = Cast<AArcanePunkCharacter>(Actor);
        }
        if(!MyCharacter) return;
        bFreeCameraMode = false;
        Possess(MyCharacter);
        if(!FreeCamera) return;
        FreeCamera->Destroy();
    }
	
}