// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	UPROPERTY()
	TArray<UAPItemBase*> ItemsInPackage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPDropPackageUI> PackageUIClass;
};
