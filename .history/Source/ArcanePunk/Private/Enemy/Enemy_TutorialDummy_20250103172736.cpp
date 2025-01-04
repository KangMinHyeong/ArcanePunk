
#include "Enemy/Enemy_TutorialDummy.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

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
	DamageApplied = DamageMath(DamageApplied);	
	SpawnDamageText(EventInstigator, DamageApplied, DamageTextAddLocation);

	HP = FMath::Clamp<float>(HP - DamageApplied, 0.0f, TotalStatus.StatusData.MaxHP);
	UE_LOG(LogTemp, Display, TEXT("더미 HP : %f"), HP);
		
	if(IsDead())
	{
		// UnPossess
		SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
		
		// Deactivate Effect
		// StunEffectComp->DeactivateImmediate();

		// Dead Motion
		// EnemyDeadMotion();
		
		return DamageApplied;
	}
	else 
	{
		// EnemyAnim->PlayHit_Montage(); // 후에 추가
	}
	
	AAPCharacterBase::TakeDamage(DamageApplied, DamageEvent, EventInstigator, DamageCauser);
    return DamageApplied;
}

void AEnemy_TutorialDummy::SpawnTutorialDummy()
{
}