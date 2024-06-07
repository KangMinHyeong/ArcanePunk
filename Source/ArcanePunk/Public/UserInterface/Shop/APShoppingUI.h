// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Character/ArcanePunkCharacter.h"
#include "Blueprint/UserWidget.h"
#include "APShoppingUI.generated.h"

class UShoppingListUI;
class UShoppingChoiceUI;
class UScrollBox;
class USizeBox;
class UButton;
class AAPInteraction_Shop;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPShoppingUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void InitShopData(AActor* ShopActor, FShopListData UpdateShopListData);

	void ClickEnhanceList(FShopGoodsData_Enhance EnhanceData, int32 ListNumber);
	void ClickNewSkillList(FShopGoodsData_NewSkill NewSkillData, int32 ListNumber);

	void PurchaseEnhance(int32 ChoiceNumber, FShopGoodsData_Enhance EnhanceData, TPair<uint8, uint16> AbilityNestingNum);
	void PurchaseSkill(int32 ChoiceNumber, FShopGoodsData_NewSkill NewSkillData);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnNotEnoughGold();

private:
	void InitShoppingList();
	UFUNCTION()
	void OnExit();


private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UScrollBox* ScrollBox_List;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	USizeBox* SizeBox_Choice;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Cancel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_PlayerGold;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbilityRowNameData;

	UPROPERTY()
	FShopListData ShopListData;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ShoppingListUIClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ShoppingChoiceUIClass;

	UPROPERTY()
	TArray<UShoppingListUI*> ShoppingList;

	TWeakObjectPtr<UShoppingChoiceUI> ShoppingChoice;

	TWeakObjectPtr<AAPInteraction_Shop> OwnerShop;
};
