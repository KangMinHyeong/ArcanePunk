// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// no inlcude, circular dependency
class AArcanePunkCharacter;
class UUserWidget;
class UTexture2D;
struct FSlateColor;

UENUM(BlueprintType)
enum class EInteractionType: uint8 // 상호작용 종류
{
	None	 				= 0 UMETA(DisplayName = "None"),
	Interact_Drop			= 1 UMETA(DisplayName = "Interact_Drop"), // 드랍 상호작용
	Interact_Conversation	= 2 UMETA(DisplayName = "Interact_Conversation"), // NPC와 대화 상호작용
	Interact_SelectStage	= 3 UMETA(DisplayName = "Interact_SelectStage"), // 스테이지 선택 NPC 상호작용
	Interact_Portal			= 4 UMETA(DisplayName = "Interact_Portal"), // Teleport 상호작용
	Interact_Shop			= 5 UMETA(DisplayName = "Interact_Shop"), // 상점 상호작용
};

UENUM(BlueprintType)
enum class EStringType: uint8 // String 종류
{
	None	 			= 0 UMETA(DisplayName = "None"),
	System				= 1 UMETA(DisplayName = "1"), // 시스템 메세지
	HUD					= 2 UMETA(DisplayName = "2"), // HUD
	Conversation		= 3 UMETA(DisplayName = "3"), // 대화
	Interact			= 4 UMETA(DisplayName = "4"), // 상호작용 메세지
	Map					= 5 UMETA(DisplayName = "5"), // 마을 이름
};


UENUM(BlueprintType)
enum class EStringRowName : uint8 // String RowName
{
	None 	= 0 UMETA(DisplayName = "None"),
	Load = 1 UMETA(DisplayName = "Load"),
	Save = 2 UMETA(DisplayName = "Save"),
	Delete = 3 UMETA(DisplayName = "Delete"),
	SaveSlotName = 4 UMETA(DisplayName = "SaveSlotName"),
	NoData = 5 UMETA(DisplayName = "NoData"),
	Slot = 6 UMETA(DisplayName = "Slot"),
	PlayTime = 7 UMETA(DisplayName = "PlayTime"),
	Hour = 8 UMETA(DisplayName = "Hour"),
	Minute = 9 UMETA(DisplayName = "Minute"),
	Second = 10 UMETA(DisplayName = "Second"),
	Validation = 11 UMETA(DisplayName = "Validation"),
	Cancel = 12 UMETA(DisplayName = "Cancel"),
	ItemList = 13 UMETA(DisplayName = "ItemList"),
	Quantity = 14 UMETA(DisplayName = "Quantity"),
	Price = 15 UMETA(DisplayName = "Price"),
	Information = 16 UMETA(DisplayName = "Information"),
	NoEnoughGold = 17 UMETA(DisplayName = "NoEnoughGold"),

	NewSkill = 18 UMETA(DisplayName = "NewSkill"),
	SkillName = 19 UMETA(DisplayName = "SkillName"),
	EnhanceSkill = 20 UMETA(DisplayName = "EnhanceSkill"),
	TypeName = 21 UMETA(DisplayName = "TypeName"),
	TierName = 22 UMETA(DisplayName = "TierName"),

	Tier_1 = 23 UMETA(DisplayName = "Tier_1"),
	Tier_2 = 24 UMETA(DisplayName = "Tier_2"),
	Tier_3 = 25 UMETA(DisplayName = "Tier_3"),

	Optimization = 26 UMETA(DisplayName = "Optimization"),

	CurrentNesting = 27 UMETA(DisplayName = "CurrentNesting"),
	Count = 28 UMETA(DisplayName = "Count"),
	AdditionEnhance = 29 UMETA(DisplayName = "AdditionEnhance"),
	CurrentGold = 30 UMETA(DisplayName = "CurrentGold"),
	Purchase = 31 UMETA(DisplayName = "Purchase"),

	Skill_Q = 32 UMETA(DisplayName = "Skill_Q"),
	Skill_E = 33 UMETA(DisplayName = "Skill_E"),
	Skill_R = 34 UMETA(DisplayName = "Skill_R"),
	Passive = 35 UMETA(DisplayName = "Passive"),
	Active = 36 UMETA(DisplayName = "Active"),

	NewGame = 37 UMETA(DisplayName = "NewGame"),
	Continue = 38 UMETA(DisplayName = "Continue"),
	GameSetting = 39 UMETA(DisplayName = "GameSetting"),
	GameExit = 40 UMETA(DisplayName = "GameExit"),
	MainTitle = 41 UMETA(DisplayName = "MainTitle"),
	SubTitle = 42 UMETA(DisplayName = "SubTitle"),

	Volume_Master = 43 UMETA(DisplayName = "Volume_Master"),
	Volume_BGM = 44 UMETA(DisplayName = "Volume_BGM"),
	Volume_Effect = 45 UMETA(DisplayName = "Volume_Effect"),

	Init = 46 UMETA(DisplayName = "Init"),
	Apply = 47 UMETA(DisplayName = "Apply"),
	
	SmartKey_Q = 48 UMETA(DisplayName = "SmartKey_Q"),
	SmartKey_E = 49 UMETA(DisplayName = "SmartKey_E"),
	SmartKey_R = 50 UMETA(DisplayName = "SmartKey_R"),

	Button_On = 51 UMETA(DisplayName = "Button_On"),
	Button_Off = 52 UMETA(DisplayName = "Button_Off"),

	Grading_Saturation = 53 UMETA(DisplayName = "Grading_Saturation"),
	Grading_Contrast = 54 UMETA(DisplayName = "Grading_Contrast"),
	Grading_Gamma = 55 UMETA(DisplayName = "Grading_Gamma"),
	Grading_Gain = 56 UMETA(DisplayName = "Grading_Gain"),
	Grading_Offset = 57 UMETA(DisplayName = "Grading_Offset"),

	Setting_Graphic = 58 UMETA(DisplayName = "Setting_Graphic"),
	Setting_Audio = 59 UMETA(DisplayName = "Setting_Audio"),
	Setting_SmartKey = 60 UMETA(DisplayName = "Setting_SmartKey"),
	Setting_Screen = 61 UMETA(DisplayName = "Setting_Screen"),

	ScreenMode_Full = 62 UMETA(DisplayName = "ScreenMode_Full"),
	ScreenMode_Window = 63 UMETA(DisplayName = "ScreenMode_Window"),

	Select_WindowMode = 64 UMETA(DisplayName = "Select_WindowMode"),
	Select_AspectRatio = 65 UMETA(DisplayName = "Select_AspectRatio"),
	Select_MaxFPS = 66 UMETA(DisplayName = "Select_MaxFPS"),
	Select_Texture = 67 UMETA(DisplayName = "Select_Texture"),
	Select_AntiAliasing = 68 UMETA(DisplayName = "Select_AntiAliasing"),
	Select_PostProcessing = 69 UMETA(DisplayName = "Select_PostProcessing"),
	Select_Shadow = 70 UMETA(DisplayName = "Select_Shadow"),
	Select_Effect = 71 UMETA(DisplayName = "Select_Effect"),
	Select_Foliage = 72 UMETA(DisplayName = "Select_Foliage"),
	Select_Shading = 73 UMETA(DisplayName = "Select_Shading"),
	Select_Reflection = 74 UMETA(DisplayName = "Select_Reflection"),
	Select_ViewDistance = 75 UMETA(DisplayName = "Select_ViewDistance"),
	Select_GlobalIllumination = 76 UMETA(DisplayName = "Select_GlobalIllumination"),
	Select_ResolutionScale = 77 UMETA(DisplayName = "Select_ResolutionScale"),
	Select_OverAll = 78 UMETA(DisplayName = "Select_OverAll"),

	NoLimit = 79 UMETA(DisplayName = "NoLimit"),

	Quality_Low = 80 UMETA(DisplayName = "Quality_Low"),
	Quality_Middle = 81 UMETA(DisplayName = "Quality_Middle"),
	Quality_High = 82 UMETA(DisplayName = "Quality_High"),
	Quality_Ultra = 83 UMETA(DisplayName = "Quality_Ultra"),

	Select = 84 UMETA(DisplayName = "Select"),

	Message_EnhanceChoice = 85 UMETA(DisplayName = "Message_EnhanceChoice"),

	CurrentReroll = 86 UMETA(DisplayName = "CurrentReroll"),
	Message_MaxEnhance = 87 UMETA(DisplayName = "Message_MaxEnhance"),

	RemainWave = 88 UMETA(DisplayName = "RemainWave"),
	RemainWaveTime = 89 UMETA(DisplayName = "RemainWaveTime"),

	GuideBarSetting = 90 UMETA(DisplayName = "GuideBarSetting"),

	Setting_OtherSettings = 91 UMETA(DisplayName = "Setting_OtherSettings"),

	Volume_Voice = 92 UMETA(DisplayName = "Volume_Voice"),
	Volume_UI = 93 UMETA(DisplayName = "Volume_UI"),

	SaveComplete = 94 UMETA(DisplayName = "SaveComplete"),
	CannotSkill = 95 UMETA(DisplayName = "CannotSkill"),
	PortalOpen = 96 UMETA(DisplayName = "PortalOpen"),
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
	int32 TypeNameSize = 28;

	UPROPERTY(EditAnywhere)
	float InteractionDuration = 0.0f; // 꾹 누르기 상호작용 시 필요, 누르고 있어야하는 시간

	UPROPERTY(EditAnywhere)
	bool bZoomIn = false; // 상호작용 시 카메라 줌인 여부
};

USTRUCT()
struct FShopGoodsData_Enhance
{
	GENERATED_USTRUCT_BODY()

	FShopGoodsData_Enhance() {}

	UPROPERTY(EditAnywhere)
	EEnhanceCategory GoodsCategory;

	UPROPERTY(EditAnywhere)
	EEnHanceType GoodsType;

	UPROPERTY(EditAnywhere)
	uint8 SkillNumber = 0; // ex - 5 : Skill_5 -> 아케인빔 , UltSkill_5 -> 질풍쇄도

	UPROPERTY(EditAnywhere)
	FName SkillName; // ex - Skill_8 , Row이름으로 

	UPROPERTY(EditAnywhere)
	uint8 EnhanceNumber; // ex - 1 -> Skill_8_1

	UPROPERTY(EditAnywhere)
	FName EnhanceName; // ex - Skill_8_1 , Row이름으로 

	UPROPERTY(EditAnywhere)
	uint8 GoodsNum = 1;

	UPROPERTY(EditAnywhere)
	int32 GoodsPrice = 1;
};

USTRUCT()
struct FShopGoodsData_NewSkill
{
	GENERATED_USTRUCT_BODY()

	FShopGoodsData_NewSkill() {}

	UPROPERTY(EditAnywhere)
	EEnhanceCategory GoodsCategory;
	
	UPROPERTY(EditAnywhere)
	EEnHanceType GoodsType;

	UPROPERTY(EditAnywhere)
	ESkillKey SkillKey;

	UPROPERTY(EditAnywhere)
	uint8 SkillNumber = 0; // ex - 5 : Skill -> 아케인빔 , UltSkill -> 질풍쇄도

	UPROPERTY(EditAnywhere)
	FName NewSkillName;

	UPROPERTY(EditAnywhere)
	int32 GoodsPrice = 1;

	UPROPERTY(EditAnywhere)
	bool bSoldOut = false;
};

USTRUCT()
struct FShopListData
{
	GENERATED_USTRUCT_BODY()

	FShopListData() {}

	UPROPERTY(EditAnywhere)
	TArray<FShopGoodsData_Enhance> ShopGoodsList_Enhance;

	UPROPERTY(EditAnywhere)
	TArray<FShopGoodsData_NewSkill> ShopGoodsList_NewSkill;
};

USTRUCT()
struct FStringDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FStringDataTable() {}

	UPROPERTY(EditAnywhere)
	int32 StringIndex = 0;

	UPROPERTY(EditAnywhere)
	EStringType StringType;

	UPROPERTY(EditAnywhere)
	FString StringType_Des;

	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FString Content;
};

USTRUCT()
struct FDialogueDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FDialogueDataTable() {}

	UPROPERTY(EditAnywhere)
	int32 Diologue_ID = 0;

	UPROPERTY(EditAnywhere)
	int32 Message_Sort = 0;

	UPROPERTY(EditAnywhere)
	int32 Character_ID = 0;

	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FString Diologue_Text;

	UPROPERTY(EditAnywhere)
	bool bEmphasize = false;
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
	virtual bool BeginFocus();
	// virtual void EndFocus();
	virtual FInteractData GetInteractData();

	UFUNCTION()
	virtual void BeginInteract();
	UFUNCTION()
	virtual void EndInteract();
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter);

	// FInteractableData InteractableData;

};
