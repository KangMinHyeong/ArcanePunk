// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanePunkCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// prodo
#include "DrawDebugHelpers.h"
#include "Items/APItemBase.h"
#include "UserInterface/APHUD.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "Test/Pickup.h"

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

	// prodo

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;

	PlayerInventory = CreateDefaultSubobject<UAPInventoryComponent>(TEXT("PlayerInventory"));
	// set capacity of inventory
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);
	
}

// Called when the game starts or when spawned
void AArcanePunkCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(!MySpringArm) return;
	MaximumSpringArmLength = MySpringArm->TargetArmLength;
	CurrentArmLength = MaximumSpringArmLength;

	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// prodo
	HUD = Cast<AAPHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
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

	// prodo
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
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


	// prodo
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AArcanePunkCharacter::BeginInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AArcanePunkCharacter::EndInteract);
	PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AArcanePunkCharacter::ToggleMenu);
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


// prodo

void AArcanePunkCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation() };
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());


	if (LookDirection > 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}

			}
		}
	}

	NoInteractableFound();
}

void AArcanePunkCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void AArcanePunkCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget();

		// when display UI whole monitor, then cant use interact
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AArcanePunkCharacter::BeginInteract()
{
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&AArcanePunkCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void AArcanePunkCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}

}

void AArcanePunkCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}

}

void AArcanePunkCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AArcanePunkCharacter::DropItem(UAPItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("drop item is null"));
	}
}

void AArcanePunkCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}
