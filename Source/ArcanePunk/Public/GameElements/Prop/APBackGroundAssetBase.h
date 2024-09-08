// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APBackGroundAssetBase.generated.h"

UCLASS()
class ARCANEPUNK_API AAPBackGroundAssetBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPBackGroundAssetBase();

	FORCEINLINE bool IsSpawnCompleted() const {return IsSpawn;};
	
protected:
	virtual void BeginPlay() override;

public:	

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BackGroundAsset;
	
	bool IsSpawn = false;
};
