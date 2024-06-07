// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Blueprint/UserWidget.h"
#include "APEnhanceSlot.generated.h"

class AArcanePunkCharacter;
class UAPGameInstance;
class UButton;
class UImage;

UCLASS()
class ARCANEPUNK_API UAPEnhanceSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void InitEnhanceData(UUserWidget* Parent, EEnHanceType UpdateEnhanceType, FName UpdateRowName, uint8 UpdateEnhanceNumber, uint16 UpdateNestingNumber);
private:
	UFUNCTION()
	void OnEnhanceInformation();
	
protected:	
	TWeakObjectPtr<UUserWidget> ParentWidget;
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<UAPGameInstance> APGI;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UButton* Button_Enhance;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* Image_TierColor;		

	EEnHanceType EnHanceType;
	FString RowName;
	uint8 EnhanceNumber;
	uint16 NestingNumber;

	UPROPERTY(EditAnywhere)
	FLinearColor GoldColor;
	UPROPERTY(EditAnywhere)
	FLinearColor PlatinumColor;

	FSkillAbilityDataSheet* AbilityData;
};
