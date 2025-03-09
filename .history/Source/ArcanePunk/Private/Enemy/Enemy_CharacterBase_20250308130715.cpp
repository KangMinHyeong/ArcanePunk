
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
#include "GameElements/DamageText/DamageText.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "UserInterface/HUD/APHUD.h"
#include "GameInstance/APGameInstance.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "NavigationSystem.h"
#include "Components/Common/APManaDropComponent.h"
#include "Enemy/SkillActor/APEnemyAttackRange.h"
#include "GameElements/Trigger/BattleSection/APBattleSectionBase.h"
#include "UserInterface/HUD/APHUD.h"
#include "Components/WidgetComponent.h"
#include "UserInterface/Common/WidgetComponent/APEnemyHPWidgetComponent.h"
#include "UserInterface/Common/WidgetComponent/APEnemyTextWidgetComponent.h"

AEnemy_CharacterBase::AEnemy_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	ManaDropComp = CreateDefaultSubobject<UAPManaDropComponent>(TEXT("ManaDropComp"));

	Weapon->SetupAttachment(GetMesh(),FName("HandWeapon"));

	TeleportMark = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportMark"));
	TeleportMark->SetupAttachment(GetMesh()); TeleportMark->Deactivate();
	
	DamagedMark = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DamagedMark"));
	DamagedMark->SetupAttachment(GetMesh()); DamagedMark->Deactivate();

	TextWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("TextWidgetComp"));
	TextWidgetComp->SetupAttachment(GetCapsuleComponent());
	TextWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TextWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComp"));
	HealthWidgetComp->SetupAttachment(GetCapsuleComponent());
	HealthWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	
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

	TotalStatus = TotalStatus_Origin;
	SetActorTickEnabled(false);
	InitMonster();	
	BindMontageEnded();
	GetCharacterMovement()->MaxWalkSpeed = TotalStatus.StatusData.MoveSpeed;
	Monster_AttackRange_Plus += Monster_AttackRange;

	OnCrowdControlCheck.AddUObject(this, &AEnemy_CharacterBase::CrowdControlCheck);
}

void AEnemy_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead() && bEnemyDeath) DissolveEnemy(DeltaTime);
}

void AEnemy_CharacterBase::DissolveEnemy(float DeltaTime)
{
	Apperence = FMath::FInterpConstantTo(Apperence, 0.0f, DeltaTime, FadeOutSpeed);
	int32 index = 0;
	for (auto Mat : SkinMesh)
	{
		Mat->SetScalarParameterValue(TEXT("Apperence"), Apperence);
		GetMesh()->SetMaterial(index, Mat);
		index++;
	}
	if(Apperence <= 0.2f) EnemyDestroyed();
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
		GetCharacterMovement()->SetMovementMode(Basic_MOVE);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

// void AEnemy_CharacterBase::TeleportMarkActivate(float Time, AActor* MarkOwner, USkillNumberBase* SkillComp)
// {
// 	MarkActor = MarkOwner;
// 	SkillComponent = SkillComp;
// 	TeleportMark->Activate();
// 	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AEnemy_CharacterBase::TeleportMarkDeactivate);
// 	GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, TimerDelegate, Time, false);
// }

// void AEnemy_CharacterBase::TeleportMarkDeactivate()
// {
// 	TeleportMark->DeactivateImmediate();
// 	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter = Cast<AArcanePunkCharacter>(MarkActor); if(!OwnerCharacter.IsValid()) return;

// 	if(SkillComponent.IsValid())
// 	{
// 		SkillComponent->MarkErase();
// 	} 
// 	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
// }

void AEnemy_CharacterBase::SetOwnerSection(AActor * BattleSection)
{
	OwnerSection = BattleSection;
}

bool AEnemy_CharacterBase::AttackPushBack(AActor* DamagedCauser)
{
	if(!IsAttackPush) return false;
	else
	{
		FVector HitLocation = DamagedCauser->GetActorLocation();
		HitLocation -= GetActorLocation(); HitLocation.Z = 0.0f;
		HitLocation = HitLocation/HitLocation.Size();
		MoveComponent->StartTickMove(-HitPushDist * HitLocation);
	}
	return true;
}

float AEnemy_CharacterBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	if(IsDead()) return 0.0f;
	float DamageApplied = DamageAmount;

	float & HP = TotalStatus.StatusData.HP;
	DamageApplied = DamageMath(DamageApplied * DamageMultiple);	
	SpawnDamageText(EventInstigator, DamageApplied, DamageTextAddLocation);

	if(CheckShieldHP(DamageApplied, DamageEvent)) return 0.0f;
	HP = FMath::Clamp<float>(HP - DamageApplied, 0.0f, TotalStatus.StatusData.MaxHP);
	UE_LOG(LogTemp, Display, TEXT("Monster HP : %f"), HP);
		
	if(IsDead())
	{
		// UnPossess
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		// Deactivate Effect
		StunEffectComp->DeactivateImmediate();
		StunEffectComp->DestroyComponent();

		// Dead Motion
		EnemyDeadMotion();
		
		// Drop
		DropChecking(DamageCauser);

		// Checking Section End
		CheckAllEnemyKilled();

		return DamageApplied;
	}
	else 
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AttackPushBack(DamageCauser);
		if(bNormalMonster) EnemyAnim->PlayHit_Montage();
	}
	
	Super::TakeDamage(DamageApplied, DamageEvent, EventInstigator, DamageCauser);
    return DamageApplied;
}

void AEnemy_CharacterBase::OnHittingEnd()
{
	Super::OnHittingEnd();
	GetCharacterMovement()->SetMovementMode(Basic_MOVE);
}

bool AEnemy_CharacterBase::IsHardCC()
{
	if(IsDead()) return false;
	
	if(CurrentState == ECharacterState::KnockBack) {return true;}
	else if(CurrentState == ECharacterState::Stun) {return true;}
	else if(CurrentState == ECharacterState::Sleep) {return true;}
	else if(CurrentState == ECharacterState::Frozen) {return true;}
	return false;
}

AActor* AEnemy_CharacterBase::IsAggro()
{
	TArray<AActor*> Actors; float Dist = DetectDist;
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Aggro"), Actors);
	AActor* AggroActor = UGameplayStatics::FindNearestActor(GetActorLocation(), Actors, Dist);
	if(AggroActor)
	{
		if(Dist <= DetectDist)
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

FVector AEnemy_CharacterBase::GetPatrolLocation(const FVector & Start)
{
	FVector PatrolLocation;

	FNavLocation NavLoc;
	const UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	bool navResult = false;
	int32 Repeat = 0;
	while(!navResult && Repeat < 20)
	{
		PatrolLocation = Start;
		float X = FMath::RandRange(-1.0f, 1.0f);
		float Y = sqrt(1.0f - X*X);
		int32 minus = FMath::RandRange(0,1); if(minus == 0) Y = -Y;

		PatrolLocation += FVector( PatrolDist * X,  PatrolDist * Y, 0.0f);
		navResult = navSystem->ProjectPointToNavigation(PatrolLocation, NavLoc, FVector(0.0f, 0.0f, 5000.0f));
		Repeat++;
	}

	return PatrolLocation;
}

void AEnemy_CharacterBase::SpawnDetectRender()
{
	TextUI->SetDetectText();
}

bool AEnemy_CharacterBase::AttackTrace(FHitResult &HitResult, FVector &HitVector, float Radius, const FVector & Start, const FVector & End)
{
    FRotator Rotation = GetActorRotation();

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

	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	bool bResult = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);
	
	FVector Center = (Start + End) * 0.5f;
	float HalfHeight = FMath::Max((Start - End).Size() * 0.5f, Radius);
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(End - Start).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;

	if(bDebugDraw)
	{
		DrawDebugCapsule(GetWorld(),
			Center,
			HalfHeight,
			Radius,
			CapsuleRot,
			DrawColor,
			false,
			3.0f,
			0,
			5);
	}

	return bResult;
}

void AEnemy_CharacterBase::NormalAttack()
{
	float Damage = TotalStatus.StatusData.ATK * CriticalCalculate();
	FHitResult HitResult;
	FVector HitVector;
	FVector Start = GetActorLocation();
	if(AttackTrace(HitResult, HitVector, Monster_AttackRadius, Start, Start + GetActorForwardVector() *Monster_AttackRange_Plus))
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

void AEnemy_CharacterBase::SpawnDamageText(AController* EventInstigator, float Damage, const FVector & AddLocation)
{
	if(Damage <= KINDA_SMALL_NUMBER) return;
	bool Check = false; 
	auto Character = Cast<AArcanePunkCharacter>(EventInstigator->GetPawn()); 
	if(Character) Check = Character->IsCriticalAttack();

	TextUI->SetDamageText(Damage, Check);
}

void AEnemy_CharacterBase::InitMonster()
{
	// Status Init
	TotalStatus.StatusData.HP = TotalStatus.StatusData.MaxHP;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Mesh Init
	int32 i = 0;
	for (auto Mat : GetMesh()->GetMaterials())
	{
		auto NewMesh = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mat);
		SkinMesh.Emplace(NewMesh); GetMesh()->SetMaterial(i, NewMesh);
		i++;
	}

	auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return; 
    auto DataTable = DataTableGI->GetDropDataTable()->FindRow<FDropData>(CharacterName, CharacterName.ToString()); 
    if(DataTable) DropData = * DataTable; 
	SetManaDrop();
	SetHPUI();

	TextUI = Cast<UAPEnemyTextWidgetComponent>(TextWidgetComp->GetUserWidgetObject());
	
	bBlockMode = true;
	GetWorldTimerManager().SetTimer(StopTimerHandle, this, &AEnemy_CharacterBase::StopClear, 1.1f, false);
}

void AEnemy_CharacterBase::StopClear()
{
	bBlockMode = false;
	GetWorldTimerManager().ClearTimer(StopTimerHandle);
	GetCharacterMovement()->SetMovementMode(Basic_MOVE);
}

void AEnemy_CharacterBase::SetManaDrop()
{
	ManaDropComp->SetDropPercent(DropData.DropArcaneEnergy_Percent);
	ManaDropComp->SetManaAmount(FMath::RandRange(DropData.DropArcaneEnergy_Min, DropData.DropArcaneEnergy_Max));
}

void AEnemy_CharacterBase::DropChecking(AActor *DamageCauser) 
{
	auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return; 

	float DropPercent = FMath::RandRange(0.0f, 100.0f);
	if(DropPercent < DropData.DropGold_Percent)
	{
		int32 Quantity = FMath::RandRange(DropData.DropGold_Min, DropData.DropGold_Max);
		FVector SpawnLocation = GetActorLocation();
		auto DropGold = GetWorld()->SpawnActor<AEnemy_DropBase>(DataTableGI->GetDropGoldClass(), SpawnLocation, GetActorRotation());
		if(DropGold) DropGold->InitializePickup(DamageCauser, Quantity, true);
	}

	ManaDropComp->SpawnManaDrop(DamageCauser);
}

void AEnemy_CharacterBase::EnemyDeadMotion()
{
	EnemyAnim->PlayDeath_Montage();
	bEnemyDeath = true;
}

void AEnemy_CharacterBase::EnemyDestroyed()
{
	Destroy();
}

void AEnemy_CharacterBase::CheckAllEnemyKilled()
{
	auto GameMode = Cast<AAPGameModeBattleStage>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!GameMode) return;
	GameMode->MonsterKilled(OwnerSection.Get());
}

void AEnemy_CharacterBase::DistinctHitPoint(const FVector & ImpactPoint, AActor *HitActor)
{
	FVector HitPoint = ImpactPoint - HitActor->GetActorLocation();
	FVector HitActorForwardVec = HitActor->GetActorForwardVector(); 
	FVector HitActorRightVec = HitActor->GetActorRightVector(); 

	float Forward = (HitActorForwardVec.X * HitPoint.X) + (HitActorForwardVec.Y * HitPoint.Y); // 앞 뒤 Hit 판별
	float Right = (HitActorRightVec.X * HitPoint.X) + (HitActorRightVec.Y * HitPoint.Y); // 좌 우 Hit 판별

	auto Character = Cast<AArcanePunkCharacter>(HitActor); if(!Character) return;
	if(!Character->IsBlockMode()) {Character->SetHitPoint(Forward, Right); Character->SetHitEffect(ImpactPoint);}
}

void AEnemy_CharacterBase::AddDamageMultiple(float Add, float Time)
{
	DamageMultiple = DamageMultiple + Add;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AEnemy_CharacterBase::SubtractDamageMultiple, Add);
	GetWorld()->GetTimerManager().SetTimer(DamageMultipleTimerHandle, TimerDelegate, Time, false);
}

void AEnemy_CharacterBase::OnAttachingDamagedMark(float Time)
{
	if(DamagedMark) DamagedMark->Activate();
	GetWorldTimerManager().SetTimer(DamagedMarkTimerHandle, this, &AEnemy_CharacterBase::OnDetachingDamagedMark, Time, false);
}

void AEnemy_CharacterBase::SubtractDamageMultiple(float Subtract)
{
	DamageMultiple = DamageMultiple - Subtract;
	GetWorld()->GetTimerManager().ClearTimer(DamageMultipleTimerHandle);
}

void AEnemy_CharacterBase::OnDetachingDamagedMark()
{
	if(DamagedMark) DamagedMark->DeactivateImmediate();
	GetWorldTimerManager().ClearTimer(DamagedMarkTimerHandle);
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
	else if(Montage == EnemyAnim->Death_Montage) OnDeath_MontageEnded();
	else if(Montage == EnemyAnim->Detect_Montage) OnDetect_MontageEnded();
	else if(Montage == EnemyAnim->Hit_Montage) OnHit_MontageEnded();
	else if(Montage == EnemyAnim->Rescue_Montage) bRescue = true;
}

void AEnemy_CharacterBase::OnNormalAttack_MontageEnded()
{
	GetCharacterMovement()->SetMovementMode(Basic_MOVE);
}

void AEnemy_CharacterBase::OnDeath_MontageEnded()
{
	SetActorTickEnabled(true);
}

void AEnemy_CharacterBase::OnDetect_MontageEnded()
{
	GetCharacterMovement()->SetMovementMode(Basic_MOVE);
}

void AEnemy_CharacterBase::OnHit_MontageEnded()
{
	GetCharacterMovement()->SetMovementMode(Basic_MOVE);
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

void AEnemy_CharacterBase::SpawnAttackRange(AActor* Target, float WaitTime)
{
	auto AttackRange = GetWorld()->SpawnActor<AAPEnemyAttackRange>(AttackRangeClass, GetMesh()->GetComponentLocation(), GetActorRotation()); if(!AttackRange) return;
	AttackRange->SetDecalSize(Monster_AttackRadius, Monster_AttackRange_Plus+Monster_AttackRadius, WaitTime);
	// FVector2D(ShootRange.X / 1000.0f, ShootRange.Y / 200.0f)
}

float AEnemy_CharacterBase::GetTargetDist2D(AActor *Target)
{
    FVector TargetLoc = Target->GetActorLocation(); TargetLoc.Z = GetActorLocation().Z;

    return (TargetLoc - GetActorLocation()).Size();
}

void AEnemy_CharacterBase::SetCapsuleOverlap(bool IsOverlap)
{
	if(IsOverlap)
	{
    
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
    	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap); 
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
    	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); 
	}
}

bool AEnemy_CharacterBase::SetHPUI()
{ 
	HPUI = Cast<UAPEnemyHPWidgetComponent>(HealthWidgetComp->GetUserWidgetObject());
	if(HPUI.IsValid()) HPUI->SetEnemy(this);
	
    return true;
}

void AEnemy_CharacterBase::RemoveHPUI()
{
	if(HPUI.IsValid())
	{
		HPUI->ReleaseSlateResources(true);
		HPUI->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AEnemy_CharacterBase::RescueEnemy()
{
}