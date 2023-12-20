// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "Enemy_DropEquipMent.generated.h"

class UAPItemBase;
class UDataTable;

UCLASS()
class ARCANEPUNK_API AEnemy_DropEquipMent : public AEnemy_DropBase
{
	GENERATED_BODY()
public:
	AEnemy_DropEquipMent();

	virtual void DropOverlap(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void InitializePickup(const TSubclassOf<UAPItemBase> BaseClass, const int32 Quantity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* DropMovement;

	UPROPERTY(EditAnywhere)
	float DropSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UAPItemBase* ItemReference;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;
};
