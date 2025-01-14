// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APInstallBase.generated.h"

class UCapsuleComponent;
class ASkillActor;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AAPInstallBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPInstallBase();

protected:
	virtual void BeginPlay() override;

	virtual void DestroySKill();
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void InitInstall(ASkillActor* SkillActor);

protected:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* InstallCapsule;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* InstallRoot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DamageTextClass;

	UPROPERTY(EditAnywhere)
	FVector DeadEffectSize = FVector(0.5f, 0.5f, 0.5f);

	UPROPERTY(EditAnywhere)
	float HP = 100.0f;

	UPROPERTY(EditAnywhere)
	float AttackDistance = 1000.0f;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 10.0f;

	TWeakObjectPtr<ASkillActor> OwnerSkillActor;
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	FTimerHandle DestroyTimerHandle;

};
