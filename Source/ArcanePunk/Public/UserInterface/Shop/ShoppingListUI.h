// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Blueprint/UserWidget.h"
#include "ShoppingListUI.generated.h"

class UTextBlock;
class UBorder;
class UImage;
class UButton;
class UWidgetSwitcher;
class USizeBox;

UCLASS()
class ARCANEPUNK_API UShoppingListUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void InitParent(UUserWidget* UpdateParentWidget, int32 UpdateListNumber);
	void InitEnhanceData(FShopGoodsData_Enhance ShopGoodsData_Enhance);	
	void InitNewSkillData(FShopGoodsData_NewSkill ShopGoodsData_NewSkill);

	void SoldOut();

private:
	void SwitchingBackgroundColor(EEnHanceType EnHanceType);
	void SetPrice(int32 Price);
	void SwitchingAbility(EEnHanceType EnHanceType, FName AbilityName);

	UFUNCTION()
	void OnListClick();
	
private:
	TWeakObjectPtr<UUserWidget> ParentWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_List;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* Border_Background;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_Goods;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_GoodsName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_GoodsNum;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_GoodsPrice;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_New;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Enhance;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_EnhanceName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* Switcher_List;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* SizeBox_SoldOut;

	UPROPERTY(EditAnywhere)
	FLinearColor SilverColor;
	UPROPERTY(EditAnywhere)
	FLinearColor GoldColor;
	UPROPERTY(EditAnywhere)
	FLinearColor PlatinumColor;

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

	bool bEnhanceList = false;

	FShopGoodsData_Enhance EnhanceData;
	FShopGoodsData_NewSkill NewSkillData;

	int32 ListNumber;
};
