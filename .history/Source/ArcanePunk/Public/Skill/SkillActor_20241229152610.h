// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneRain.h"
#include "FSkillData.h"
#include "GameFramework/Actor.h"
#include "SkillActor.generated.h"

UCLASS()
class ARCANEPUNK_API ASkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillActor();
	FORCEINLINE FName GetSkillName() const {return SkillName;};
	FORCEINLINE uint8 GetMPConsumption() const {return MPConsumption;};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	virtual void UseSkill();
	virtual void EnhanceSkill(int32 EnhanceNumber);
	virtual void InitSkill(FName SkillName, AArcanePunkCharacter* OwnerCharacter);
	void PlaySkillAction(ESkillKey SkillKey);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
		
private:
	void SetActive(bool Active);
	void DrawDebugForShapeComponent() const;

private:
	AArcanePunkCharacter* OwnerCharacter;
	
private:
	FName SkillName;
	ESkillType SkillType;
	float LaunchAngle;
	float Range;
	float Damage;
	float CoolTime;
	uint8 MPConsumption;
	uint8 PenetrateCount;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* SkillEffectComponent;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SkillOffEffect;
	UPROPERTY(EditAnywhere)
	UShapeComponent* CollisionShape;
	FName SocketName;
	UAnimMontage* SkillAction = nullptr;

	ESkillKey CurrentSkillKey;

	bool bSkilling = false;

	uint8 CurrentPenetrateCount = 0;

	TSet<AActor*> CheckSet;

	float PlusDamage = 0.0f;
	TMap<int32, uint8> EnhanceNestingData;

private:
	// 움직임 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	FTimerHandle DestroyTimerHandle;
};
