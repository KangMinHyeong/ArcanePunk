
#include "Enemy/Enemy_TutorialDummy.h"

AEnemy_TutorialDummy::AEnemy_TutorialDummy()
{
}

void AEnemy_TutorialDummy::BeginPlay()
{
    Super::BeginPlay();

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
}

float AEnemy_TutorialDummy::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
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
		TeleportMarkDeactivate();
		StunEffectComp->DeactivateImmediate();
		StunEffectComp->DestroyComponent();
		TeleportMark->DeactivateImmediate();

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
		// EnemyAnim->PlayHit_Montage(); // 후에 추가
	}
	
	AAPCharacterBase::TakeDamage(DamageApplied, DamageEvent, EventInstigator, DamageCauser);
    return DamageApplied;
}