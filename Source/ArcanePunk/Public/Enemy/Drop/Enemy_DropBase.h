// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Components/Character/APInventoryComponent.h"
#include "Components/Common/APInteractionBoxComponent.h"
#include "GameFramework/Actor.h"
#include "Enemy_DropBase.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UBoxComponent;

UCLASS()
class ARCANEPUNK_API AEnemy_DropBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_DropBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnMovement();

public:	
	virtual void Tick(float DeltaTime) override;
	
	void OnDestinatingGround();
	
	virtual void BeginFocus() override;

	virtual void InitializePickup(AActor *DamageCauser, const int32 Quantity, bool RootingImmediate = false); 

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DropMesh;

	UPROPERTY(EditAnywhere)
	UAPInteractionBoxComponent* InteractionTrigger;

	UPROPERTY(EditAnywhere)
	FVector SpawnImpulse = FVector(200.0f,200.0f,500.0f);

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* DropTrailEffect;

	UPROPERTY(EditAnywhere)
	float MaxAngularVelocity = 180.0f;

	UPROPERTY(EditAnywhere)
	float GravityAccelerate = 100.0f;
	float CurrentAccelerate = 0.0f;
	UPROPERTY(EditAnywhere)
	float AccelerateSpeed = 300.0f;

	// Inventory 관련 변수
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UAPItemBase* ItemReference;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditAnywhere, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	// Sound
	UPROPERTY(EditAnywhere)
	USoundBase* DropSound;
	UPROPERTY(EditAnywhere)
	float DropSoundVolume = 1.0f;

	// Drop Effect
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DropEffect;
};
