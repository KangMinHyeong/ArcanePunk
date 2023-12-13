// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy_CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DamageText/DamageText.h"
#include "NiagaraComponent.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "Components/Character/SkillNumber/SkillNumber2.h"
#include "Components/Character/APSkillNumber.h"
#include "Character/ArcanePunkCharacter.h"

// Sets default values
AEnemy_CharacterBase::AEnemy_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(),FName("HandWeapon"));

	TeleportMark = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportMark"));
	TeleportMark->SetupAttachment(GetMesh());
	TeleportMark->Deactivate();
}

// Called when the game starts or when spawned
void AEnemy_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	OnDrop = true;
	DefaultSlip = GetCharacterMovement()->BrakingFrictionFactor;
	DefaultMaterial = GetMesh()->GetMaterial(0);
	Anim = Cast<UAP_EnemyBaseAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AEnemy_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy_CharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AEnemyBaseAIController* MonsterAIController = Cast<AEnemyBaseAIController>(GetController());
	if(!MonsterAIController) return;

	PossessedBy(MonsterAIController);


}

void AEnemy_CharacterBase::TeleportMarkActivate(float Time, AActor* MarkOwner)
{
	MarkActor = MarkOwner;
	TeleportMark->Activate();
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &AEnemy_CharacterBase::TeleportMarkDeactivate, Time, false);
}

void AEnemy_CharacterBase::TeleportMarkDeactivate()
{
	TeleportMark->DeactivateImmediate();
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(MarkActor);
	if(!OwnerCharacter) return;
	UE_LOG(LogTemp, Display, TEXT("Your message"));
	OwnerCharacter->GetAPSkillNumberComponent()->GetSkillNumber2()->MarkErase();
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
}

bool AEnemy_CharacterBase::IsHitting()
{
    return bHitting;
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
	
	DamageApplied = FMath::Min(HP, DamageApplied);
	
	HP = HP - DamageMath(DamageApplied);
	UE_LOG(LogTemp, Display, TEXT("Monster HP : %f"), HP);
	//GetWorldTimerManager().SetTimer(HitTimerHandle, this, &ABossMonster_Stage1::CanBeDamagedInit, bGodModeTime, false);
	SpawnDamageText(DamageAmount);

	if(HitMaterial) GetMesh()->SetMaterial(0, HitMaterial);

	if(IsDead())
	{
	// 	UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
	// 	bDead = true;
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	 	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemy_CharacterBase::EnemyDestroyed, DeathLoadingTime, false);
	}
	else
	{
		bHitting = true;
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AEnemy_CharacterBase::ResetHitStiffness, HitStiffnessTime, false);
	}

    return DamageApplied;
}

bool AEnemy_CharacterBase::IsDead()
{
	if(HP <= 0) bIsDead = true;
	else bIsDead = false;
    return bIsDead;
}

bool AEnemy_CharacterBase::IsNormalAttack()
{
    return bNormalAttack;
}

float AEnemy_CharacterBase::DamageMath(float Damage)
{
    return Damage * Defense_constant * (1/(Defense_constant + Character_Defense));
}

bool AEnemy_CharacterBase::AttackTrace(FHitResult &HitResult, FVector &HitVector)
{
    FRotator Rotation = GetActorRotation();

	FVector End = GetActorLocation() + Rotation.Vector() * Monster_AttackRange + FVector::UpVector* 25.0f; // 캐릭터와 몬스터의 높이차가 심하면 + FVector::UpVector* MonsterHigh

	// 같은 아군은 타격 판정이 안되게 하는 코드
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), Actors);

	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }    
	
	HitVector = -Rotation.Vector();

	//DrawDebugSphere(GetWorld(), End, AttackRadius, 10, FColor::Green, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(Monster_AttackRadius);

	return GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);
}

void AEnemy_CharacterBase::NormalAttack()
{
	bNormalAttack = true;
	GetWorldTimerManager().SetTimer(NormalAttackTimerHandle, this, &AEnemy_CharacterBase::ResetNormalAttack, NormalAttack_CastingTime, false);
	float Damage = Monster_ATK;
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
			Actor->TakeDamage(Damage, myDamageEvent, MyController, this);
		}
	}
}

float AEnemy_CharacterBase::GetDistanceLimit()
{
    return Distance_Limit;
}

void AEnemy_CharacterBase::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
}

void AEnemy_CharacterBase::ResetNormalAttack()
{
	bNormalAttack = false;
	GetWorldTimerManager().ClearTimer(NormalAttackTimerHandle);
}

void AEnemy_CharacterBase::ResetHitStiffness()
{
	bHitting = false;
	GetCharacterMovement()->BrakingFrictionFactor = DefaultSlip;
	GetMesh()->SetMaterial(0,DefaultMaterial);
	GetWorldTimerManager().ClearTimer(HitTimerHandle);
}

void AEnemy_CharacterBase::SpawnDamageText(float Damage)
{
	ADamageText* DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass, GetActorLocation(), FRotator(0.0f, 180.0f, 0.0f));
	if(!DamageText) return;

	DamageText->SetOwner(this);
	DamageText->SetDamageText(Damage);
}

void AEnemy_CharacterBase::DropItemActor() 
{
	float IsSpawn = FMath::RandRange(0.0f, 100.0f);
	float RandAngle = FMath::RandRange(-DropAngleMax,DropAngleMax);
	if(IsSpawn <= DropPercent)
	{
		FRotator DropAngle = DropRot + FRotator(RandAngle, 0, RandAngle);
		auto DropItems = GetWorld()->SpawnActor<AEnemy_DropBase>(DropActorClass, GetActorLocation() + GetActorUpVector()*100.0f, DropAngle);
	}
}

void AEnemy_CharacterBase::EnemyDestroyed()
{
	if(OnDrop) DropItemActor();
	TeleportMarkDeactivate();
	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
	Destroy();
}
