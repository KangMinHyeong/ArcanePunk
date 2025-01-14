// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APInstallBase.generated.h"

class AEnemy_CharacterBase;
class USkillNumberBase;
class USoundBase;

UCLASS()
class ARCANEPUNK_API AAPInstallBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPInstallBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void InitInstall(AActor* SkillActor);

};
