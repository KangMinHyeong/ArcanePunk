// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APSkillType.generated.h"

class UProjectileMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillType : public UActorComponent
{
	GENERATED_BODY()
public:	
	UAPSkillType();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetSkillType(uint8 SkillType,  bool& bStun, UPrimitiveComponent* TriggerComp = nullptr, UProjectileMovementComponent* ProjectileMovementComponent = nullptr);

private:
	void SetOwnerScale();
	void SetProjectileMove(UPrimitiveComponent* TriggerComp = nullptr, UProjectileMovementComponent* ProjectileMovementComponent = nullptr);
	void SetOwnerHoming(AActor* OverlapActor = nullptr);

	void Accelrating();
	UFUNCTION()
	void HomingEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

private:
	UPROPERTY()
	UProjectileMovementComponent* OwnerProjectileMoveComp;
	
	UProjectileMovementComponent* OriginProjectileMoveComp;

	float OriginProjectileSpeed = 0.0f;

	bool bTriggerOn = false;
	bool Init = true;
	
	UPROPERTY()
	TArray<AActor*> Actors;
	
	UPROPERTY()
	AActor* TraceActor = nullptr;

	FTimerHandle AccelerateTimerHandle;

	float TraceSpeed = 1000.0f;


};
