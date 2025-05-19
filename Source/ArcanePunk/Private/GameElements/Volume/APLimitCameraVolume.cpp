
#include "GameElements/Volume/APLimitCameraVolume.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSpringArmComponent.h"
#include "Components/Character/APFadeOutTriggerComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAPLimitCameraVolume::AAPLimitCameraVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	AreaTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaTrigger"));
	
	SetRootComponent(AreaTrigger);
}

void AAPLimitCameraVolume::BeginPlay()
{
	Super::BeginPlay();

	InitArea();
	

	SetActorTickEnabled(false);

	AreaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPLimitCameraVolume::OnOverlap);
	AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPLimitCameraVolume::OnOverlapEnd);
}

bool AAPLimitCameraVolume::CheckLimit(float X, float Y)
{
	switch (CurrentArea)
	{
	case ECurrentArea::Area_1:
		if(X >= 5.0f && Y <= -5.0f) {return false;}
		break;

	case ECurrentArea::Area_2:
		if(X <= -5.0f && Y <= -5.0f) return false;
		break;

	case ECurrentArea::Area_3:
		if(X <= -5.0f && Y >= 5.0f) return false;
		break;

	case ECurrentArea::Area_4:
		if(X >= 5.0f && Y >= 5.0f) return false;
		break;
	}

    return true;
}

void AAPLimitCameraVolume::CheckPlayerLocation(float X, float Y, bool first)
{
	if((X <= AreaPoint_1.X && X > AreaPoint_2.X && Y <= AreaPoint_1.Y && Y > AreaPoint_2.Y) || (X >= AreaPoint_1.X && Y <= AreaPoint_1.Y) || (X > AreaPoint_2.X && Y < AreaPoint_2.Y))
	{
		if(CurrentArea == ECurrentArea::Area_2) {FixedSpringArmLocation = AreaPoint_2;}
		if(CurrentArea == ECurrentArea::Area_4) {FixedSpringArmLocation = AreaPoint_1;}

		if(first)
		{
			FinalPoint_1 = AreaPoint_1; FinalPoint_1.Z = GetActorLocation().Z;
			FinalPoint_2 = AreaPoint_2; FinalPoint_2.Z = GetActorLocation().Z;
		}

		CurrentArea = ECurrentArea::Area_1;
	}
	else if((X <= AreaPoint_2.X && X > AreaPoint_3.X && Y >= AreaPoint_2.Y && Y < AreaPoint_3.Y) || (X <= AreaPoint_2.X && Y <= AreaPoint_2.Y) || (X < AreaPoint_3.X && Y < AreaPoint_3.Y))
	{
		if(CurrentArea == ECurrentArea::Area_1) FixedSpringArmLocation = AreaPoint_2;
		if(CurrentArea == ECurrentArea::Area_3) FixedSpringArmLocation = AreaPoint_3;

		if(first)
		{
			FinalPoint_1 = AreaPoint_3; FinalPoint_1.Z = GetActorLocation().Z;
			FinalPoint_2 = AreaPoint_2; FinalPoint_2.Z = GetActorLocation().Z;
		}
		
		CurrentArea = ECurrentArea::Area_2;
	}
	else if((X >= AreaPoint_3.X && X < AreaPoint_4.X && Y >= AreaPoint_3.Y && Y < AreaPoint_4.Y) || (X <= AreaPoint_3.X && Y >= AreaPoint_3.Y) || (X < AreaPoint_4.X && Y > AreaPoint_4.Y))
	{
		if(CurrentArea == ECurrentArea::Area_2) FixedSpringArmLocation = AreaPoint_3;
		if(CurrentArea == ECurrentArea::Area_4) FixedSpringArmLocation = AreaPoint_4;

		if(first)
		{
			FinalPoint_1 = AreaPoint_3; FinalPoint_1.Z = GetActorLocation().Z;
			FinalPoint_2 = AreaPoint_4; FinalPoint_2.Z = GetActorLocation().Z;
		}

		CurrentArea = ECurrentArea::Area_3;
	}
	else
	{
		if(CurrentArea == ECurrentArea::Area_1)  FixedSpringArmLocation = AreaPoint_1;
		if(CurrentArea == ECurrentArea::Area_3)  FixedSpringArmLocation = AreaPoint_4;

		if(first)
		{
			FinalPoint_1 = AreaPoint_1; FinalPoint_1.Z = GetActorLocation().Z;
			FinalPoint_2 = AreaPoint_4; FinalPoint_2.Z = GetActorLocation().Z;
		}

		CurrentArea = ECurrentArea::Area_4;
	}
}

void AAPLimitCameraVolume::LineTrace(float &X, float &Y)
{
	if(!Player.IsValid()) return;
	FHitResult Hit_1; FHitResult Hit_2;
	FVector Start = Player->GetAPSpringArm()->GetComponentLocation(); Start.Z = GetActorLocation().Z;

	FVector End_1 = GetActorLocation(); End_1.X = Start.X;
	FVector End_2 = GetActorLocation(); End_2.Y = Start.Y;
	bool check = false;
	if(GetWorld()->LineTraceSingleByChannel(Hit_1, Start, End_1, ECC_GameTraceChannel8))
	{
		check = true;
		FinalPoint_1 = Hit_1.Location;
		// DrawDebugSphere(GetWorld(),
		// 	FinalPoint_1,
		// 	65.0f,
		// 	12,
		// 	FColor::Red,
		// 	false,
		// 	0.1f,
		// 	0,
		// 	5);
	} 
	else
	{

	}
	
	if(GetWorld()->LineTraceSingleByChannel(Hit_2, Start, End_2, ECC_GameTraceChannel8))
	{
		check = true;
		FinalPoint_2 = Hit_2.Location;

		// DrawDebugSphere(GetWorld(),
		// 	FinalPoint_2,
		// 	65.0f,
		// 	12,
		// 	FColor::Red,
		// 	false,
		// 	0.1f,
		// 	0,
		// 	5);
	} 
	else
	{

	}
	
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

void AAPLimitCameraVolume::InitArea()
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
}

void AAPLimitCameraVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Player.IsValid() && bEnd)
	{
		FVector & Current = Player->GetAPSpringArm()->TargetOffset;
		Current = FMath::VInterpConstantTo(Current, FVector::ZeroVector, DeltaTime, 100.0f);

		if(Current.X <= KINDA_SMALL_NUMBER && Current.Y <= KINDA_SMALL_NUMBER)
		{
			Player = nullptr;
			SetActorTickEnabled(false);
		}
	}
	

	if(Player.IsValid() && !bEnd)
	{
		CheckPlayerLocation(Player->GetAPSpringArm()->GetComponentLocation().X, Player->GetAPSpringArm()->GetComponentLocation().Y);

		float X = Player->GetAPSpringArm()->GetComponentLocation().X - FixedSpringArmLocation.X;
		float Y = Player->GetAPSpringArm()->GetComponentLocation().Y - FixedSpringArmLocation.Y;
		
		float Min_X = 0.0f; float Min_Y = 0.0f;
				
		if(CheckLimit(X,Y))
		{
			LineTrace(Min_X, Min_Y);
			
			Player->GetAPSpringArm()->TargetOffset.X = Min_X - X;
			Player->GetAPSpringArm()->TargetOffset.Y = Min_Y - Y;
		}
		else
		{
			Player->GetAPSpringArm()->TargetOffset.X = Min_X - X;
			Player->GetAPSpringArm()->TargetOffset.Y = Min_Y - Y;
		}
	}
}

void AAPLimitCameraVolume::InitCondition()
{
	if(!Player.IsValid()) return;
	CurrentArea = ECurrentArea::None;
	bEnd = true;
}

void AAPLimitCameraVolume::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(Cast<AArcanePunkCharacter>(OtherActor))
	{
		if(!AreaTrigger->OnComponentEndOverlap.IsBound())
		AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPLimitCameraVolume::OnOverlapEnd);
		
		Player = Cast<AArcanePunkCharacter>(OtherActor);
		InitCondition();
	}
}

void AAPLimitCameraVolume::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if(Cast<AArcanePunkCharacter>(OtherActor))
	{
		Player = Cast<AArcanePunkCharacter>(OtherActor);
		FixedSpringArmLocation = Player->GetAPSpringArm()->GetComponentLocation() - Player->GetActorForwardVector() * Player->GetCapsuleComponent()->GetScaledCapsuleRadius();
		SetActorTickEnabled(true);
		bEnd = false;
		
		CheckPlayerLocation(FixedSpringArmLocation.X, FixedSpringArmLocation.Y, true);
	}
}
