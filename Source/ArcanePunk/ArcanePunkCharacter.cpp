// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanePunkCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AArcanePunkCharacter::AArcanePunkCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	MySpringArm->SetupAttachment(GetRootComponent());
	MyCamera->SetupAttachment(MySpringArm);

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

	
}

// Called every frame
void AArcanePunkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArcanePunkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AArcanePunkCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AArcanePunkCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("ZoomInOut"), this, &AArcanePunkCharacter::ZoomInOut);
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
