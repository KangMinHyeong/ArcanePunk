// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanePunkCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "ArcanePunkPlayerState.h"

// Sets default values
AArcanePunkCharacter::AArcanePunkCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Skill_Q_Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Q_Effect"));

	MySpringArm->SetupAttachment(GetRootComponent());
	MyCamera->SetupAttachment(MySpringArm);
	Skill_Q_Effect->SetupAttachment(GetMesh());

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
}

// Called when the game starts or when spawned
void AArcanePunkCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(!MySpringArm) return;
	MaximumSpringArmLength = MySpringArm->TargetArmLength;
	CurrentArmLength = MaximumSpringArmLength;

	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;
	DefaultSlip = GetCharacterMovement()->BrakingFrictionFactor;

	MyPlayerState = GetWorld()->SpawnActor<AArcanePunkPlayerState>(PlayerStateClass);

	if(MyPlayerState == nullptr) UE_LOG(LogTemp, Display, TEXT("Your message"));

	MyPlayerState->MoveSpeed = DefaultSpeed;
}

// Called every frame
void AArcanePunkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookCharacter();

}

// Called to bind functionality to input
void AArcanePunkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AArcanePunkCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AArcanePunkCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("ZoomInOut"), this, &AArcanePunkCharacter::ZoomInOut);
	if(!bAttack_A && !bAttack_B)PlayerInputComponent->BindAction(TEXT("Attack_A"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Attack_typeA);
	if(!bAttack_B && !bAttack_A)PlayerInputComponent->BindAction(TEXT("Attack_B"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Attack_typeB);
	PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Skill_typeQ);
	PlayerInputComponent->BindAction(TEXT("Skill_E"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Skill_typeE);
	PlayerInputComponent->BindAction(TEXT("Jogging"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::StartJog);
	PlayerInputComponent->BindAction(TEXT("Jogging"), EInputEvent::IE_Released, this, &AArcanePunkCharacter::EndJog);
	PlayerInputComponent->BindAction(TEXT("Normal"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::NormalState);
	PlayerInputComponent->BindAction(TEXT("Stun"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::StunState);
	PlayerInputComponent->BindAction(TEXT("KnockBack"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::KnockBackState);
	PlayerInputComponent->BindAction(TEXT("Sleep"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::SleepState);
}

bool AArcanePunkCharacter::IsAttack_A()
{
    return bAttack_A;
}

bool AArcanePunkCharacter::IsAttack_B()
{
    return bAttack_B;
}

bool AArcanePunkCharacter::IsSkill_Q()
{
    return bSkill_Q;
}

bool AArcanePunkCharacter::IsSkill_E()
{
    return bSkill_E;
}

uint8 AArcanePunkCharacter::returnState()
{
    return CurrentState;
}

void AArcanePunkCharacter::MoveForward(float AxisValue)
{
	if(!bCanMove) return;
	PlayerVec.X = AxisValue;
	if(PlayerVec.SizeSquared() != 0)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
		AddMovementInput(PlayerVec);
	}	
	
	// if(PlayerVec.SizeSquared() > 0)
	// {
	// 	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
	// 	AddMovementInput(PlayerVec);
	// }
	// else
	// {
	// 	SetActorRotation(GetController()->GetControlRotation());
	// }
}

void AArcanePunkCharacter::MoveRight(float AxisValue)
{
	if(!bCanMove) return;
	PlayerVec.Y = AxisValue;
	if(PlayerVec.SizeSquared() != 0)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
		AddMovementInput(PlayerVec);
	}
}

void AArcanePunkCharacter::ZoomInOut(float AxisValue)
{
	CurrentArmLength += AxisValue * ZoomCoefficient;
	if(CurrentArmLength > MaximumSpringArmLength) CurrentArmLength = MaximumSpringArmLength;
	else if(CurrentArmLength < MinimumSpringArmLength) CurrentArmLength = MinimumSpringArmLength;

	MySpringArm->TargetArmLength = CurrentArmLength;
	
}

void AArcanePunkCharacter::Attack_typeA()
{
	if(!bAttack_A)
	{
		bAttack_A = true;
		bCanMove = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		NormalAttack(MyPlayerState->ATK);
		GetWorldTimerManager().SetTimer(Attack_ATimerHandle, this, &AArcanePunkCharacter::Attack_typeA, Attack_CastingTime, false);
	}
	else
	{
		bAttack_A = false;
		bCanMove = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetWorldTimerManager().ClearTimer(Attack_ATimerHandle);
	}
}

void AArcanePunkCharacter::Attack_typeB()
{
	if(!bAttack_B)
	{
		bAttack_B = true;
		bCanMove = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		NormalAttack(MyPlayerState->ATK);
		GetWorldTimerManager().SetTimer(Attack_BTimerHandle, this, &AArcanePunkCharacter::Attack_typeB, Attack_CastingTime, false);
	}
	else
	{
		bAttack_B = false;
		bCanMove = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetWorldTimerManager().ClearTimer(Attack_BTimerHandle);
	}
}

void AArcanePunkCharacter::Skill_typeQ()
{
	if(!bSkill_Q)
	{
		bSkill_Q = true;
		bCanMove = false;
		// Skill_Q_Effect->Activate(true);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetWorldTimerManager().SetTimer(Skill_QTimerHandle, this, &AArcanePunkCharacter::Skill_typeQ, Skill_CastingTime, false);
	}
	else
	{
		bSkill_Q = false;
		bCanMove = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetWorldTimerManager().ClearTimer(Skill_QTimerHandle);
	}
}

void AArcanePunkCharacter::Skill_typeE()
{
	if(!bSkill_E)
	{
		bSkill_E = true;
		bCanMove = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetWorldTimerManager().SetTimer(Skill_ETimerHandle, this, &AArcanePunkCharacter::Skill_typeE, Skill_CastingTime, false);
	}
	else
	{
		bSkill_E = false;
		bCanMove = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetWorldTimerManager().ClearTimer(Skill_ETimerHandle);
	}
}

void AArcanePunkCharacter::StartJog()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed * 2.0f;
}

void AArcanePunkCharacter::EndJog()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}

void AArcanePunkCharacter::NormalState()
{
	CurrentState = 0;
	bCanMove = true;
	GetCharacterMovement()->BrakingFrictionFactor = DefaultSlip;
	GetWorldTimerManager().ClearTimer(State_ETimerHandle);
}

void AArcanePunkCharacter::StunState()
{
	CurrentState = 1;
	bCanMove = false;
	GetWorldTimerManager().SetTimer(State_ETimerHandle, this, &AArcanePunkCharacter::NormalState, State_Time, false);
}

void AArcanePunkCharacter::KnockBackState()
{
	CurrentState = 2;
	bCanMove = false;
	FVector KnockBackVec = -GetActorForwardVector() * 1500.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	LaunchCharacter(KnockBackVec, true, true);
	GetWorldTimerManager().SetTimer(State_ETimerHandle, this, &AArcanePunkCharacter::NormalState, State_Time/3, false);
}

void AArcanePunkCharacter::SleepState()
{
	CurrentState = 3;
	bCanMove = false;
	GetWorldTimerManager().SetTimer(State_ETimerHandle, this, &AArcanePunkCharacter::NormalState, State_Time, false);
}

//피격 판정 생성시 이용 예정
//나중에 쓸 함수(피격 시 발동되게)
void AArcanePunkCharacter::SwitchState(uint8 Current)
{
	switch(Current)
	{
		case 0:
		NormalState();
		break;

		case 1:
		StunState();
		break;

		case 2:
		KnockBackState();
		break;

		case 3:
		SleepState();
		break;
	}
}

bool AArcanePunkCharacter::AttackTrace(FHitResult &HitResult, FVector &HitVector)
{
	// FVector Location;
	FRotator Rotation = GetActorRotation();
	// AController* MyController = Cast<AController>(GetController());
	// if(MyController == nullptr)
	// {
	// 	return false;
	// }
	// MyController->GetPlayerViewPoint(Location, Rotation);
	FVector End = GetActorLocation() + Rotation.Vector() * MaxDistance + FVector::UpVector* 25.0f; // 캐릭터와 몬스터의 높이차가 심하면 + FVector::UpVector* MonsterHigh

	// 아군은 타격 판정이 안되게 하는 코드
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);
	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }    
	
	HitVector = -Rotation.Vector();

	//DrawDebugSphere(GetWorld(), End, AttackRadius, 10, FColor::Green, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(AttackRadius);

	return GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);// 타격 판정 인자 Params 인자 추가
}

void AArcanePunkCharacter::NormalAttack(float ATK)
{
	if(CurrentCharacterState != 0) return;
	float Damage = ATK;
	FHitResult HitResult;
	FVector HitVector;
	bool Line = AttackTrace(HitResult, HitVector);
	if(Line)
	{
		if(AActor* Actor = HitResult.GetActor())
		{
			FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
			AController* MyController = Cast<AController>(GetController());
			if(MyController == nullptr) return;
			UE_LOG(LogTemp, Display, TEXT("youy"));
			Actor->TakeDamage(Damage, myDamageEvent, MyController, this);
		}
	}
}

bool AArcanePunkCharacter::VisionTrace(FHitResult &HitResult)
{
	FVector Location;
	FRotator Rotation;
	AController* MyController = Cast<AController>(GetController());
	if(MyController == nullptr)
	{
		return false;
	}
	MyController->GetPlayerViewPoint(Location, Rotation);
	FVector Start = Location;
	FVector End = GetActorLocation();

	return GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel3);
}

void AArcanePunkCharacter::LookCharacter()
{
	FHitResult HitResult;
	bool Line = VisionTrace(HitResult);
	if(Line)
	{
		if(AActor* Actor = HitResult.GetActor())
		{
			Actor->SetActorHiddenInGame(true);
		}
	}
}
