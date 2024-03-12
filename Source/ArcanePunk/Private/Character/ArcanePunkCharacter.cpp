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
#include "Components/Character/APAnimHubComponent.h"
#include "Components/Character/APTakeDamageComponent.h"
#include "Components/Character/APSpawnFootPrintComponent.h"
#include "NiagaraComponent.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "ArcanePunk/APGameModeBase.h"
#include "Components/Character/APHitPointComponent.h"
#include "Components/CapsuleComponent.h"

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
	StunEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StunEffect"));
	LeftBeamPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftBeamPoint"));
	PlayerPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerPanel"));

	AttackComp = CreateDefaultSubobject<UAPAttackComponent>(TEXT("AttackComp"));
	MoveComp = CreateDefaultSubobject<UAPMovementComponent>(TEXT("MoveComp"));
	APSkillHubComp = CreateDefaultSubobject<UAPSkillHubComponent>(TEXT("APSkillHubComp"));
	AnimHubComp = CreateDefaultSubobject<UAPAnimHubComponent>(TEXT("AnimHubComp"));
	TakeDamageComp = CreateDefaultSubobject<UAPTakeDamageComponent>(TEXT("TakeDamageComp"));
	SpawnStepComp = CreateDefaultSubobject<UAPSpawnFootPrintComponent>(TEXT("SpawnStepComp"));
	HitPointComp = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComp"));
	CrowdControlComp = CreateDefaultSubobject<UAPCrowdControlComponent>(TEXT("CrowdControlComp"));

	MySpringArm->SetupAttachment(GetRootComponent());
	PlayerPanel->SetupAttachment(GetRootComponent());
	MyCamera->SetupAttachment(MySpringArm);
	Weapon->SetupAttachment(GetMesh(),FName("HandWeapon"));
	FootPrint_L->SetupAttachment(GetMesh(), FName("FootPrint_L"));
	FootPrint_R->SetupAttachment(GetMesh(), FName("FootPrint_R"));
	StunEffect->SetupAttachment(GetRootComponent());
	LeftBeamPoint->SetupAttachment(GetMesh(), FName("BeamPoint"));

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
	PlayerInventory->SetSlotsCapacity(15);
	PlayerInventory->SetWeightCapacity(50.0f);
}

void AArcanePunkCharacter::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<AArcanePunkPlayerController>(GetController());

	MaximumSpringArmLength = MySpringArm->TargetArmLength;
	CurrentArmLength = MaximumSpringArmLength;

	DefaultMaterial = GetMesh()->GetMaterial(0);
	DefaultSlip = GetCharacterMovement()->BrakingFrictionFactor;
	GetCharacterMovement()->MaxWalkSpeed = MyPlayerTotalStatus.PlayerDynamicData.MoveSpeed;
	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;

	UpdateStatus();
	InitPlayerStatus();
	InitEquipData(WeaponReference, DesiredWeaponID);

	SetHavingSkills();
	APSkillHubComp->UpdateSkill_Q();
	APSkillHubComp->UpdateSkill_E();
	SetRSkill();

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

	PlayerInputComponent->BindAction(TEXT("Attack_A"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Attack_typeA); // 연속 공격
	PlayerInputComponent->BindAction(TEXT("Attack_B"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Attack_typeB); // 차징 공격

	PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::SkillBase_Q);
	PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Released, this, &AArcanePunkCharacter::Release_Q);

	PlayerInputComponent->BindAction(TEXT("Skill_E"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::SkillBase_E);
	PlayerInputComponent->BindAction(TEXT("Skill_E"), EInputEvent::IE_Released, this, &AArcanePunkCharacter::Release_E);

	PlayerInputComponent->BindAction(TEXT("Skill_R"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::SkillBase_R);
	PlayerInputComponent->BindAction(TEXT("Skill_R"), EInputEvent::IE_Released, this, &AArcanePunkCharacter::Release_R);

	PlayerInputComponent->BindAction(TEXT("Jogging"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::StartJog);
	PlayerInputComponent->BindAction(TEXT("Jogging"), EInputEvent::IE_Released, this, &AArcanePunkCharacter::EndJog);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Dash);

	PlayerInputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::OnBlockMode);
	
	PlayerInputComponent->BindAction(TEXT("Save"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Save);

	PlayerInputComponent->BindAction(TEXT("WorldMap"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::WorldMap);
	
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

void AArcanePunkCharacter::MoveForward(float AxisValue)
{
	FVector2D PlayerVec;
	PlayerVec.X = AxisValue;
	if (!HUD->TutorialDone)
	{
		if (PlayerVec.X > 0) HUD->UpdateTutorialWidget("PressUp");
		else if (PlayerVec.X < 0) HUD->UpdateTutorialWidget("PressDown");
	}
	if(StopState.IsEmpty()) MoveComp->PlayerMoveForward(AxisValue);

}

void AArcanePunkCharacter::MoveRight(float AxisValue)
{
	FVector2D PlayerVec;
	PlayerVec.Y = AxisValue;
	if (!HUD->TutorialDone)
	{
		if (PlayerVec.Y > 0) HUD->UpdateTutorialWidget("PressRight");
		else if (PlayerVec.Y < 0) HUD->UpdateTutorialWidget("PressLeft");
	}

	if(StopState.IsEmpty()) MoveComp->PlayerMoveRight(AxisValue);

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
	if(bDoing || !StopState.IsEmpty()) return;
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("ClickRight");
	AttackComp->StartAttack_A(bCanMove);	
}

void AArcanePunkCharacter::Attack_typeB()
{

	if(bDoing || !bCanMove || !StopState.IsEmpty()) return;
	bDoing = true;
	AttackComp->StartAttack_B(bCanMove);

	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("ClickLeft");
}

void AArcanePunkCharacter::HideClear()
{
	if(bHideMode)
	{
		SetHideMode(false);
		GetWorldTimerManager().ClearTimer(HideTimerHandle);
		GetWorldTimerManager().SetTimer(HideTimerHandle, this, &AArcanePunkCharacter::HideCheck, ReturnToHideTime, false);
	}
}

void AArcanePunkCharacter::HideCheck()
{
	if(InArcaneTent) SetHideMode(true);
}

float AArcanePunkCharacter::CriticalCalculate(float Multiple)
{
	float CriticalMultiple = MyPlayerTotalStatus.PlayerDynamicData.CriticalStep * (Multiple - 1.0f) + 1;
    return CriticalMultiple;
}

void AArcanePunkCharacter::InitEquipData(TArray<UAPItemBase *> & EquipArr, FName EquipID)
{
	//후에 세이브 데이터 생기면 로직 추가해야할 듯
	if(EquipArr.IsEmpty())
	{
		if (EquipDataTable && !EquipID.IsNone())
		{
			const FItemData* ItemData = EquipDataTable->FindRow<FItemData>(EquipID, EquipID.ToString());

			auto ItemReference = NewObject<UAPItemBase>(this, UAPItemBase::StaticClass());

			ItemReference->ID = ItemData->ID;
			ItemReference->ItemType = ItemData->ItemType;
			ItemReference->ItemQuality = ItemData->ItemQuality;
			ItemReference->ItemNumericData = ItemData->ItemNumericData;
			ItemReference->ItemTextData = ItemData->ItemTextData;
			ItemReference->ItemAssetData = ItemData->ItemAssetData;
			ItemReference->ItemStatistics = ItemData->ItemStatistics;

			EquipArr.Add(ItemReference);
		}
	}
	
}

void AArcanePunkCharacter::ChangeEquipData(TArray<UAPItemBase *> & EquipArr, UAPItemBase *NewData)
{
	// 현재는 테스트용 나중에 강화 등 캐릭터 스테이터스에 변화를 고려하여 수정
	if(EquipArr.IsEmpty())
	{
		if(NewData) EquipArr.Add(NewData);
	}
	else
	{
		EquipArr.Empty();
		if(NewData) EquipArr.Add(NewData);;
	}
	
	UpdateStatus();
	if(PC.IsValid()) PC->OnUpdateStatusText.Broadcast();
}

void AArcanePunkCharacter::SkillBase_Q()
{
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("PressQ");
	if(bCanSkill && bCanMove && StopState.IsEmpty()) APSkillHubComp->PressQ();	
	OnQSkill = true;
}

void AArcanePunkCharacter::SkillBase_E()
{
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("PressE");
	if(bCanSkill && bCanMove && StopState.IsEmpty()) APSkillHubComp->PressE();
	OnESkill = true;
}

void AArcanePunkCharacter::SkillBase_R()
{
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("PressR");
	if(bCanSkill && bCanMove && StopState.IsEmpty()) APSkillHubComp->PressSpace();
	OnRSkill = true;
}

void AArcanePunkCharacter::StartJog()
{
	if(bCanJog) GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed * 2.0f;
	if (!HUD->TutorialDone) HUD->UpdateTutorialWidget("PressShift + PressMove");
}

void AArcanePunkCharacter::EndJog()
{
	if(bCanJog) GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}

void AArcanePunkCharacter::Jump()
{
	if(!bCanMove || !StopState.IsEmpty() || bDoing) return;
	Super::Jump();
}

void AArcanePunkCharacter::Dash()
{
	if(!bCanMove || !StopState.IsEmpty() || bDoing || IsDead()) return;
	FVector DashDirection = GetActorForwardVector() * DashSpeed;
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	LaunchCharacter(DashDirection, true, true);
}

void AArcanePunkCharacter::OnBlockMode()
{ 
	bBlockMode = true;
	GetWorldTimerManager().SetTimer(BlockTimerHandle, this, &AArcanePunkCharacter::ClearBlockMode, BlockTime, false);
}

void AArcanePunkCharacter::ClearBlockMode()
{
	bBlockMode = false;
	GetWorldTimerManager().ClearTimer(BlockTimerHandle);
}

void AArcanePunkCharacter::SetSkillAbility(EEnhanceCategory EnhanceCategory, EEnHanceType EnHanceType)
{
	if(!HUD) return;

	HUD->DisplayEnhanceChoice(EnhanceCategory, EnHanceType);
	bInteract = false;
}

float AArcanePunkCharacter::GetAttackMoveSpeed(int32 Section)
{
	float Speed = 0.0f;
	if (Section == 1) Speed = Attack1_MoveSpeed;
	else if(Section == 2) Speed = Attack2_MoveSpeed;
	else if(Section == 3) Speed = Attack3_MoveSpeed;
    return Speed;
}

void AArcanePunkCharacter::Save()
{
	if(!PC.IsValid()) return;
	PC->OpenSaveSlot();
}

void AArcanePunkCharacter::SaveStatus(FString PlayerSlotName, FString GameSlotName)
{
	// if (!HUD->TutorialDone) {
	// 	HUD->UpdateTutorialWidget("PressCtrl + 9");
	// 	return;
	// }
	MyPlayerTotalStatus.SaveSlotName = PlayerSlotName;
	MyPlayerTotalStatus.PlayerDynamicData.PlayerLocation = GetActorLocation();
	
	MyPlayerState->UpdatePlayerData(MyPlayerTotalStatus);

	auto MyGameState = Cast<AAPGameState>(UGameplayStatics::GetGameState(GetWorld()));
	MyGameStatus.SaveSlotName = GameSlotName;
	MyGameStatus.LevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));

	MyGameState->UpdateGameData(MyGameStatus);	

	if(!PC.IsValid()) return;
	PC->StartSaveUI();
}

void AArcanePunkCharacter::SetRSkill()
{
	if( (QSkill == ESkillNumber::Skill_5 &&  ESkill == ESkillNumber::Skill_6) || (QSkill == ESkillNumber::Skill_6 &&  ESkill == ESkillNumber::Skill_5) ){ RSkill = EUltSkillNumber::UltSkill_1;}
	else {RSkill = EUltSkillNumber::None;}

	APSkillHubComp->UpdateSkill_R();
}

void AArcanePunkCharacter::WorldMap()
{
	auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); if(!GM) return; 
	if(!GM->IsBattleStage()) return;
	if(HUD) HUD->OpenWorldMap();
}

void AArcanePunkCharacter::CurrentPlayerLocation() // 후에 사용할지 말지 결정, 현재는 필요없는듯?
{
	// if(MyPlayerTotalStatus.PlayerDynamicData.SaveOperation)
	// {
	// 	SetActorLocation(MyPlayerTotalStatus.PlayerDynamicData.PlayerLocation);
	// }
}

FTransform AArcanePunkCharacter::ReturnCameraTransform()
{
    return MyCamera->GetComponentTransform();
}

void AArcanePunkCharacter::SetHavingSkills()
{
	HavingSkill.Empty(); 
	HavingSkill.Add(QSkill); HavingSkill.Add(ESkill);
}

void AArcanePunkCharacter::SetInArcaneTent(bool NewBool)
{
	InArcaneTent = NewBool;

	SetHideMode(InArcaneTent);
}

void AArcanePunkCharacter::SetHideMode(bool NewBool)
{
	bHideMode = NewBool;
	if(bHideMode)
	{
		if(PC.IsValid()) PC->SetHideUI(true);
		GetMesh()->SetMaterial(0,HideMaterial);
		// HideUI 생성 및 캐릭터 머터리얼 변경
	}
	else
	{
		if(PC.IsValid()) PC->SetHideUI(false);
		GetMesh()->SetMaterial(0,GetDefaultMaterial());
		// HideUI 삭제 및 캐릭터 머터리얼 원상태
	}
}

float AArcanePunkCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float OriginHP = MyPlayerTotalStatus.PlayerDynamicData.HP;
	if(!bBlockMode)
	{
		TakeDamageComp->DamageCalculation(DamageApplied);
		if(PC.IsValid()) PC->OnUpdateStatusText.Broadcast();
		if(HUD) HUD->OnUpdateHPBar.Broadcast(OriginHP);
	}
    return DamageApplied;
}

bool AArcanePunkCharacter::IsDead()
{
	if(!MyPlayerState) return false;
	
    return MyPlayerTotalStatus.PlayerDynamicData.HP<=0;
}

void AArcanePunkCharacter::DeadPenalty(float DeathTime)
{
	if(!PC.IsValid()) return;
	DisableInput(PC.Get());
	// if(MyPlayerState) MyPlayerState->DeathPenalty();
	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, PC.Get(), &AArcanePunkPlayerController::DisplayDeadUI, DeathTime, false);
}

FTransform AArcanePunkCharacter::GetFootTransform(bool Left)
{
	if(Left) return FootPrint_L->GetComponentTransform();
    return FootPrint_R->GetComponentTransform();
}

USkeletalMeshComponent *AArcanePunkCharacter::GetPlayerEquipment(uint8 NewValue)
{
	switch(NewValue)
	{
		case 1:
		return Weapon;
		break;
	}
    return nullptr;
}

UAPItemBase *AArcanePunkCharacter::GetEquipData(uint8 NewValue)
{
	switch(NewValue)
	{
		case 1:
		return (WeaponReference.IsEmpty() ? nullptr : WeaponReference.Top());
		break;
	}
    return nullptr;
}

void AArcanePunkCharacter::SetEquipData(uint8 NewValue, UAPItemBase *NewData)
{
	switch(NewValue)
	{
		case 1:
		ChangeEquipData(WeaponReference, NewData);
		break;
	}
}

bool AArcanePunkCharacter::PMCheck(FHitResult &HitResult, FVector OverlapStart, FVector OverlapEnd) // 캐릭터 발 밑 피지컬머터리얼 체크
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;
	 
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GetCapsuleComponent()->GetScaledCapsuleRadius());
	 
	return GetWorld()->SweepSingleByChannel(HitResult, OverlapStart, OverlapEnd, FQuat::Identity, ECC_Pawn, Sphere, Params);
}

void AArcanePunkCharacter::UpdateStatus()
{
	WeaponReference.IsEmpty() ? FinalATK = MyPlayerTotalStatus.PlayerDynamicData.ATK : FinalATK = MyPlayerTotalStatus.PlayerDynamicData.ATK + WeaponReference.Top()->ItemStatistics.DamageValue;
	 
}

float AArcanePunkCharacter::GetFinalATK(float Multiple)
{
	float Percent = FMath::RandRange(0.0f, 100.0f);
	if(Percent <= MyPlayerTotalStatus.PlayerDynamicData.CriticalPercent)
	{
		bCriticalAttack = true;
		return FinalATK * CriticalCalculate(Multiple);
	}
	else {bCriticalAttack= false;}

	return FinalATK;
}

void AArcanePunkCharacter::InitPlayerStatus()
{
	MyPlayerState = Cast<AArcanePunkPlayerState>(GetPlayerState());
	if(!MyPlayerState) return;
	MyPlayerTotalStatus = MyPlayerState->PlayerTotalStatus;

	CurrentPlayerLocation();
}

void AArcanePunkCharacter::BeginInteract()
{
	bInteract = true;

	if(!InteractionActors.IsEmpty())
	{
		auto Interface = Cast<IInteractionInterface>(InteractionActors.Top());
		if(Interface)
		{
			Interface->Interact(this);
		}
	}
}

void AArcanePunkCharacter::EndInteract()
{
	bInteract = false;
}

void AArcanePunkCharacter::ActivateInteractionSweep()
{
	TArray<FHitResult> HitResults;
	FCollisionShape Sphere =  FCollisionShape::MakeSphere(InteractionRadius);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllDynamicObjects); 
	if(GetWorld()->SweepMultiByObjectType(HitResults, GetActorLocation(), GetActorLocation()+GetActorUpVector()*15.0f, FQuat::Identity, ObjectQueryParam, Sphere))// 타격 판정 인자 Params 인자 추가
	{
		InteractionActors.Empty();
		for(auto HitResult : HitResults)
		{
			auto Interface = Cast<IInteractionInterface>(HitResult.GetActor());
			if(Interface)
			{
				InteractionActors.Add(HitResult.GetActor());
			}
		}
		if(InteractionActors.IsEmpty()) return;

		if(InteractionActors.Num() >= 2)
		{
			InteractionActors.Sort([this](AActor& A, AActor& B) 
			{
				return (A.GetActorLocation() - GetActorLocation()).Size() > (B.GetActorLocation() - GetActorLocation()).Size();
			});
		}
		if(PC.IsValid())
		{
			auto Interface = Cast<IInteractionInterface>(InteractionActors.Top());
			if(Interface)
			{
				PC->OpenInteraction(InteractionActors.Top(), Interface->GetInteractData());
			}
		} 
	}
}

void AArcanePunkCharacter::InteractionActorRemove(AActor* InteractionActor)
{
	if(InteractionActors.IsEmpty()) return;

	InteractionActors.Remove(InteractionActor);
}

// prodo
void AArcanePunkCharacter::DropItems(UAPItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) + GetActorUpVector() * 150.0f };
		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(DropClass, SpawnLocation, GetActorRotation(), SpawnParams);

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

	// HUD->ToggleMenu();
	
	if (HUD)
	{
		UpdateInventoryWidgetPosition(PlayerInventory->GetInventoryContents().Num());
	}
	
}

void AArcanePunkCharacter::UpdateInventoryWidgetPosition(int32 Numbers)
{
	if (HUD)
	{
		UMainMenu* Inventory;
		Inventory = HUD->GetInventoryWidget();

		// 아이템 개수에 따라서 UI의 위치 수정
		// one item y size = 80

		FVector2D ViewportSize = HUD->GetViewportSize();
		FVector2D WidgetSize = Inventory->GetDesiredSize();

		FVector2D BasePosition = FVector2D(ViewportSize.X - (WidgetSize.X / 2.0f), ViewportSize.Y - (WidgetSize.Y / 2.0f));
		
		FVector2D NewPosition = BasePosition;

		// 1개당 50 +

		switch (Numbers)
		{
		case 0:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y + 120.0f);
			break;
		case 1:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y + 80.0f);
			break;
		case 2:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y + 40.0f);
			break;
		case 3:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y);
			break;
		case 4:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y - 40.0f);
			break;
		case 5:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y - 80.0f);
			break;
		case 6:
		default:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y - 120.0f);
			break;
		}
		

		Inventory->SetPositionInViewport(NewPosition);
	}
}

void AArcanePunkCharacter::InventorySort()
{
	if(PlayerInventory)
	{
		PlayerInventory->SortingInventory();
	}
}

