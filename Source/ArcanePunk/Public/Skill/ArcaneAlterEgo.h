
#pragma once

#include "CoreMinimal.h"
#include "GameInstance/APGameInstance.h"
#include "Components/SkillActor/APSkillType.h"
#include "Components/SkillActor/APSkillAbility.h"
#include "Interfaces/SkillInterface.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APHitPointComponent.h"
#include "Character/APCharacterBase.h"
#include "ArcaneAlterEgo.generated.h"

#define Defense_constant 1000

DECLARE_MULTICAST_DELEGATE(FOnEgoHPChanged);

class UAPHPWidgetComponent;
class UNiagaraSystem;
class AArcanePunkCharacter;
class UWidgetComponent;

UCLASS()
class ARCANEPUNK_API AArcaneAlterEgo : public AAPCharacterBase , public ISkillInterface
{
	GENERATED_BODY()

public:
	AArcaneAlterEgo();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

private:
	UFUNCTION()
	void MoveToMouseClick(FVector ClickPoint);
	// UFUNCTION()
	// void CopyAttack(uint8 ComboStack);

	void EndHitting();

	void StunExplosion();

protected:
	UPROPERTY(EditAnywhere, Category = "Status")
	UWidgetComponent* HealthWidgetComp;

	TWeakObjectPtr<UAPHPWidgetComponent> HPUI;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	FSkillAbilityNestingData SkillAbilityData;
	FSkillAbilityRowNameData * RowDataTable;

	FTimerHandle HittingTimerHandle;
	FTimerHandle DestroyTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Hit")
	float DestroyTime = 20.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 150.0f;

	UPROPERTY(EditAnywhere)
	float ExplosionCoefficient = 2.0f;

	UPROPERTY(EditAnywhere)
	float StunTime = 3.0f;

	UPROPERTY(EditAnywhere)
	FVector ExplosionScale = FVector(1.5f, 1.5f, 1.5f);

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DamagedEffect;

	UPROPERTY(EditAnywhere)
	FVector DamagedScale = FVector(1.0f, 1.0f, 1.0f);

public:
	FOnEgoHPChanged OnEgoHPChanged;
};
