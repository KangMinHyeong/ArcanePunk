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

	virtual void Tick(float DeltaTime) override;

	void AddItem(FName ItemID);
	void AddEnhance();
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual FInteractData GetInteractData() override;
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

protected:
	virtual void BeginPlay() override;
	EEnhanceCategory GetRandCategory();
	int32 CheckGoldAmount();
	
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

	UPROPERTY(EditAnywhere)
	FLinearColor SilverColor;
	UPROPERTY(EditAnywhere)
	FLinearColor GoldColor;
	UPROPERTY(EditAnywhere)
	FLinearColor PlatinumColor;

	bool IsInit = false;
};
