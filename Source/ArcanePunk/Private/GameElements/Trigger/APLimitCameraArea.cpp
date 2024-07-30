
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

	InitArea();
	

	SetActorTickEnabled(false);

	AreaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPLimitCameraArea::OnOverlap);
	AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPLimitCameraArea::OnOverlapEnd);
}

bool AAPLimitCameraArea::CheckLimit(float X, float Y)
{
	switch (CurrentArea)
	{
	case ECurrentArea::Area_1:
		if(X >= 0.0f && Y <= 0.0f) {return false;}
		break;

	case ECurrentArea::Area_2:
		if(X <= 0.0f && Y <= 0.0f) return false;
		break;

	case ECurrentArea::Area_3:
		if(X <= 0.0f && Y >= 0.0f) return false;
		break;

	case ECurrentArea::Area_4:
		if(X >= 0.0f && Y >= 0.0f) return false;
		break;
	}

	// if(AreaDegree > 0.0f && AreaDegree < 90.0f)
	// {
	// 	if(X > 0.0f && Y > 0.0f) return false;
	// }
	// else if(AreaDegree > 90.0f && AreaDegree < 180.0f)
	// {
	// 	if(X > 0.0f && Y < 0.0f) return false;
	// }
	// else if(AreaDegree < 0.0f && AreaDegree > -90.0f)
	// {
	// 	if(X < 0.0f && Y > 0.0f) return false;
	// }
	// else if(AreaDegree < -90.0f && AreaDegree > -180.0f)
	// {
	// 	if(X < 0.0f && Y < 0.0f) return false;
	// }

    return true;
}

void AAPLimitCameraArea::CheckPlayerLocation(float X, float Y)
{
	if((X < AreaPoint_1.X && X >= AreaPoint_2.X && Y < AreaPoint_1.Y && Y >= AreaPoint_2.Y) || (X > AreaPoint_1.X && Y < AreaPoint_1.Y) || (X >= AreaPoint_2.X && Y <= AreaPoint_2.Y))
	{
		if(CurrentArea == ECurrentArea::Area_2) FixedSpringArmLocation = AreaPoint_2;
		if(CurrentArea == ECurrentArea::Area_4) FixedSpringArmLocation = AreaPoint_1;

		CurrentArea = ECurrentArea::Area_1;
	}
	else if((X < AreaPoint_2.X && X >= AreaPoint_3.X && Y > AreaPoint_2.Y && Y <= AreaPoint_3.Y) || (X < AreaPoint_2.X && Y < AreaPoint_2.Y) || (X <= AreaPoint_3.X && Y <= AreaPoint_3.Y))
	{
		if(CurrentArea == ECurrentArea::Area_1) FixedSpringArmLocation = AreaPoint_2;
		if(CurrentArea == ECurrentArea::Area_3) FixedSpringArmLocation = AreaPoint_3;

		CurrentArea = ECurrentArea::Area_2;
	}
	else if((X > AreaPoint_3.X && X <= AreaPoint_4.X && Y > AreaPoint_3.Y && Y <= AreaPoint_4.Y) || (X < AreaPoint_3.X && Y > AreaPoint_3.Y) || (X <= AreaPoint_4.X && Y >= AreaPoint_4.Y))
	{
		if(CurrentArea == ECurrentArea::Area_2) FixedSpringArmLocation = AreaPoint_3;
		if(CurrentArea == ECurrentArea::Area_4) FixedSpringArmLocation = AreaPoint_4;

		CurrentArea = ECurrentArea::Area_3;
	}
	else
	{
		if(CurrentArea == ECurrentArea::Area_1) FixedSpringArmLocation = AreaPoint_1;
		if(CurrentArea == ECurrentArea::Area_3) FixedSpringArmLocation = AreaPoint_4;

		CurrentArea = ECurrentArea::Area_4;
	}
}

void AAPLimitCameraArea::LineTrace(float &X, float &Y)
{
	if(!Player.IsValid()) return;
	FHitResult Hit_1; FHitResult Hit_2;
	FVector Start = Player->GetActorLocation(); Start.Z = GetActorLocation().Z;

	FVector End_1 = GetActorLocation(); End_1.X = Start.X;
	FVector End_2 = GetActorLocation(); End_2.Y = Start.Y;
	bool check = false;
	if(GetWorld()->LineTraceSingleByChannel(Hit_1, Start, End_1, ECC_GameTraceChannel8))
	{
		check = true;
		FinalPoint_1 = Hit_1.Location;
	} 
	// else {X = FinalPoint_1.X, Y = FinalPoint_1.Y;}

	if(GetWorld()->LineTraceSingleByChannel(Hit_2, Start, End_2, ECC_GameTraceChannel8))
	{
		check = true;
		FinalPoint_2 = Hit_2.Location;
	} 
	// else {X = FinalPoint_2.X, Y = FinalPoint_2.Y;}

	if(!check) 
	{
		X = X - FixedSpringArmLocation.X;
		Y = Y - FixedSpringArmLocation.Y;
		return;
	}


	if((FixedSpringArmLocation - FinalPoint_1).Size() < (FixedSpringArmLocation - FinalPoint_2).Size()) 
	{
		X = FinalPoint_1.X, Y = FinalPoint_1.Y;
	}
	else {X = FinalPoint_2.X, Y = FinalPoint_2.Y;}	

	X = X - FixedSpringArmLocation.X;
	Y = Y - FixedSpringArmLocation.Y;

}

void AAPLimitCameraArea::InitArea()
{
	AreaDegree = GetActorRotation().Yaw;
	AreaAngle = FMath::DegreesToRadians(AreaDegree);
	AreaWidth = AreaTrigger->GetScaledBoxExtent().X * 2.0f;
	AreaHeight = AreaTrigger->GetScaledBoxExtent().Y * 2.0f;
	AreaCenter = GetActorLocation();
	float cos = FMath::Cos(AreaAngle); float sin = FMath::Sin(AreaAngle);
	float w = AreaWidth * 0.5f; float h = AreaHeight * 0.5f;
	AreaPoint_1 = FVector(AreaCenter.X + cos * (w) + sin * (h), AreaCenter.Y -(- sin * (w) + cos * (h)), AreaCenter.Z);
	AreaPoint_2 = FVector(AreaCenter.X + cos * (-w) + sin * (h), AreaCenter.Y -(- sin * (-w) + cos * (h)), AreaCenter.Z);
	AreaPoint_3 = FVector(AreaCenter.X + cos * (-w) + sin * (-h), AreaCenter.Y -(- sin * (-w) + cos * (-h)), AreaCenter.Z);
	AreaPoint_4 = FVector(AreaCenter.X + cos * (w) + sin * (-h), AreaCenter.Y -(- sin * (w) + cos * (-h)), AreaCenter.Z);

	UE_LOG(LogTemp, Display, TEXT("AreaPoint_1.X : %f, AreaPoint_1.Y : %f"), AreaPoint_1.X, AreaPoint_1.Y);
	UE_LOG(LogTemp, Display, TEXT("AreaPoint_2.X : %f, AreaPoint_2.Y : %f"), AreaPoint_2.X, AreaPoint_2.Y);
	UE_LOG(LogTemp, Display, TEXT("AreaPoint_3.X : %f, AreaPoint_3.Y : %f"), AreaPoint_3.X, AreaPoint_3.Y);
	UE_LOG(LogTemp, Display, TEXT("AreaPoint_4.X : %f, AreaPoint_4.Y : %f"), AreaPoint_4.X, AreaPoint_4.Y);

}

void AAPLimitCameraArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Player.IsValid() && bEnd)
	{
		FVector & Current = Player->GetMySpringArm()->TargetOffset;
		Current = FMath::VInterpConstantTo(Current, FVector::ZeroVector, DeltaTime, 5.0f);

		if(Current.X <= KINDA_SMALL_NUMBER && Current.Y <= KINDA_SMALL_NUMBER)
		{
			Player = nullptr;
			SetActorTickEnabled(false);
		}
	}
	

	if(Player.IsValid())
	{
		CheckPlayerLocation(Player->GetActorLocation().X, Player->GetActorLocation().Y);

		float X = Player->GetActorLocation().X - FixedSpringArmLocation.X;
		float Y = Player->GetActorLocation().Y - FixedSpringArmLocation.Y;

		float Min_X = 0.0f; float Min_Y = 0.0f;

		
		if(CheckLimit(X,Y))
		{
			LineTrace(Min_X, Min_Y);


			Player->GetMySpringArm()->TargetOffset.X = Min_X - X;
			Player->GetMySpringArm()->TargetOffset.Y = Min_Y - Y;

			// UE_LOG(LogTemp, Display, TEXT("Min_X : %f, Min_Y : %f"), Min_X, Min_Y);
			// Min_X = FMath::Min(abs(X), abs(FMath::Tan(AreaAngle) * Y));
			// Min_Y = FMath::Min(abs(Y), abs( X / FMath::Tan(AreaAngle)));
		
			// if(X < 0) Min_X = -Min_X;
			// if(Y < 0) Min_Y = -Min_Y;
		}
		else
		{
			Player->GetMySpringArm()->TargetOffset.X = Min_X - X;
			Player->GetMySpringArm()->TargetOffset.Y = Min_Y - Y;
		}
		// UE_LOG(LogTemp, Display, TEXT("TargetOffset.X : %f, TargetOffset.Y : %f"), Player->GetMySpringArm()->TargetOffset.X, Player->GetMySpringArm()->TargetOffset.Y);
		

		// X_Init = FMath::FInterpConstantTo(X_Init, 0.0f, DeltaTime, InitSpeed);
		// Y_Init = FMath::FInterpConstantTo(Y_Init, 0.0f, DeltaTime, InitSpeed);

		// float Len = (Player->GetActorLocation() - FadeOutTrigger->GetComponentLocation()).Size();
		// FadeOutTrigger->SetBoxExtent(FVector(Len, 32.0f, 32.0f)); 

		// CameraWidth = InitCameraWidth + Player->GetMySpringArm()->TargetOffset.Y;
		// FadeTriggerRot.Pitch = FMath::RadiansToDegrees(UKismetMathLibrary::Atan(CameraWidth/CameraHeight)) + SpringArmRot.Pitch;
		
		// float aba = (90.0f - FMath::RadiansToDegrees(UKismetMathLibrary::Acos(Player->GetMySpringArm()->TargetOffset.X/Len)));
		// FadeTriggerRot.Yaw = -(aba + SpringArmRot.Yaw);
		// FadeOutTrigger->SetRelativeRotation(FadeTriggerRot);
	}
}

void AAPLimitCameraArea::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(Cast<AArcanePunkCharacter>(OtherActor))
	{
		Player = Cast<AArcanePunkCharacter>(OtherActor);
		// auto SA = Player->GetMySpringArm();
		// X_Init = SA->TargetOffset.X;
		// Y_Init = SA->TargetOffset.Y;

		// FadeOutTrigger = Player->GetFadeOutTrigger();
		// FadeTriggerRot = FadeOutTrigger->GetRelativeRotation();

		// SpringArmRot.Pitch = SA->GetComponentRotation().Pitch;
		// CameraHeight = SA->TargetArmLength * FMath::Cos(FMath::DegreesToRadians(abs(SpringArmRot.Pitch)));
		// CameraWidth = SA->TargetArmLength * FMath::Sin(FMath::DegreesToRadians(abs(SpringArmRot.Pitch)));
		// InitCameraWidth = CameraWidth;
		UE_LOG(LogTemp, Display, TEXT("Your In"));
		bEnd = true;
	}
}

void AAPLimitCameraArea::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if(Cast<AArcanePunkCharacter>(OtherActor))
	{
		Player = Cast<AArcanePunkCharacter>(OtherActor);
		FixedSpringArmLocation = Player->GetMySpringArm()->GetComponentLocation() - Player->GetActorForwardVector() * 80.0f;
		SetActorTickEnabled(true);
		bEnd = false;
		// Player->GetMySpringArm()->TargetOffset.X = 0.0f;
		// Player->GetMySpringArm()->TargetOffset.Y = 0.0f;
		
		// FadeOutTrigger = Player->GetFadeOutTrigger();
		// FadeOutTrigger->SetRelativeRotation(FRotator::ZeroRotator);
		// FadeOutTrigger->SetBoxExtent(FVector(Player->GetMySpringArm()->TargetArmLength, 32.0f, 32.0f)); 

		// Player = nullptr;
		// TArray<AActor*> OverlappingActors
		// GetOverlappingActors(OverlappingActors, AAPLimitCameraArea::StaticClass())
		// for(auto Actor : OverlappingActors) Cast<AAPLimitCameraArea>(Actor)->ResetInit();
	}
}
