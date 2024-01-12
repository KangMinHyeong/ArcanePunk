// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "Enemy_DropBase.generated.h"

UCLASS()
class ARCANEPUNK_API AEnemy_DropBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_DropBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnMovement();

	virtual void InitializePickup(const TSubclassOf<UAPItemBase> BaseClass, const int32 Quantity); 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void DropOverlap(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DropMesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent* DropTrigger;

	UPROPERTY(EditAnywhere)
	FVector SpawnImpulse = FVector(200.0f,200.0f,500.0f);

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* DropMovement;

	UPROPERTY(EditAnywhere)
	float DropSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float MaxAngularVelocity = 180.0f;

	UPROPERTY(EditAnywhere)
	float GravityAccelerate = 100.0f;

	// Inventory 관련 변수
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UAPItemBase* ItemReference;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;
	//
};
