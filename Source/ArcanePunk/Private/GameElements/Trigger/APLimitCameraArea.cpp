
#include "GameElements/Trigger/APLimitCameraArea.h"

#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/Character/APFadeOutTriggerComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAPLimitCameraArea::AAPLimitCameraArea()
{
	PrimaryActorTick.bCanEverTick = true;

	AreaTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaTrigger"));
	
	SetRootComponent(AreaTrigger);
}

void AAPLimitCameraArea::BeginPlay()
{
	Super::BeginPlay();

	AreaDegree = GetActorRotation().Yaw; UE_LOG(LogTemp, Display, TEXT("Your %f"), AreaDegree);
	AreaAngle = FMath::DegreesToRadians(AreaDegree);
	
	SetActorTickEnabled(false);

	AreaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPLimitCameraArea::OnOverlap);
	AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPLimitCameraArea::OnOverlapEnd);
}

bool AAPLimitCameraArea::CheckLimit(float X, float Y)
{
	if(AreaDegree > 0.0f && AreaDegree < 90.0f)
	{
		if(X > 0.0f && Y > 0.0f) return false;
	}
	else if(AreaDegree > 90.0f && AreaDegree < 180.0f)
	{
		if(X > 0.0f && Y < 0.0f) return false;
	}
	else if(AreaDegree < 0.0f && AreaDegree > -90.0f)
	{
		if(X < 0.0f && Y > 0.0f) return false;
	}
	else if(AreaDegree < -90.0f && AreaDegree > -180.0f)
	{
		if(X < 0.0f && Y < 0.0f) return false;
	}

    return true;
}

void AAPLimitCameraArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Player.IsValid())
	{
		// if(!X_Limit && !Y_Limit) {return;}
		
		// if(Y_Limit) 
		// {
		// 	float Add = FixedSpringArmLocation.Y - Player->GetActorLocation().Y;
		// 	Player->GetMySpringArm()->TargetOffset.Y = Add;
		// }

		// if(X_Limit) 
		// {
		// 	float Add = FixedSpringArmLocation.X - Player->GetActorLocation().X;
		// 	Player->GetMySpringArm()->TargetOffset.X = Add;
		// }
		float X = Player->GetActorLocation().X - FixedSpringArmLocation.X;
		float Y = Player->GetActorLocation().Y - FixedSpringArmLocation.Y;

		float Min_X = 0.0f; float Min_Y = 0.0f;
		if(CheckLimit(X,Y))
		{
			Min_X = FMath::Min(abs(X), abs(FMath::Tan(AreaAngle) * Y));
			Min_Y = FMath::Min(abs(Y), abs( X / FMath::Tan(AreaAngle)));
		
			if(X < 0) Min_X = -Min_X;
			if(Y < 0) Min_Y = -Min_Y;
		}

		Player->GetMySpringArm()->TargetOffset.X = (X_Init + Min_X - X) * abs(FMath::Cos(AreaAngle));
		Player->GetMySpringArm()->TargetOffset.Y = (Y_Init + Min_Y - Y) * abs(FMath::Sin(AreaAngle));

		X_Init = FMath::FInterpConstantTo(X_Init, 0.0f, DeltaTime, InitSpeed);
		Y_Init = FMath::FInterpConstantTo(Y_Init, 0.0f, DeltaTime, InitSpeed);

		float Len = (Player->GetActorLocation() - FadeOutTrigger->GetComponentLocation()).Size();
		FadeOutTrigger->SetBoxExtent(FVector(Len, 32.0f, 32.0f)); 

		CameraWidth = InitCameraWidth + Player->GetMySpringArm()->TargetOffset.Y;
		FadeTriggerRot.Pitch = FMath::RadiansToDegrees(UKismetMathLibrary::Atan(CameraWidth/CameraHeight)) + SpringArmRot.Pitch;
		
		float aba = (90.0f - FMath::RadiansToDegrees(UKismetMathLibrary::Acos(Player->GetMySpringArm()->TargetOffset.X/Len)));
		FadeTriggerRot.Yaw = -(aba + SpringArmRot.Yaw);
		FadeOutTrigger->SetRelativeRotation(FadeTriggerRot);
	}
}

void AAPLimitCameraArea::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(Cast<AArcanePunkCharacter>(OtherActor))
	{
		Player = Cast<AArcanePunkCharacter>(OtherActor);
		FixedSpringArmLocation = Player->GetMySpringArm()->GetComponentLocation();

		auto SA = Player->GetMySpringArm();
		X_Init = SA->TargetOffset.X;
		Y_Init = SA->TargetOffset.Y;

		FadeOutTrigger = Player->GetFadeOutTrigger();
		FadeTriggerRot = FadeOutTrigger->GetRelativeRotation();

		SpringArmRot.Pitch = SA->GetComponentRotation().Pitch;
		CameraHeight = SA->TargetArmLength * FMath::Cos(FMath::DegreesToRadians(abs(SpringArmRot.Pitch)));
		CameraWidth = SA->TargetArmLength * FMath::Sin(FMath::DegreesToRadians(abs(SpringArmRot.Pitch)));
		InitCameraWidth = CameraWidth;

		SetActorTickEnabled(true);
	}
}

void AAPLimitCameraArea::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if(Cast<AArcanePunkCharacter>(OtherActor))
	{
		SetActorTickEnabled(false);

		Player->GetMySpringArm()->TargetOffset.X = 0.0f;
		Player->GetMySpringArm()->TargetOffset.Y = 0.0f;
		
		FadeOutTrigger = Player->GetFadeOutTrigger();
		FadeOutTrigger->SetRelativeRotation(FRotator::ZeroRotator);
		FadeOutTrigger->SetBoxExtent(FVector(Player->GetMySpringArm()->TargetArmLength, 32.0f, 32.0f)); 

		Player = nullptr;
		// TArray<AActor*> OverlappingActors
		// GetOverlappingActors(OverlappingActors, AAPLimitCameraArea::StaticClass())
		// for(auto Actor : OverlappingActors) Cast<AAPLimitCameraArea>(Actor)->ResetInit();
	}
}
