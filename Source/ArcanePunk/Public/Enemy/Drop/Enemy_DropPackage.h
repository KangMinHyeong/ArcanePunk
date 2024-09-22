// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "Enemy_DropPackage.generated.h"

class UAPDropPackageUI;

UCLASS()
class ARCANEPUNK_API AEnemy_DropPackage : public AEnemy_DropBase
{
	GENERATED_BODY()
public:
	AEnemy_DropPackage();
	FORCEINLINE FName GetDropID() const {return DropID;};

	virtual void Tick(float DeltaTime) override;

	void AddItem(FName ItemID);
	void AddEnhance();
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual FInteractData GetInteractData() override;
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

	void DropActivate();

protected:
	virtual void BeginPlay() override;
	EEnhanceCategory GetRandCategory();
	int32 CheckGoldAmount();
	void InitHidden(bool bHidden);

private:
	UPROPERTY()
	TArray<UAPItemBase*> ItemsInPackage;

	EEnhanceCategory EnhanceCategory = EEnhanceCategory::None;

	EEnHanceType EnHanceType = EEnHanceType::None;

	UPROPERTY(EditAnywhere, Category = "EnHanceType Percent")
	TArray<float> EnHanceTypePercent; // 0.0f ~ 100.0f percent;

	UPROPERTY(EditAnywhere, Category = "EnHanceCategory Percent")
	TMap<EEnhanceCategory, float> EnHanceCategoryPercent; // 0.0f ~ 100.0f percent;

	FTimerHandle InteractTimerHandle;

	UPROPERTY(EditAnywhere)
	float InteractFrequency = 0.1f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* GroundEffect_Silver;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* GroundEffect_Gold;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* GroundEffect_Platinum;

	UNiagaraSystem* CurrentEffect;

	UPROPERTY(EditAnywhere)
	float EffectScale = 1.5f;

	UPROPERTY(EditAnywhere, Category = "ID")
	FName DropID = "1-1";

	bool IsInit = false;

	UPROPERTY(EditAnywhere)
	bool bInitHidden = true;
};
