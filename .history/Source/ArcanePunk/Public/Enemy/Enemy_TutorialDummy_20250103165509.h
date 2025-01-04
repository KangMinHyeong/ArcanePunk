// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy_TutorialDummy.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AEnemy_TutorialDummy : public AEnemy_CharacterBase
{
	GENERATED_BODY()

public:
	AEnemy_TutorialDummy();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
};
