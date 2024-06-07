
#include "Enemy/Enemy_CharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/AIController/EnemyBaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DamageText/DamageText.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APTakeDamageComponent.h"
#include "Components/WidgetComponent.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Enemy/Drop/Enemy_DropPackage.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "UserInterface/APHUD.h"
#include "GameInstance/APGameInstance.h"

AEnemy_CharacterBase::AEnemy_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComp"));

	Weapon->SetupAttachment(GetMesh(),FName("HandWeapon"));

	TeleportMark = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportMark"));
	TeleportMark->SetupAttachment(GetMesh());
	TeleportMark->Deactivate();

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemy_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerTotalStatus = MyPlayerTotalStatus_Origin;
	SetActorTickEnabled(false);
	InitMonster();	
	BindMontageEnded();
	DefaultSlip = GetCharacterMovement()->BrakingFrictionFactor;
	DefaultMaterial = GetMesh()->GetMaterial(0);
	GetCharacterMovement()->MaxWalkSpeed = MyPlayerTotalStatus.PlayerDynamicData.MoveSpeed;
	
	OnCrowdControlCheck.AddUObject(this, &AEnemy_CharacterBase::CrowdControlCheck);
}

void AEnemy_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy_CharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEnemy_CharacterBase::CrowdControlCheck()
{
	if(StopState.IsEmpty())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

void AEnemy_CharacterBase::TeleportMarkActivate(float Time, AActor* MarkOwner, USkillNumberBase* SkillComp)
{
	MarkActor = MarkOwner;
	SkillComponent = SkillComp;
	TeleportMark->Activate();
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AEnemy_CharacterBase::TeleportMarkDeactivate);
	GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, TimerDelegate, Time, false);
}

void AEnemy_CharacterBase::TeleportMarkDeactivate()
{
	TeleportMark->DeactivateImmediate();
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(MarkActor); if(!OwnerCharacter.IsValid()) return;

	if(SkillComponent.IsValid())
	{
		SkillComponent->MarkErase();
	} 
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
}

float AEnemy_CharacterBase::GetForward()
{
    return MonsterIsForward;
}

float AEnemy_CharacterBase::GetRight()
{
    return MonsterIsRight;
}

bool AEnemy_CharacterBase::AttackPushBack(FVector NewLocation)
{
	if(!IsAttackPush) return false;
	else
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0;
		SetActorLocation(GetActorLocation() + NewLocation);
	}
	return true;
}

float AEnemy_CharacterBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(IsDead()) return 0.0f;
	float HP = MyPlayerTotalStatus.PlayerDynamicData.HP;
	DamageApplied = DamageApplied * DamageMultiple;
	DamageApplied = FMath::Min(HP, DamageApplied);
	
	HP = HP - DamageMath(DamageApplied);
	MyPlayerTotalStatus.PlayerDynamicData.HP = HP;
	UE_LOG(LogTemp, Display, TEXT("Monster HP : %f"), HP);
	OnEnemyHPChanged.Broadcast();
	//GetWorldTimerManager().SetTimer(HitTimerHandle, this, &ABossMonster_Stage1::CanBeDamagedInit, bGodModeTime, false);
	SpawnDamageText(EventInstigator, DamageAmount, DamageTextAddLocation);

	if(IsDead())
	{
	// 	UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TeleportMarkDeactivate();
		CheckAllEnemyKilled();
		StunEffectComp->DeactivateImmediate();
		TeleportMark->DeactivateImmediate();
	 	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemy_CharacterBase::EnemyDestroyed, DeathLoadingTime, false);
	}
	else
	{
		bHitting = true;
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AEnemy_CharacterBase::ResetHitStiffness, HitStiffnessTime, false);
	}

    return DamageApplied;
}

bool AEnemy_CharacterBase::IsHardCC()
{
	if(CurrentState == ECharacterState::KnockBack) {return true;}
	else if(CurrentState == ECharacterState::Stun) {return true;}
	else if(CurrentState == ECharacterState::Sleep) {return true;}
	return false;
}

AActor* AEnemy_CharacterBase::IsAggro()
{
	TArray<AActor*> Actors; float Dist = Distance_Limit;
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Aggro"), Actors);
	AActor* AggroActor = UGameplayStatics::FindNearestActor(GetActorLocation(), Actors, Dist);
	if(AggroActor)
	{
		if(Dist <= Distance_Limit)
		{
			return AggroActor;
		}
		else
		{
			return nullptr;
		}
	}
	return nullptr;
}

float AEnemy_CharacterBase::DamageMath(float Damage)
{
    return Damage * Defense_constant * (1/(Defense_constant + MyPlayerTotalStatus.PlayerDynamicData.DEF));
}

bool AEnemy_CharacterBase::AttackTrace(FHitResult &HitResult, FVector &HitVector, bool Custom, float Radius, FVector CustomStart, FVector CustomEnd)
{
    FRotator Rotation = GetActorRotation();

	FVector Start = CustomStart;
	if(!Custom) Start = GetActorLocation();

	FVector End = CustomEnd + FVector::UpVector* 25.0f ;
	if(!Custom) End = GetActorLocation() + Rotation.Vector() * Monster_AttackRange + FVector::UpVector* 25.0f; // 캐릭터와 몬스터의 높이차가 심하면 + FVector::UpVector* MonsterHigh

	// 같은 아군은 타격 판정이 안되게 하는 코드
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);

	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }    
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Ground"), Actors);
	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }   
	
	HitVector = -Rotation.Vector();

	//DrawDebugSphere(GetWorld(), End, AttackRadius, 10, FColor::Green, false, 5);
	float FinalRadius = Radius;
	if(!Custom) FinalRadius = Monster_AttackRadius;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(FinalRadius);

	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);
}


void AEnemy_CharacterBase::NormalAttack()
{
	float Damage = MyPlayerTotalStatus.PlayerDynamicData.ATK * CriticalCalculate();
	FHitResult HitResult;
	FVector HitVector;
	if(AttackTrace(HitResult, HitVector))
	{
		if(AActor* Actor = HitResult.GetActor())
		{
			FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
			AController* MyController = Cast<AController>(GetController());
			if(MyController == nullptr) return;
			DistinctHitPoint(HitResult.Location, Actor);
			Actor->TakeDamage(Damage, myDamageEvent, MyController, this);
			if(bKnockBackAttack) OnPlayerKnockBack(Actor, KnockBackDist, KnockBackTime);
		}
	}
}

void AEnemy_CharacterBase::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
}

void AEnemy_CharacterBase::ResetHitStiffness()
{
	bHitting = false;
	GetCharacterMovement()->BrakingFrictionFactor = DefaultSlip;
	GetMesh()->SetMaterial(0,DefaultMaterial);
	GetWorldTimerManager().ClearTimer(HitTimerHandle);
}

void AEnemy_CharacterBase::SpawnDamageText(AController* EventInstigator, float Damage, FVector AddLocation)
{
	ADamageText* DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass, GetActorLocation() + AddLocation, FRotator(0.0f, 180.0f, 0.0f));
	if(!DamageText) return;

	bool Check = false; 
	auto Character = Cast<AArcanePunkCharacter>(EventInstigator->GetPawn()); 
	if(Character) Check = Character->IsCriticalAttack();

	DamageText->SetOwner(this);
	DamageText->SetDamageText(Damage, Check);
}

void AEnemy_CharacterBase::InitMonster()
{
	MyPlayerTotalStatus.PlayerDynamicData.HP = MyPlayerTotalStatus.PlayerDynamicData.MaxHP;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetWorldTimerManager().SetTimer(StopTimerHandle, this, &AEnemy_CharacterBase::StopClear, 1.1f, false);
}

void AEnemy_CharacterBase::StopClear()
{
	GetWorldTimerManager().ClearTimer(StopTimerHandle);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AEnemy_CharacterBase::DropItemActor() 
{
	auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;
	auto PackageDrops = GI->GetPackageDropMap();

	for(TPair<FName, float>& DropClass : PackageDrops)
	{
		float DropPercent = FMath::RandRange(0.0f,  100.0f);
		if(DropClass.Key == "Enhance")
		{
			if(DropPercent <= DropClass.Value)
			{
				if(DropPackage.IsValid())
				{
					DropPackage->AddEnhance();
				}
				else
				{
					DropPackage = GetWorld()->SpawnActor<AEnemy_DropPackage>(GI->GetDropPackageClass(), GetActorLocation() + GetActorUpVector()*GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetActorRotation());
					DropPackage->AddEnhance();
				}	
			}
			continue;
		}

		if(DropPercent <= DropClass.Value)
		{
			if(DropPackage.IsValid())
			{
				DropPackage->AddItem(DropClass.Key);
			}
			else
			{
				DropPackage = GetWorld()->SpawnActor<AEnemy_DropPackage>(GI->GetDropPackageClass(), GetActorLocation() + GetActorUpVector()*GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetActorRotation());
				DropPackage->AddItem(DropClass.Key);
			}	
		}
	}
	if(DropPackage.IsValid()) DropPackage->SetDropOverlap(true);
}

void AEnemy_CharacterBase::EnemyDestroyed()
{
	if(OnDrop) DropItemActor();

	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
	Destroy();
}

void AEnemy_CharacterBase::CheckAllEnemyKilled()
{
	auto GameMode = Cast<AAPGameModeBattleStage>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!GameMode) return;
	GameMode->MonsterKilled();
}

void AEnemy_CharacterBase::DistinctHitPoint(FVector ImpactPoint, AActor *HitActor)
{
	FVector HitPoint = ImpactPoint - HitActor->GetActorLocation();
	FVector HitActorForwardVec = HitActor->GetActorForwardVector(); 
	FVector HitActorRightVec = HitActor->GetActorRightVector(); 

	float Forward = (HitActorForwardVec.X * HitPoint.X) + (HitActorForwardVec.Y * HitPoint.Y); // 앞 뒤 Hit 판별
	float Right = (HitActorRightVec.X * HitPoint.X) + (HitActorRightVec.Y * HitPoint.Y); // 좌 우 Hit 판별

	auto Character = Cast<AArcanePunkCharacter>(HitActor); if(!Character) return;
	if(!Character->IsBlockMode()) {Character->GetTakeDamageComponent()->SetHitPoint(Forward, Right); Character->GetTakeDamageComponent()->SetHitEffect(ImpactPoint);}
}

void AEnemy_CharacterBase::SetHitPoint(float Forward, float Right)
{
	MonsterIsForward = Forward;
	MonsterIsRight = Right;
}

void AEnemy_CharacterBase::SetDamageMultiple(float Multiple, float Time)
{
	DamageMultiple = DamageMultiple * Multiple;
	FTimerHandle Timer;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AEnemy_CharacterBase::MultipleEnd, &Timer, Multiple);
	GetWorld()->GetTimerManager().SetTimer(Timer, TimerDelegate, Time, false);
}

void AEnemy_CharacterBase::MultipleEnd(FTimerHandle* TimerHandle, float Multiple)
{
	DamageMultiple = DamageMultiple / Multiple;
	GetWorld()->GetTimerManager().ClearTimer(*TimerHandle);
}

void AEnemy_CharacterBase::SetHitEffect(FVector HitLocation)
{
	if(MonsterIsForward > 0)
	{
		if(MonsterIsRight > 0)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect_R, GetMesh(), TEXT("HitLocation"), HitLocation, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
			// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect_R, HitLocation, FRotator::ZeroRotator);
		}
		else
		{
			// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect_L, HitLocation, FRotator::ZeroRotator);
			UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect_L, GetMesh(), TEXT("HitLocation"), HitLocation, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
		}
	}
	else
	{
		// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect_B, HitLocation, FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect_B, GetMesh(), TEXT("HitLocation"), HitLocation, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
	}
}

void AEnemy_CharacterBase::BindMontageEnded()
{
	EnemyAnim = Cast<UAP_EnemyBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if(EnemyAnim.IsValid()) EnemyAnim->OnMontageEnded.AddDynamic(this, &AEnemy_CharacterBase::EnemyMontageEnded);
}

void AEnemy_CharacterBase::EnemyMontageEnded(UAnimMontage *Montage, bool bInterrupted)
{
    if(!EnemyAnim.IsValid()) return;
	
	if(Montage == EnemyAnim->NormalAttack_Montage) OnNormalAttack_MontageEnded();
}

void AEnemy_CharacterBase::OnNormalAttack_MontageEnded()
{
}

void AEnemy_CharacterBase::OnPlayerKnockBack(AActor* Actor, float Dist, float Time)
{
	auto Character = Cast<AAPCharacterBase>(Actor); if(!Character) return;
	if(!Character->IsBlockMode()) Character->GetCrowdControlComp()->KnockBackState(GetActorLocation(), Dist, Time);
}

void AEnemy_CharacterBase::OnPlayerStun(AActor *Actor, float Time)
{
	auto Character = Cast<AAPCharacterBase>(Actor); if(!Character) return;
	if(!Character->IsBlockMode())  Character->GetCrowdControlComp()->StunState(Time);
}

void AEnemy_CharacterBase::RotateTowardsTarget(AActor *TargetActor, float Speed)
{
	FVector Loc = TargetActor->GetActorLocation() - GetMesh()->GetComponentLocation();
	Loc.Z = 0.0f; FRotator Rotation = FRotationMatrix::MakeFromX(Loc).Rotator();

	if(Speed < 0.0f) {MoveComponent->SetAttackRotation(Rotation);}
	else {MoveComponent->SetAttackRotation(Rotation, Speed);}
   	
}