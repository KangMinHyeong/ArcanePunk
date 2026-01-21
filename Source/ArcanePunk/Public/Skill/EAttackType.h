#pragma once

#include "CoreMinimal.h"
#include "EAttackType.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Physical    UMETA(DisplayName = "Physical"),
	Arcane      UMETA(DisplayName = "Arcane")
};