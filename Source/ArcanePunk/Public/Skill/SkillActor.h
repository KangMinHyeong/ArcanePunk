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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void UseSkill();
	virtual void InitSkill(FName SkillName, AArcanePunkCharacter* OwnerCharacter);
	
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
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* SkillEffectComponent;
	UPROPERTY(EditAnywhere)
	UShapeComponent* CollisionShape;
	FName SocketName;
	UAnimMontage* SkillAction = nullptr;

private:
	// 움직임 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;
};
