// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APTakeDamageComponent.generated.h"

#define Defense_constant 1000

class UNiagaraSystem;
class AArcanePunkCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPTakeDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPTakeDamageComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitTakeDamageComp();
	
	void DamageCalculation(float &DamageApplied);
	void ReflectDamage(float & DamageApplied, AActor* DamageCauser);
	
	void SetHitPoint(float Forward, float Right);

	UFUNCTION(BlueprintPure)
	float GetForward();

	UFUNCTION(BlueprintPure)
	float GetRight();

	void SetHitEffect(FVector HitLocation); 

private:
	void OnHitting();
	bool CheckingDamaged();

private:
	FTimerHandle HittingTimerHandle;

	float PlayerIsForward = 0.0f;
	float PlayerIsRight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* HitEffect_L;

	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* HitEffect_R;

	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* HitEffect_B;

	UPROPERTY(EditAnywhere, Category = "Hit")
	float HitMotionTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Hit")
	UMaterialInterface* HitMaterial;

	UPROPERTY(EditAnywhere, Category = "Death")
	float DeathTime = 3.0f;

	uint8 DamagedNumber = 0;

	UPROPERTY(EditAnywhere, Category = "Hit")
	TSubclassOf<AActor> DamagedShieldClass;

	TWeakObjectPtr<AActor> DamagedShield;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
};
