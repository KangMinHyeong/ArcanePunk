// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Character/ArcanePunkCharacter.h"
#include "Blueprint/UserWidget.h"
#include "ShoppingChoiceUI.generated.h"

class UTextBlock;
class UBorder;
class UImage;
class UButton;

UCLASS()
class ARCANEPUNK_API UShoppingChoiceUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void InitParent(UUserWidget* UpdateParentWidget, int32 UpdateChoiceNumber);
	void InitInformation_Enhance(FShopGoodsData_Enhance ShopGoodsData_Enhance);
	void InitInformation_NewSkill(FShopGoodsData_NewSkill ShopGoodsData_NewSkill);

private:
	void SwitchingBackgroundColor(EEnHanceType EnHanceType);
	void SwitchingSkillType(EEnhanceCategory EnhanceCategory);
	void SwitchingAbility(EEnHanceType EnHanceType, FName AbilityName);
	void SwitchingAddition_Enhance(EEnhanceCategory EnhanceCategory, EEnHanceType EnHanceType, uint8 EnhanceNum);
	void SwitchingAddition_NewSkill(EEnHanceType EnHanceType);

	UFUNCTION()
	void OnCheckPurchase();

	void UpdatePlayerGold(int32 GoodsPrice);
	
private:
	FShopGoodsData_Enhance EnhanceData;
	FShopGoodsData_NewSkill NewSkillData;

	TWeakObjectPtr<UUserWidget> ParentWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Purchase;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* Border_Background;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_Goods;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_New;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_GoodsType;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_GoodsName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Tier;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Category;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Information;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Price;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Purchase;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Addition;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Addition_Inform;
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Addition_Inform_2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_PlayerGold;
	
	UPROPERTY(EditAnywhere)
	FLinearColor SilverColor;
	UPROPERTY(EditAnywhere)
	FLinearColor GoldColor;
	UPROPERTY(EditAnywhere)
	FLinearColor PlatinumColor;

	UPROPERTY(EditAnywhere)
	FLinearColor RequireColor;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillNameListData;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbility_Silver;
	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbility_Gold;
	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbility_Platinum;

	FName NormalSkill = TEXT("NormalSkill");
	FName UltSkill = TEXT("UltSkill");
	FName Passive = TEXT("Passive");

	bool bEnhanceUI = true;

	TPair<uint8, uint16> CurrentAbilityData;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	int32 MaxNum = 9;

	int32 ChoiceNumber;

	int32 CurrentPrice = 0;
};
