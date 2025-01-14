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

	virtual void DestroySKill();

protected:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* InstallCapsule;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* InstallRoot;

	UPROPERTY(EditAnywhere)
	float HP = 100.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DamageTextClass;

	UPROPERTY(EditAnywhere)
	FVector DeadEffectSize = FVector(0.5f, 0.5f, 0.5f);

	UPROPERTY(EditAnywhere)
	float AttackDistance = 1000.0f;

	TWeakObjectPtr<AActor> Enemy;
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	FTimerHandle DestroyTimerHandle;
};
