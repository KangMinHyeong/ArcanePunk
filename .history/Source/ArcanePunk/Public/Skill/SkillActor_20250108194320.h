// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneRain.h"
#include "FSkillData.h"
#include "GameFramework/Actor.h"
#include "SkillActor.generated.h"

class AAPSkillRange;
class ASkillController;

UCLASS()
class ARCANEPUNK_API ASkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillActor();
	FORCEINLINE FName GetSkillName() const {return SkillName;};
	FORCEINLINE TSubclassOf<AAPSkillRange> GetSkillTargetRangeClass() const {return SkillTargetRangeClass;};
	FORCEINLINE TSubclassOf<AAPSkillRange> GetSkillGroundRangeClass() const {return SkillGroundRangeClass;};
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
	UFUNCTION()
	void ChargingSkill();
	virtual void EnhanceSkill(int32 EnhanceNumber);
	virtual void InitSkill(FName SkillName, AArcanePunkCharacter* OwnerCharacter, ASkillController* OwnerController);
	void PlaySkillAction(ESkillKey SkillKey);

	UFUNCTION()
	void OnOverlap_Projectile(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
		
private:
	void SetActive(bool Active);
	void DrawDebugForShapeComponent() const;
	void DragSkill(float DeltaTime);
	void ChargindCheck(float DeltaTime);
	void StartCoolDown();

	float CalculateDamage();

private:
	AArcanePunkCharacter* OwnerCharacter;
	ASkillController* OwnerController;

private:
	FName SkillName;
	ESkillType SkillType;
	ECharacterState CrowdControl;
	float LaunchAngle;
	float Range;
	float Damage;
	float MaxCharging;
	float CoolTime;
	float CC_Duration;
	uint8 MPConsumption;
	uint8 PenetrateCount;
	bool bDrag;
	bool bAttachedEffect;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* SkillEffectComponent;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SkillOffEffect;
	UPROPERTY(EditAnywhere)
	UShapeComponent* CollisionShape;
	UPROPERTY()
	TSubclassOf<AAPSkillRange> SkillTargetRangeClass;
	UPROPERTY()
	TSubclassOf<AAPSkillRange> SkillGroundRangeClass;
	FName SocketName;
	UAnimMontage* SkillAction = nullptr;

	ESkillKey CurrentSkillKey;

	bool bSkilling = false;
	bool bCharging = false;
	uint8 CurrentPenetrateCount = 0;

	TSet<AActor*> CheckSet;

	float DamageCoefficient = 1.0f; 
	float ChargingCoefficient = 1.0f;
	TMap<int32, uint8> EnhanceNestingData;
	float DragSpeed;

	UPROPERTY()
	UNiagaraComponent* ChargingEffect;

private:
	// 움직임 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	FTimerHandle DestroyTimerHandle;

public:
	float RangeCoefficient = 1.0f; // Test용
};
