// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ArcanePunkPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "DrawDebugHelpers.h"
#include "UserInterface/LoadingFade.h"

AArcanePunkPlayerController::AArcanePunkPlayerController()
{
}

void AArcanePunkPlayerController::SetActivate_R(bool bValue)
{
    bActivate_R = bValue;
    if(bActivate_R) bShowMouseCursor = true;
    else bShowMouseCursor = false;
}

 void AArcanePunkPlayerController::BeginPlay()
{
    Super::BeginPlay();

    StartLoading();
}

void AArcanePunkPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction(TEXT("Status"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::LookStatus);
    InputComponent->BindAction(TEXT("FreeCameraMode"), EInputEvent::IE_Pressed, this, &AArcanePunkPlayerController::FreeCameraMode);
}

void AArcanePunkPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    AArcanePunkCharacter* APCharacter = Cast<AArcanePunkCharacter>(GetPawn());
    if(bActivate_R)
    {
        MouseSkillEvent();
    }
}

void AArcanePunkPlayerController::MouseSkillEvent()
{
    AArcanePunkCharacter* APCharacter = Cast<AArcanePunkCharacter>(GetPawn());
    if(!APCharacter) return;
    UKismetSystemLibrary::DrawDebugCircle(GetWorld(), APCharacter->GetMesh()->GetComponentLocation(), APCharacter->GetR_LimitDist()-50.0f, 200, FColor::Green, 0, 15, FVector(1.f,0.f,0.f), FVector(0.f,1.f,0.f), false);
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
    if(HitResult.bBlockingHit)
    {
        float Distance =  FVector::Distance(APCharacter->GetActorLocation(), HitResult.ImpactPoint);
        bCanSkill_R = ViewInteraction(APCharacter, Distance, HitResult.ImpactPoint);
        if(bCanSkill_R)
        {
            IsCasting(APCharacter, HitResult.ImpactPoint);
        }        
    }
}

void AArcanePunkPlayerController::Casting()
{
    if(bCanSkill_R) bCast = true;
    else bCast = false;
}

void AArcanePunkPlayerController::LookStatus()
{
    if(bFreeCameraMode) return;
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

void AArcanePunkPlayerController::FreeCameraMode() // 수정 필요
{  
    if(!bFreeCameraMode)
    {
        MyCharacter = Cast<AArcanePunkCharacter>(GetPawn());
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

void AArcanePunkPlayerController::IsCasting(AArcanePunkCharacter* APCharacter, FVector HitPoint)
{
    APCharacter->SetSkill_R(true);
    if(bCast)
    {
        UE_LOG(LogTemp, Display, TEXT("Your message"));
        bActivate_R = false;
        bShowMouseCursor = false;
        APCharacter->SetR_Location(HitPoint);
        APCharacter->Cast_R();
        APCharacter->SetSkill_R(bActivate_R);
        bCast = false;
    }
}

bool AArcanePunkPlayerController::ViewInteraction(AArcanePunkCharacter *APCharacter, float Distance, FVector HitPoint)
{
    float LimitDist = APCharacter->GetR_LimitDist();

    if(Distance <= LimitDist)
    {
        UKismetSystemLibrary::DrawDebugCircle(GetWorld(), HitPoint, 100.0f, 200, FColor::Green, 0, 25.0f, FVector(1.f,0.f,0.f), FVector(0.f,1.f,0.f), true);
        return true;
    }
    else
    {
        UKismetSystemLibrary::DrawDebugCircle(GetWorld(), HitPoint, 100.0f, 200, FColor::Red, 0, 25.0f, FVector(1.f,0.f,0.f), FVector(0.f,1.f,0.f), true);
        return false;
    }
    
}

void AArcanePunkPlayerController::StartFadeIn()
{
    auto FadeLoadingWidget = Cast<ULoadingFade>(CreateWidget(this, FadeLoadingWidgetClass));
    if(!FadeLoadingWidget) return;

    FadeLoadingWidget->AddToViewport();
    FadeLoadingWidget->FadeIn();

    if(!LoadingWidget) return;
    LoadingWidget->RemoveFromParent();
}

void AArcanePunkPlayerController::StartFadeOut()
{
    auto FadeLoadingWidget = Cast<ULoadingFade>(CreateWidget(this, FadeLoadingWidgetClass));
    if(!FadeLoadingWidget) return;

    FadeLoadingWidget->AddToViewport();
    FadeLoadingWidget->FadeOut();
}

void AArcanePunkPlayerController::StartLoading()
{
    LoadingWidget = CreateWidget(this, LoadingWidgetClass);
    if(!LoadingWidget) return;
    LoadingWidget->AddToViewport();

    GetWorldTimerManager().SetTimer(LoadTimerHandle, this, &AArcanePunkPlayerController::StartFadeIn, LoadingTime, false);
}