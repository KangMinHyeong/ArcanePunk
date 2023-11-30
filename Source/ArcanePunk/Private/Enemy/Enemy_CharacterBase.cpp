// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy_CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "DamageText/DamageText.h"

// Sets default values
AEnemy_CharacterBase::AEnemy_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(),FName("HandWeapon"));

}

// Called when the game starts or when spawned
void AEnemy_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

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

float AEnemy_CharacterBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageApplied = FMath::Min(HP, DamageApplied);

	
	HP = HP - DamageMath(DamageApplied);
	UE_LOG(LogTemp, Display, TEXT("Monster HP : %f"), HP);
	//GetWorldTimerManager().SetTimer(HitTimerHandle, this, &ABossMonster_Stage1::CanBeDamagedInit, bGodModeTime, false);
	SpawnDamageText();

	if(IsDead())
	{
	// 	UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
	// 	bDead = true;

		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABossMonster_Stage1::Destoryed, DeathLoadingTime, false);

	}

    return DamageApplied;
}

// void AEnemy_CharacterBase::Idle()
// {
// 	if(!Anim) return;
// 	if(bIdle) return;
// 	Anim->PlayIdle();
// 	bIdle = true;
// }

// void AEnemy_CharacterBase::Run()
// {
// 	if(!Anim) return;
// 	Anim->PlayRun();
// }

bool AEnemy_CharacterBase::IsDead()
{
    return HP<=0;
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

void AEnemy_CharacterBase::SpawnDamageText()
{
	ADamageText* DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if(!DamageText) return;

	DamageText->SetOwner(this);
}