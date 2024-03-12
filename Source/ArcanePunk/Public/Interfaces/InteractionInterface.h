// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// no inlcude, circular dependency
class AArcanePunkCharacter;
class UUserWidget;
class UTexture2D;
struct FSlateColor;
// UENUM()
// enum class EInteractableType : uint8
// {
// 	Pickup					UMETA(DisplayName = "Pickup"),
// 	NonPlayerCharacter		UMETA(DisplayName = "NonePlayerCharacter"),
// 	Device					UMETA(DisplayName = "Device"),
// 	Toggle					UMETA(DisplayName = "Toggle"),
// 	Container				UMETA(DisplayName = "Container")

// };

// USTRUCT()
// struct FInteractableData
// {
// 	GENERATED_USTRUCT_BODY()

// 	FInteractableData() :
// 	InteractableType(EInteractableType::Pickup),
// 	Name(FText::GetEmpty()),
// 	Action(FText::GetEmpty()),
// 	Quantity(0),
// 	InteractionDuration(0.0f) {};

// 	UPROPERTY(EditInstanceOnly)
// 	EInteractableType InteractableType;

// 	UPROPERTY(EditInstanceOnly)
// 	FText Name;

// 	UPROPERTY(EditInstanceOnly)
// 	FText Action;

// 	// �ֿ�� �ְ� ��ĥ �� �ִ� �������� ���
// 	UPROPERTY(EditInstanceOnly)
// 	int8 Quantity;

// 	// ��ȣ�ۿ��ϴµ� �ʿ��� �ð�
// 	UPROPERTY(EditInstanceOnly)
// 	float InteractionDuration;
// };

UENUM(BlueprintType)
enum class EInteractionType: uint8 // 상호작용 종류
{
	None	 		= 0 UMETA(DisplayName = "None"),
	Drop			= 1 UMETA(DisplayName = "줍기"), // 드랍 상호작용
	Accost			= 2 UMETA(DisplayName = "대화"), // NPC와 대화 상호작용
	SelectStage		= 3 UMETA(DisplayName = "스테이지 선택"), // 스테이지 선택 NPC 상호작용
	Teleport		= 4 UMETA(DisplayName = "포탈 이동"), // Teleport 상호작용
};

USTRUCT()
struct FInteractData
{
	GENERATED_USTRUCT_BODY()

	FInteractData() {}

	UPROPERTY(EditAnywhere)
	EInteractionType InteractionType = EInteractionType::None;

	UPROPERTY(EditAnywhere)
	FString InteractionKeyName = TEXT("F"); // 상호작용 키 이름

	UPROPERTY(EditAnywhere)
	FString InteractionTypeName = TEXT("None");; // 줍기, 대화 등

	UPROPERTY(EditAnywhere)
	int32 TypeNameSize = 28;

	UPROPERTY(EditAnywhere)
	float InteractionDuration = 0.0f; // 꾹 누르기 상호작용 시 필요, 누르고 있어야하는 시간

	UPROPERTY(EditAnywhere)
	bool bZoomIn = false; // 상호작용 시 카메라 줌인 여부
};

USTRUCT()
struct FConversationData
{
	GENERATED_USTRUCT_BODY()

	FConversationData() {}

	UPROPERTY(EditAnywhere)
	UTexture2D* ConversationPortrait;

	UPROPERTY(EditAnywhere)
	FString ConversationActorName;

	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FString ConversationText;

	UPROPERTY(EditAnywhere)
	uint8 EmphasizeFontSize = 22;

	UPROPERTY(EditAnywhere)
	FSlateColor EmphasizeColor;

	UPROPERTY(EditAnywhere)
	TArray<uint16> EmphasizeLetterIndex; // 강조할 글자 위치, 0부터시작
};

USTRUCT()
struct FConversationDataArray
{
	GENERATED_USTRUCT_BODY()

	FConversationDataArray() {}

	UPROPERTY(EditAnywhere)
	TArray<FConversationData> ConversationDataArray;
};

USTRUCT()
struct FConversationDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FConversationDataTable() {}

	UPROPERTY(EditAnywhere)
	FName ConversationPartnerID;

	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	uint8 ConversationPartnerState = 0;

	UPROPERTY(EditAnywhere)
	TArray<FConversationDataArray> ConversationPartnerPrompt; // Left : 강조할 글자 위치, Right : 강조할 글자의 폰트 크기
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()

	
};

class ARCANEPUNK_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual FInteractData GetInteractData();

	virtual void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter);

	// FInteractableData InteractableData;

};
