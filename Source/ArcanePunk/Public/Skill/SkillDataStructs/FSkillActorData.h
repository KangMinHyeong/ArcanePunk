// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "Engine/DataTable.h"
#include "FSkillActorData.generated.h"

class AAPSkillRange;
class UNiagaraSystem;
class UTexture2D;

USTRUCT(BlueprintType)
struct ARCANEPUNK_API FSkillActorData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
};