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

	virtual void DropOverlap(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void AddItem(FName ItemID);
	void AddEnhance(uint8 EnhanceCategoryNum);

private:
	UPROPERTY()
	TArray<UAPItemBase*> ItemsInPackage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPDropPackageUI> PackageUIClass;

	UPROPERTY()
	EEnhanceCategory EnhanceCategory = EEnhanceCategory::None;

	UPROPERTY()
	EEnHanceType EnHanceType = EEnHanceType::Silver;

	UPROPERTY(EditAnywhere, Category = "EnHanceType Percent")
	TArray<float> EnHanceTypePercent; // 0.0f ~ 100.0f percent;
};
