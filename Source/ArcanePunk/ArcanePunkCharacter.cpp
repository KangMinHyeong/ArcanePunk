// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanePunkCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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
}

// Called every frame
void AArcanePunkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(PlayerVec.SizeSquared() > 0 && bCanMove)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
		AddMovementInput(PlayerVec);
	}
	else
	{
		SetActorRotation(GetController()->GetControlRotation());
	}

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

void AArcanePunkCharacter::MoveForward(float AxisValue)
{
	PlayerVec.X = AxisValue;
	if(PlayerVec.SizeSquared() != 0)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
		AddMovementInput(PlayerVec);
	}	
	
}

void AArcanePunkCharacter::MoveRight(float AxisValue)
{
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
		GetWorldTimerManager().SetTimer(Attack_ATimerHandle, this, &AArcanePunkCharacter::Attack_typeA, Attack_CastingTime, false);
	}
	else
	{
		bAttack_A = false;
		bCanMove = true;
		GetWorldTimerManager().ClearTimer(Attack_ATimerHandle);
	}
}

void AArcanePunkCharacter::Attack_typeB()
{
	if(!bAttack_B)
	{
		bAttack_B = true;
		bCanMove = false;
		GetWorldTimerManager().SetTimer(Attack_BTimerHandle, this, &AArcanePunkCharacter::Attack_typeB, Attack_CastingTime, false);
	}
	else
	{
		bAttack_B = false;
		bCanMove = true;
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
		GetWorldTimerManager().SetTimer(Skill_QTimerHandle, this, &AArcanePunkCharacter::Skill_typeQ, Skill_CastingTime, false);
	}
	else
	{
		bSkill_Q = false;
		bCanMove = true;
		GetWorldTimerManager().ClearTimer(Skill_QTimerHandle);
	}
}

void AArcanePunkCharacter::Skill_typeE()
{
	if(!bSkill_E)
	{
		bSkill_E = true;
		bCanMove = false;
		GetWorldTimerManager().SetTimer(Skill_ETimerHandle, this, &AArcanePunkCharacter::Skill_typeE, Skill_CastingTime, false);
	}
	else
	{
		bSkill_E = false;
		bCanMove = true;
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
