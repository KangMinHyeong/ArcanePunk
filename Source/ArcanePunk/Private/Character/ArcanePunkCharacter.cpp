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
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Save/APSaveGame.h"
#include "Skill/SwordImpact.h"
#include "Skill/SwordThrowBase.h"
#include "Enemy/Enemy_CharacterBase.h"

// prodo
#include "DrawDebugHelpers.h"
#include "kismet/KismetSystemLibrary.h"
#include "Items/APItemBase.h"
#include "UserInterface/APHUD.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Test/Pickup.h"

// Minhyeong
AArcanePunkCharacter::AArcanePunkCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Q_Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Q_Effect"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	FootPrint_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootPrint_L"));
	FootPrint_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootPrint_R"));

	MySpringArm->SetupAttachment(GetRootComponent());
	MyCamera->SetupAttachment(MySpringArm);
	Q_Effect->SetupAttachment(GetMesh(),FName("SwordEffect"));
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

// Called when the game starts or when spawned
void AArcanePunkCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(!MySpringArm) return;
	MaximumSpringArmLength = MySpringArm->TargetArmLength;
	CurrentArmLength = MaximumSpringArmLength;

	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;

	DefaultSlip = GetCharacterMovement()->BrakingFrictionFactor;

	MyPlayerState = Cast<AArcanePunkPlayerState>(GetPlayerState());

	if(!MyPlayerState) return;
	InitPlayerStatus();

	GetCharacterMovement()->MaxWalkSpeed = MyPlayerStatus.MoveSpeed;

	// prodo
	HUD = Cast<AAPHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called every frame
void AArcanePunkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(AttackMove)
	{
		AttackMoving(DeltaTime);
	}
	// prodo
	// if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	// {
	// 	PerformInteractionCheck();
	// }

	// LookCharacter();
}

// Called to bind functionality to input
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
	PlayerInputComponent->BindAction(TEXT("Skill_R"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Skill_typeR);
	PlayerInputComponent->BindAction(TEXT("Jogging"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::StartJog);
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
	BindAttackCheck();
	BindComboCheck();
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

void AArcanePunkCharacter::Attack_typeA() //몽타주 델리게이트 사용
{
	if(bAttack_A)
	{
		if(!FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo)) return;
		if (CanCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		if(!Anim) return;
		ComboAttackStart();
		BeforeRotation = GetController()->GetControlRotation();
		Anim->PlayAttack_A_Montage();
		Anim->JumpToComboSection(CurrentCombo);
		bAttack_A = true;
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		bCanMove = false;
	}
	
}

void AArcanePunkCharacter::Attack_typeB()
{
	if(bSkill_R)
	{
		AArcanePunkPlayerController* MyController = Cast<AArcanePunkPlayerController>(GetController());
		if(!MyController) return;
		MyController->Casting();
		return;
	}
	else
	{
		if(bAttack_B) return;
		bAttack_B = true;
		if(!Anim) return;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Attack_Sound, GetActorLocation(), E_SoundScale);
		Anim->PlayAttack_B_Montage();
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

void AArcanePunkCharacter::Skill_typeQ()
{
	if(bSkill_Q) return;
	bSkill_Q = true;
	if(!Anim) return;
	Anim->PlaySkill_Q_Montage();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void AArcanePunkCharacter::Skill_typeE()
{
	if(bMarking)
	{
		auto Enemy = Cast<AEnemy_CharacterBase>(MarkingActor);
		if(!Enemy) return;
		SetActorLocation(MarkingLocation);
		Enemy->TeleportMarkDeactivate();
		bMarking = false;
	}
	else
	{
		if(bSkill_E) return;
		else
		{
			bSkill_E = true;
			if(!Anim) return;
			Anim->PlaySkill_E_Montage();
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		}	
	}
	
}

void AArcanePunkCharacter::Skill_typeR()
{
	AArcanePunkPlayerController* MyController = Cast<AArcanePunkPlayerController>(GetController());
	if(!MyController) return;

	if(!bSkill_R)
	{
		bSkill_R = true;
		//UGameplayStatics::SpawnDecalAtLocation(GetWorld(), R_Range_Decal, FVector(R_LimitDist, R_LimitDist, 1), GetActorLocation(), FRotator(-90, 0, 0), 10.0f);
		// UGameplayStatics::SpawnDecalAttached(R_Range_Decal, FVector(R_LimitDist, R_LimitDist, 10), RangeComp);
		// UMaterialInstance
		MyController->SetActivate_R(bSkill_R);
	}
	else
	{
		bSkill_R = false;
		MyController->SetActivate_R(bSkill_R);
	}
}

void AArcanePunkCharacter::Cast_R(FVector HitLocation)
{
	if(!Anim) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), R_Effect, R_Location, FRotator::ZeroRotator, R_Size);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), R_Sound_Cast, R_Location, E_SoundScale);
	NormalAttack(HitLocation, false, 0.4);
	Anim->PlaySkill_R_Montage();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
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

bool AArcanePunkCharacter::AttackTrace(FHitResult &HitResult, FVector &HitVector, FVector Start, bool CloseAttack)
{
	// FVector Location;
	FRotator Rotation = GetActorRotation();
	// AController* MyController = Cast<AController>(GetController());
	// if(MyController == nullptr)
	// {
	// 	return false;
	// }
	// MyController->GetPlayerViewPoint(Location, Rotation);
	FVector End = Start;
	if(CloseAttack) End = End + Rotation.Vector() * MaxDistance + FVector::UpVector* 25.0f; // 캐릭터와 몬스터의 높이차가 심하면 + FVector::UpVector* MonsterHigh
	else End = End + FVector::UpVector* 100.0f;

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

	FCollisionShape Sphere;
	if(CloseAttack) Sphere = FCollisionShape::MakeSphere(AttackRadius);
	else Sphere = FCollisionShape::MakeSphere(AttackRadius*1.25);

	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);// 타격 판정 인자 Params 인자 추가
}

void AArcanePunkCharacter::NormalAttack(FVector Start, bool CloseAttack, float Multiple)
{
	if(CurrentCharacterState != 0) return;
	float Damage = MyPlayerStatus.ATK * Multiple;
	FHitResult HitResult;
	FVector HitVector;
	bool Line = AttackTrace(HitResult, HitVector, Start, CloseAttack);
	if(Line)
	{
		if(AActor* Actor = HitResult.GetActor())
		{
			FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
			AController* MyController = Cast<AController>(GetController());
			if(!MyController) return;
			UE_LOG(LogTemp, Display, TEXT("youy"));
			Actor->TakeDamage(Damage, myDamageEvent, MyController, this);
		}
	}
}

void AArcanePunkCharacter::Activate_Q()
{
	//Q_Effect->Activate(true);
	//GetWorldTimerManager().SetTimer(DeActivate_Q_TimerHandle, this, &AArcanePunkCharacter::DeActivate_Q, Q_ActiveTime, false);

	auto SwordSkill = GetWorld()->SpawnActor<ASwordImpact>(SwordImpactClass, GetActorLocation()+GetActorForwardVector()*150.0f, GetActorRotation());
	if(!SwordSkill) return;
	SwordSkill->SetOwner(this);
}

void AArcanePunkCharacter::Activate_E()
{
	auto SwordThrow = GetWorld()->SpawnActor<ASwordThrowBase>(SwordThrowClass, GetActorLocation()+GetActorForwardVector()*150.0f, GetActorRotation() + FRotator(0,90.0f,0));
	if(!SwordThrow) return;
	SwordThrow->SetOwner(this);
}

void AArcanePunkCharacter::SetSkill_R(bool bValue)
{
	bSkill_R = bValue;
}

float AArcanePunkCharacter::GetR_LimitDist()
{
	return R_LimitDist;
}

void AArcanePunkCharacter::SetR_Location(FVector Vector)
{
	R_Location = Vector;
}

FPlayerData AArcanePunkCharacter::GetPlayerStatus()
{
    return MyPlayerStatus;
}

void AArcanePunkCharacter::MarkingOn(FVector Location, AActor* OtherActor)
{
	bMarking = true;
	MarkingLocation = Location;
	MarkingActor = OtherActor;
	GetWorldTimerManager().SetTimer(MarkTimerHandle, this, &AArcanePunkCharacter::MarkErase, SwordTeleportTime, false);
}

void AArcanePunkCharacter::AnimMovement()
{
	GetCharacterMovement()->BrakingFrictionFactor = Fric;
	AttackMove = true;
}

void AArcanePunkCharacter::AnimMoveStop()
{
	GetCharacterMovement()->BrakingFrictionFactor = DefaultSlip;
	AttackMove = false;
}

void AArcanePunkCharacter::SpawnFootPrint(bool LeftFoot)
{
	FHitResult HitResult;
	if(LeftFoot)
	{
		if(PMCheck(HitResult, GetActorLocation(), GetActorLocation() - GetActorUpVector()*Customfloat))
		{
			if(EPhysicalSurface::SurfaceType2 == UGameplayStatics::GetSurfaceType(HitResult))
			{
				auto FootPrint = GetWorld()->SpawnActor<AActor>(LeftFootClass, FootPrint_L->GetComponentTransform());
			}	
		}
	}
	else
	{
		if(PMCheck(HitResult, GetActorLocation(), GetActorLocation() - GetActorUpVector()*Customfloat))
		{
			if(EPhysicalSurface::SurfaceType2 == UGameplayStatics::GetSurfaceType(HitResult))
			{
				auto FootPrint = GetWorld()->SpawnActor<AActor>(RightFootClass, FootPrint_R->GetComponentTransform());
			}
		}
	}
}

void AArcanePunkCharacter::OnHitting()
{
	bHitting = false;
	GetWorldTimerManager().ClearTimer(HitTimerHandle);
}

void AArcanePunkCharacter::BindAttackCheck()
{
	Anim = Cast<UArcanePunkCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if(!Anim) return;
	// MontageEnd
	Anim->OnMontageEnded.AddDynamic(this, &AArcanePunkCharacter::PlayerMontageEnded);

}

void AArcanePunkCharacter::BindComboCheck()
{
	if(!Anim) return;
	//ComboAttack
	Anim->OnComboCheck.AddLambda([this]()->void {

		CanCombo = false;
		if (IsComboInputOn)
		{
			ComboAttackStart();
			Anim->JumpToComboSection(CurrentCombo);
		}
	});
}

void AArcanePunkCharacter::DeActivate_Q()
{
	// Q_Effect->Deactivate();
	// GetWorldTimerManager().ClearTimer(DeActivate_Q_TimerHandle);
}

void AArcanePunkCharacter::SaveStatus()
{	
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
void AArcanePunkCharacter::ComboAttackStart()
{
	CanCombo = true;
	IsComboInputOn = false;
	if(!FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1)) return;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AArcanePunkCharacter::ComboAttackEnd()
{
	CanCombo = false;
	IsComboInputOn = false;
	CurrentCombo = 0;
}

// 나중에 벽 투명화로 쓸 C++ 코드 (함수 헤더 지움)

// bool AArcanePunkCharacter::VisionTrace(FHitResult &HitResult)
// {
// 	FVector Location;
// 	FRotator Rotation;
// 	AController* MyController = Cast<AController>(GetController());
// 	if(MyController == nullptr)
// 	{
// 		return false;
// 	}
// 	MyController->GetPlayerViewPoint(Location, Rotation);
// 	FVector Start = Location;
// 	FVector End = GetActorLocation();

// 	return GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel3);
// }

// void AArcanePunkCharacter::LookCharacter()
// {
// 	FHitResult HitResult;
// 	bool Line = VisionTrace(HitResult);
// 	if(Line)
// 	{
// 		if(AActor* Actor = HitResult.GetActor())
// 		{
// 			// Actor->MaterialParameters
// 			// GetMaterial
			
// 		}
// 	}
// }

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
	
	DamageApplied = FMath::Min(MyPlayerStatus.HP, DamageApplied);

	MyPlayerStatus.HP = MyPlayerStatus.HP - DamageMath(DamageApplied);
	
	if(IsDead())
	{
	// 	UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
	// 	bDead = true;
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//DetachFromControllerPendingDestroy();
	// 	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABossMonster_Stage1::Destoryed, DeathLoadingTime, false);

	}
	else
	{
		bHitting = true;
	
		UE_LOG(LogTemp, Display, TEXT("Character HP : %f"), MyPlayerStatus.HP);
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AArcanePunkCharacter::OnHitting, HitMotionTime, false);
	}

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

void AArcanePunkCharacter::PlayerMontageEnded(UAnimMontage *Montage, bool bInterrupted)
{
	if(!Anim) return;
	
	if(Montage == Anim->Attack_A_Montage) OnAttack_A_MontageEnded();
	else if(Montage == Anim->Attack_B_Montage) OnAttack_B_MontageEnded();
	else if(Montage == Anim->Skill_Q_Montage) OnSkill_Q_MontageEnded();
	else if(Montage == Anim->Skill_E_Montage) OnSkill_E_MontageEnded();
	else if(Montage == Anim->Skill_R_Montage) OnSkill_R_MontageEnded();
	
}

void AArcanePunkCharacter::OnAttack_A_MontageEnded()
{
	bAttack_A = false;
	ComboAttackEnd();
	SetActorRotation(BeforeRotation);
	bCanMove = true;
}

void AArcanePunkCharacter::OnAttack_B_MontageEnded()
{
	bAttack_B = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AArcanePunkCharacter::OnSkill_Q_MontageEnded()
{
	bSkill_Q = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AArcanePunkCharacter::OnSkill_E_MontageEnded()
{
	bSkill_E = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AArcanePunkCharacter::OnSkill_R_MontageEnded()
{
	bSkill_R = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AArcanePunkCharacter::MarkErase()
{
	bMarking = false;
	GetWorldTimerManager().ClearTimer(MarkTimerHandle);
}

bool AArcanePunkCharacter::PMCheck(FHitResult &HitResult, FVector OverlapStart, FVector OverlapEnd)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;
	 
    return GetWorld()->LineTraceSingleByChannel(HitResult, OverlapStart, OverlapEnd, ECC_Visibility, Params);
}

void AArcanePunkCharacter::AttackMoving(float DeltaTime)
{
	float Speed = 0.0f;
	int32 Section = Anim->AttackSection;
	if (Section == 1) Speed = Attack1_MoveSpeed;
	else if(Section == 2) Speed = Attack2_MoveSpeed;
	else if(Section == 3) Speed = Attack3_MoveSpeed;

	FHitResult HitResult;
	FVector OverlapStart = GetActorLocation() + GetActorForwardVector() * 34.0f;
	FVector OverlapEnd = OverlapStart + GetActorForwardVector()*Speed * DeltaTime;

	if(!PMCheck(HitResult, OverlapStart, OverlapEnd))
	{
		UE_LOG(LogTemp, Display, TEXT("Your a"));
		FVector DashVector = GetActorLocation() + GetActorForwardVector()*Speed * DeltaTime;
		SetActorLocation(DashVector);
	} 
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Your b"));
	}
}

float AArcanePunkCharacter::DamageMath(float Damage)
{
    return Damage * Defense_constant * (1/(Defense_constant + MyPlayerStatus.DEF));
}

void AArcanePunkCharacter::InitPlayerStatus()
{
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
