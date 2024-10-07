// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "Imitator.generated.h"

class UBoxComponent;
class USkeletalMeshComponent;
class UUltSkillNumber_20;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AImitator : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AImitator();

protected:
	virtual void BeginPlay() override;	
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	virtual void DestroySKill() override;
private:
	UFUNCTION()
	void OnOverlaping(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult); 

	void CollisionEnableStart();

protected:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ImitatorMesh;	

	UPROPERTY(EditAnywhere)
	UBoxComponent* ImitatorTrigger;	

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ImitatorEffect;	

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ImitatorSpawnEffect;

	uint8 CopySkillNum = 0;

	TWeakObjectPtr<UUltSkillNumber_20> UltSkillNumber_20;

	UPROPERTY(EditAnywhere)
	FRotator CustomRot;

	FRotator CurrentRot;
};
