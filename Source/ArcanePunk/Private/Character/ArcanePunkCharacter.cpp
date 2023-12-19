// Fill out your copyright notice in the Description page of Project Settings.

// Minhyeong
#include "Character/ArcanePunkCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Controller.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Save/APSaveGame.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APSkillNumber.h"
#include "Components/Character/APAnimHubComponent.h"
#include "Components/Character/APTakeDamageComponent.h"
#include "Components/Character/APSpawnFootPrintComponent.h"

// prodo
#include "DrawDebugHelpers.h"
#include "kismet/KismetSystemLibrary.h"
#include "Items/APItemBase.h"
#include "UserInterface/APHUD.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Test/Pickup.h"
#include "UserInterface/APTuTorialUserWidget.h"

// Minhyeong
AArcanePunkCharacter::AArcanePunkCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	FootPrint_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootPrint_L"));
	FootPrint_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootPrint_R"));
	HitMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("HitMaterial"));
	AttackComp = CreateDefaultSubobject<UAPAttackComponent>(TEXT("AttackComp"));
	MoveComp = CreateDefaultSubobject<UAPMovementComponent>(TEXT("MoveComp"));
	SkillComp = CreateDefaultSubobject<UAPSkillHubComponent>(TEXT("SkillComp"));
	SkillNumberComp = CreateDefaultSubobject<UAPSkillNumber>(TEXT("SkillNumberComp"));
	AnimHubComp = CreateDefaultSubobject<UAPAnimHubComponent>(TEXT("AnimHubComp"));
	TakeDamageComp = CreateDefaultSubobject<UAPTakeDamageComponent>(TEXT("TakeDamageComp"));
	SpawnFootPrintComp = CreateDefaultSubobject<UAPSpawnFootPrintComponent>(TEXT("SpawnFootPrintComp"));

	MySpringArm->SetupAttachment(GetRootComponent());
	MyCamera->SetupAttachment(MySpringArm);
	Weapon->SetupAttachment(GetMesh(),FName("HandWeapon"));
	FootPrint_L->SetupAttachment(GetMesh(), FName("FootPrint_L"));
	FootPrint_R->SetupAttachment(GetMesh(), FName("FootPrint_R"));

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

void AArcanePunkCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaximumSpringArmLength = MySpringArm->TargetArmLength;
	CurrentArmLength = MaximumSpringArmLength;

	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;
	DefaultMaterial = GetMesh()->GetMaterial(0);
	DefaultSlip = GetCharacterMovement()->BrakingFrictionFactor;
	GetCharacterMovement()->MaxWalkSpeed = MyPlayerStatus.MoveSpeed;
	InitPlayerStatus();

	// prodo
	HUD = Cast<AAPHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->UpdateTutorialWidget("NONE");
}

void AArcanePunkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArcanePunkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AArcanePunkCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AArcanePunkCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("ZoomInOut"), this, &AArcanePunkCharacter::ZoomInOut);

	PlayerInputComponent->BindAction(TEXT("Attack_A"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Attack_typeA);
	PlayerInputComponent->BindAction(TEXT("Attack_BorCasting"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Attack_typeB);

	PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Skill_typeQ);
	PlayerInputComponent->BindAction(TEXT("Skill_E"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Skill_typeE);

	PlayerInputComponent->BindAction(TEXT("Skill_R"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Skill_typeSpace);
	PlayerInputComponent->BindAction(TEXT("Jogging"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Skill_typeShift);

	PlayerInputComponent->BindAction(TEXT("Jogging"), EInputEvent::IE_Released, this, &AArcanePunkCharacter::EndJog);

	PlayerInputComponent->BindAction(TEXT("Normal"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::NormalState);
	PlayerInputComponent->BindAction(TEXT("Stun"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::StunState);
	PlayerInputComponent->BindAction(TEXT("KnockBack"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::KnockBackState);
	PlayerInputComponent->BindAction(TEXT("Sleep"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::SleepState);

	PlayerInputComponent->BindAction(TEXT("Save"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::SaveStatus);

	// prodo
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AArcanePunkCharacter::BeginInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AArcanePunkCharacter::EndInteract);
	PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AArcanePunkCharacter::ToggleMenu);
}

void AArcanePunkCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimHubComp->BindAttackCheck();
	AnimHubComp->BindComboCheck();
}

uint8 AArcanePunkCharacter::returnState()
{
    return CurrentState;
}

void AArcanePunkCharacter::MoveForward(float AxisValue)
{
	PlayerVec.X = AxisValue;
	if (!HUD->TutorialDone)
	{
		if (PlayerVec.X > 0) HUD->UpdateTutorialWidget("PressUp");
		else if (PlayerVec.X < 0) HUD->UpdateTutorialWidget("PressDown");
	}

	MoveComp->PlayerMoveForward(AxisValue);
}

void AArcanePunkCharacter::MoveRight(float AxisValue)
{
	PlayerVec.Y = AxisValue;
	if (!HUD->TutorialDone)
	{
		if (PlayerVec.Y > 0) HUD->UpdateTutorialWidget("PressRight");
		else if (PlayerVec.Y < 0) HUD->UpdateTutorialWidget("PressLeft");
	}

	MoveComp->PlayerMoveRight(AxisValue);
}

void AArcanePunkCharacter::ZoomInOut(float AxisValue)
{

	if (!HUD->TutorialDone)
	{
		if (AxisValue < 0) HUD->UpdateTutorialWidget("WheelUp");
		else if (AxisValue > 0) HUD->UpdateTutorialWidget("WheelDown");
	}

	CurrentArmLength += AxisValue * ZoomCoefficient;
	if (CurrentArmLength > MaximumSpringArmLength) CurrentArmLength = MaximumSpringArmLength;
	else if (CurrentArmLength < MinimumSpringArmLength) CurrentArmLength = MinimumSpringArmLength;
	

	MySpringArm->TargetArmLength = CurrentArmLength;
}

void AArcanePunkCharacter::Attack_typeA() //몽타주 델리게이트 사용
{
	if(bDoing) return;
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("ClickRight");
	AttackComp->StartAttack_A(bCanMove);
}

void AArcanePunkCharacter::Attack_typeB()
{
	if(bMouseAttack)
	{
		if(bDoing) return;
		bDoing = true;
		AttackComp->StartAttack_B(bCanMove);
	} 
	else
	{
		AArcanePunkPlayerController* MyController = Cast<AArcanePunkPlayerController>(GetController());
		if(!MyController) return;
		MyController->Casting();
		bMouseAttack = true;
		return;
	}
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("ClickLeft");
}

void AArcanePunkCharacter::Skill_typeQ()
{
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("PressQ");
	SkillComp->PressQ();	
}

void AArcanePunkCharacter::Skill_typeE()
{
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("PressE");
	SkillComp->PressE();
}

void AArcanePunkCharacter::Skill_typeSpace()
{
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("PressR");
	SkillComp->PressSpace();
}

void AArcanePunkCharacter::Skill_typeShift()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed * 2.0f;
	bMouseAttack = false;
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("PressShift + PressMove");
	SkillComp->PressShift();
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

//나중에 쓸 함수(피격 시 발동되게) //피격 판정 생성시 이용 예정
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

float AArcanePunkCharacter::GetSkill3_LimitDist()
{
	return Skill3_LimitDist;
}

void AArcanePunkCharacter::SetSkill3_Location(FVector Vector)
{
	Skill3_Location = Vector;
}

FPlayerData AArcanePunkCharacter::GetPlayerStatus()
{
    return MyPlayerStatus;
}

void AArcanePunkCharacter::SetPlayerStatus(FPlayerData NewPlayerData)
{
	MyPlayerStatus = NewPlayerData;
}

float AArcanePunkCharacter::GetAttackMoveSpeed(int32 Section)
{
	float Speed = 0.0f;
	if (Section == 1) Speed = Attack1_MoveSpeed;
	else if(Section == 2) Speed = Attack2_MoveSpeed;
	else if(Section == 3) Speed = Attack3_MoveSpeed;
    return Speed;
}

float AArcanePunkCharacter::GetPushCoefficient()
{
    return AttackPushCoefficient;
}

UAPMovementComponent *AArcanePunkCharacter::GetAPMoveComponent()
{
    return MoveComp;
}

UMaterialInterface *AArcanePunkCharacter::GetHitMaterial()
{
    return HitMaterial;
}

UMaterialInterface *AArcanePunkCharacter::GetDefaultMaterial()
{
    return DefaultMaterial;
}

UAPAttackComponent *AArcanePunkCharacter::GetAttackComponent()
{
    return AttackComp;
}

float AArcanePunkCharacter::GetMaxDistance()
{
    return MaxDistance;
}

float AArcanePunkCharacter::GetAttackRadius()
{
    return AttackRadius;
}

UParticleSystem *AArcanePunkCharacter::GetComboHitEffect()
{
    return ComboHitEffect;
}

FVector AArcanePunkCharacter::GetComboHitEffectScale()
{
    return HitEffectScale;
}

void AArcanePunkCharacter::SetbMouseAttack(bool NewValue)
{
	bMouseAttack = NewValue;
}

bool AArcanePunkCharacter::GetCanMove()
{
    return bCanMove;
}

void AArcanePunkCharacter::SaveStatus()
{
	if (!HUD->TutorialDone) {
		HUD->UpdateTutorialWidget("PressCtrl + 9");
		return;
	}

	
	MyPlayerStatus.MoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	MyPlayerStatus.PlayerLocation = GetActorLocation();
	MyGameStatus.LevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));
	
	MyPlayerState->UpdatePlayerData(MyPlayerStatus);

	auto MyGameState = Cast<AAPGameState>(UGameplayStatics::GetGameState(GetWorld()));
	MyGameState->UpdateGameData(MyGameStatus);

	AArcanePunkPlayerController* MyController = Cast<AArcanePunkPlayerController>(GetController());
	if(!MyController) return;
	
	MyController->StartSaveUI();
	
}

void AArcanePunkCharacter::CurrentPlayerLocation()
{
	if(MyPlayerStatus.SaveOperation)
	{
		SetActorLocation(MyPlayerStatus.PlayerLocation);
	}
}

FTransform AArcanePunkCharacter::ReturnCameraTransform()
{
    return MyCamera->GetComponentTransform();
}

void AArcanePunkCharacter::SetCanMove(bool NewValue)
{
	bCanMove = NewValue;
}

float AArcanePunkCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	TakeDamageComp->DamageCalculation(DamageApplied);
    return DamageApplied;
}

bool AArcanePunkCharacter::IsDead()
{
	if(!MyPlayerState) return false;
	
    return MyPlayerStatus.HP<=0;
}

bool AArcanePunkCharacter::IsHitting()
{
    return bHitting;
}

void AArcanePunkCharacter::SetHitting(bool NewBool)
{
	bHitting = NewBool;
}

float AArcanePunkCharacter::GetHitMotionTime()
{
    return HitMotionTime;
}

UAPSpawnFootPrintComponent *AArcanePunkCharacter::GetSpawnFootPrintComponent()
{
    return SpawnFootPrintComp;
}

TSubclassOf<AActor> AArcanePunkCharacter::GetFootClass(bool Left)
{	
	if(Left) return LeftFootClass;
    return RightFootClass;
}

FTransform AArcanePunkCharacter::GetFootTransform(bool Left)
{
	if(Left) return FootPrint_L->GetComponentTransform();
    return FootPrint_R->GetComponentTransform();
}

bool AArcanePunkCharacter::PMCheck(FHitResult &HitResult, FVector OverlapStart, FVector OverlapEnd)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;
	
	FCollisionShape Sphere = FCollisionShape::MakeSphere(AttackRadius * 0.75f);

	return GetWorld()->SweepSingleByChannel(HitResult, OverlapStart, OverlapEnd, FQuat::Identity, ECC_Visibility, Sphere, Params);
}

void AArcanePunkCharacter::SetDoing(bool NewBool)
{
	bDoing = NewBool;
}

bool AArcanePunkCharacter::GetDoing()
{
    return bDoing;
}

UAPSkillHubComponent *AArcanePunkCharacter::GetAPSkillHubComponent()
{
    return SkillComp;
}

UAPSkillNumber *AArcanePunkCharacter::GetAPSkillNumberComponent()
{
    return SkillNumberComp;
}

TSubclassOf<class ASwordImpact> AArcanePunkCharacter::GetSwordImpactClass()
{
    return SwordImpactClass;
}

TSubclassOf<class ASwordThrowBase> AArcanePunkCharacter::GetSwordThrowClass()
{
    return SwordThrowClass;
}

void AArcanePunkCharacter::InitPlayerStatus()
{
	MyPlayerState = Cast<AArcanePunkPlayerState>(GetPlayerState());
	if(!MyPlayerState) return;
	MyPlayerStatus = MyPlayerState->PlayerStatus;

	CurrentPlayerLocation();
}

// prodo

void AArcanePunkCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	if (!TextRenderActor)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);// { GetActorLocation() + (GetActorForwardVector() * 50.0f) };
		const FTransform SpawnTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), SpawnLocation);// (GetActorRotation(), SpawnLocation);

		TextRenderActor = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), SpawnTransform, SpawnParams);
		FRotator Rotator = TextRenderActor->GetActorRotation();
		TextRenderActor->SetActorRotation(FRotator(Rotator.Pitch, Rotator.Yaw + 180.0f, Rotator.Roll));
		TextRenderActor->SetActorScale3D(FVector3d(5.0f, 5.0f, 5.0f));
		TextRenderActor->SetActorHiddenInGame(true);
	}

	// add circle collision to player
	// if anyone detect in collision then, the first detected enroll interactable

	TArray<TEnumAsByte<EObjectTypeQuery>> types;
	types.Reserve(1);
	types.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));

	TArray<AActor*> Ignores;
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;
	FHitResult HitResult;
	float DrawTime = 5.0f;

	bool b = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), 500, types, false, Ignores, DrawDebug, HitResult, true, FLinearColor::Red, FLinearColor::Green, DrawTime);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), 500, 26, FColor::Red, false, 0.2f, 0, 2.0f);

	if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		if (HitResult.GetActor() != InteractionData.CurrentInteractable)
		{
			FoundInteractable(HitResult.GetActor());
			return;
		}

		if (HitResult.GetActor() == InteractionData.CurrentInteractable)
		{
			return;
		}

	}

	// linetrace
	/* 
	FVector TraceStart{ GetPawnViewLocation() };
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	LookDirection = -1;

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
	*/
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

	// hud 위치 설정 viewport 상에서 이루어져야함
	//FVector Position = NewInteractable->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%f %f"), Position.X, Position.Y);
	//FVector WPosition = HUD->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%f %f"), WPosition.X, WPosition.Y);
	//HUD->SetInteractableText(FText::FromString("IS IT WORK?"));
	//HUD->SetInteractableTextPosition(Position);
	//HUD->SetActorLocation(Position);
	//WPosition = HUD->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%f %f"), WPosition.X, WPosition.Y);

	if (TextRenderActor)
	{
		//UE_LOG(LogTemp, Warning, TEXT("실행중"));
		TextRenderActor->SetActorHiddenInGame(false);
		FVector Position = NewInteractable->GetActorLocation();
		TextRenderActor->SetActorLocation(FVector(Position.X, Position.Y - 250.0f , Position.Z + 250.0f));
		TextRenderActor->GetTextRender()->SetText(FText::FromString("Press K"));
		//TextRenderActor->GetTextRender()->SetText(FText::FromString(("{0} {1} To Press K.", TargetInteractable->InteractableData.Action, TargetInteractable->InteractableData.Name);  //TargetInteractable->InteractableData.Action + TargetInteractable->InteractableData.Name); // + FText::FromString("To Press K"));
	}

	
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

		if (TextRenderActor)
		{
			TextRenderActor->SetActorHiddenInGame(true);
		}

		HUD->HideInteractionWidget();

		// when display UI whole monitor, then cant use interact
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AArcanePunkCharacter::BeginInteract()
{
	if (!HUD->TutorialDone) {
		HUD->UpdateTutorialWidget("PressF");
		return;
	}

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
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("PressTab");

	HUD->ToggleMenu();
}
