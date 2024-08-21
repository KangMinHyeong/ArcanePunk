// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APEnemyAttackRange.generated.h"

class UDecalComponent;

UCLASS()
class ARCANEPUNK_API AAPEnemyAttackRange : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPEnemyAttackRange();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetDecalSize(float X, float Y, float AttackRangeTime, bool bSquare = true);

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* RangeRoot;

	UPROPERTY(EditAnywhere)
	UDecalComponent* BaseDecal;

	UPROPERTY(EditAnywhere)
	UDecalComponent* PatternDecal;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> BaseMaterials;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> PatternMaterials;

	UPROPERTY(EditAnywhere)
	float CurrentMove = -1.0f;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 0.1f;

	FName ScalarName;
};
