// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "Engine/DataTable.h"
#include "Skill/SkillDataStructs/FSkillActorData.h"
#include "FProjectileSkillActorData.generated.h"

class AAPSkillRange;
class UNiagaraSystem;
class UTexture2D;

USTRUCT(BlueprintType)
struct ARCANEPUNK_API FProjectileSkillActorData : public FSkillActorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterState CrowdControl  = ECharacterState::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> SkillEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> SkillOffEffect; // 사라지거나 파괴될 때 스폰되는 이펙트
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UShapeComponent> CollisionShapeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAPSkillRange> SkillTargetRangeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAPSkillRange> SkillGroundRangeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> InstallActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SkillImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CollisionSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillSoundPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCharging = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CC_Duration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttachedEffect = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MPConsumption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 PenetrateCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpecialEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SkillAction;
};