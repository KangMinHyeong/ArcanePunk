// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneTent.generated.h"

class USphereComponent;
class UDecalComponent;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AArcaneTent : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneTent();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;	
	
	virtual void DestroySKill() override;	

private:
	UFUNCTION()
  	void OnBeginHiding(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndHiding(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* TentTrigger;	

	UPROPERTY(EditAnywhere)
	UDecalComponent* TentDecal;		

	UPROPERTY()
	AArcanePunkCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	float ReturnToHideTime = 0.5f;
};
