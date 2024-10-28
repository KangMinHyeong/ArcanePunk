// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FSkillData.generated.h"

/**
 * 
 */

class UNiagaraSystem;

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Projectile = 0,
};

USTRUCT(BlueprintType)
struct ARCANEPUNK_API FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> SkillEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UShapeComponent> CollisionShapeClass;
	
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
	FName SocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpecialEffect;
};
