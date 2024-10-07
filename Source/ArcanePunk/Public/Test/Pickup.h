// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

class UDataTable;
class UAPItemBase;

UCLASS()
class ARCANEPUNK_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();


	void InitializePickup(const TSubclassOf<UAPItemBase> BaseClass, const int32 Quantity);

	void InitializeDrop(UAPItemBase* ItemToDrop, const int32 Quantity);

	FORCEINLINE UAPItemBase* GetItemData() { return ItemReference; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UAPItemBase* ItemReference;

	// UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Item Reference")
	// FInteractableData InstanceInteractableData;

	// need to assign after create
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	// for interaction

	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

	void UpdateInteractableData();

	void TakePickup(const AArcanePunkCharacter* PlayerCharacter);

	// change property event where the UE5 Editor
// #if WITH_EDITOR
// 	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
// #endif

};