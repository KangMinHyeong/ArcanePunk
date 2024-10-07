// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Interaction/APInteractionTriggerBase.h"
#include "APInteraction_Shop.generated.h"


UCLASS()
class ARCANEPUNK_API AAPInteraction_Shop : public AAPInteractionTriggerBase
{
	GENERATED_BODY()
public:
	AAPInteraction_Shop();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

	void OpenShopUI();

	FORCEINLINE void SetShopData(FShopListData ShopData) {ShopListData = ShopData;};

	void SkillChange(FShopListData ShopData, EEnhanceCategory ChangedSkill);

private:
	void InitSuffle();
	void AddNewSkill();
	void AddEnhance();

	EEnhanceCategory CheckCategoryPercent();
	EEnHanceType CheckTierPercent();
	void CheckNewSkill(FShopGoodsData_NewSkill& NewSkillData);
	void CheckEnhance(FShopGoodsData_Enhance& EnhanceData);
	bool CheckEnhanceNumber(FShopGoodsData_Enhance& EnhanceData);
	void AddAlreadyAbility(const FShopGoodsData_Enhance & EnhanceData);
	TMap<uint8, uint16> GetCurrentNesting(const FShopGoodsData_Enhance & EnhanceData);

	TArray<uint8> IndexSuffle(uint8 MaxNumber);
	FSkillAbilityDataSheet* GetTierData(EEnHanceType EnHanceType, const FString & CurrentRowName);
	void SetEnhanceData(FShopGoodsData_Enhance &EnhanceData, TArray<FString>& RowName, TArray<FString>& AlreadyAbility);

private:	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShopMesh;

	UPROPERTY(EditAnywhere)
	FShopListData ShopListData;

	bool bInit = true;

	UPROPERTY(EditAnywhere)
	uint8 ShopList_Mininum = 5;

	UPROPERTY(EditAnywhere)
	uint8 ShopList_Maximum = 10;

	UPROPERTY(EditAnywhere)
	float SkillPercent = 15.0f; // 스킬 등장 확률 , 100 - SkillPercent = 증강 등장 확률

	
	UPROPERTY(EditAnywhere)
	TMap<EEnhanceCategory, float> CategoryPercent;

	TMap<EEnhanceCategory, float> DefaultCategoryPercent;

	UPROPERTY(EditAnywhere)
	TMap<EEnHanceType, float> TierPercent;

	TWeakObjectPtr<AArcanePunkCharacter> MainPlayer;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillNameListData;
	UPROPERTY(EditAnywhere)
	UDataTable* SkillRowNameData;
	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbility_Silver;
	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbility_Gold;
	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbility_Platinum;

	TArray<uint8> AlreadySkillNumber_Q;
	TArray<uint8> AlreadySkillNumber_E;
	TArray<uint8> AlreadySkillNumber_R;
	TArray<uint8> AlreadySkillNumber_Passive;

	TArray<FString> AlreadyAbility_Silver; // 증강 중첩 갯수에 도달하면
	TArray<FString> AlreadyAbility_Gold;
	TArray<FString> AlreadyAbility_Platinum;

	TArray<FShopGoodsData_Enhance> Enhance_Spare;

};
