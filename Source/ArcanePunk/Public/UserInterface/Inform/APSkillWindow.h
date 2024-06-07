// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Blueprint/UserWidget.h"
#include "APSkillWindow.generated.h"

class UTextBlock;
class UButton;
class UWidgetSwitcher;
class USizeBox;
class UScrollBox;
class AArcanePunkCharacter;
class UAPGameInstance;

UCLASS()
class ARCANEPUNK_API UAPSkillWindow : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;

	void OpenSkillInformation(EEnhanceCategory UpdateEnhanceCategory, FSkillNameList* SkillNameTable, float CoolTime);
	void OpenEnhanceInformation(EEnHanceType UpdateEnhanceType, FSkillAbilityDataSheet* UpdateAbilityData, uint16 UpdateNestingNumber);

private:
	void BindButton();

	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void OnActive();
	UFUNCTION()
	void OnPassive();

	void InitActiveSkill();
	void InitPassiveSkill();
	
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UButton* Button_Cancel;	

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UButton* Button_Active;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UButton* Button_Passive;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UWidgetSwitcher* Switcher_Change;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UScrollBox* ScrollBox_Active;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UScrollBox* ScrollBox_Passive;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UScrollBox* ScrollBox_Information;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_Active;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_Passive;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SkillWindowSlotClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SkillWindowInformationClass;

	UPROPERTY(EditAnywhere)
	FSlateColor OnColor;
	UPROPERTY(EditAnywhere)
	FSlateColor OffColor;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<UAPGameInstance> APGI;
};
