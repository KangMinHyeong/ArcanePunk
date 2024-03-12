// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "Enemy_DropUnlock.generated.h"

UCLASS()
class ARCANEPUNK_API AEnemy_DropUnlock : public AEnemy_DropBase
{
	GENERATED_BODY()
public:
	AEnemy_DropUnlock();
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitEnHanceType();

private:
	UPROPERTY(EditAnywhere)
	ESkillKey Unlock_SkillAbility = ESkillKey::Q;
	
	UPROPERTY()
	EEnHanceType EnHanceType = EEnHanceType::Silver;

	UPROPERTY(EditAnywhere, Category = "EnHanceTypeMaterial")
	TArray<UMaterial*> EnHanceTypeMaterial; 

	UPROPERTY(EditAnywhere, Category = "EnHanceTypeMaterial")
	TArray<float> EnHanceTypePercent; // 0.0f ~ 100.0f percent;

};
