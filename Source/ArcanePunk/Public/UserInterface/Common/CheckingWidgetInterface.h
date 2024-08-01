// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CheckingWidgetInterface.generated.h"


UENUM(BlueprintType)
enum class ECheckType : uint8 // Q스킬
{
	None 			 = 0 UMETA(DisplayName = "None"),
	Save			 = 1 UMETA(DisplayName = "Check_Save"), // 저장 하시겠습니까?
	Load	 		 = 2 UMETA(DisplayName = "Check_Load"), // 이 파일로 시작하시겠습니까?
	Purchase		 = 3 UMETA(DisplayName = "Check_Purchase"), // 구매하시겠습니까?
	Select			 = 4 UMETA(DisplayName = "Check_Select"), // 선택하시겠습니까?
	Delete			 = 5 UMETA(DisplayName = "Check_Delete"), // 삭제하시겠습니까?
};

UINTERFACE(MinimalAPI)
class UCheckingWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class ARCANEPUNK_API ICheckingWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void OnValidating(ECheckType UpdateCheckType);
	
};
