// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerState/APPlayerData.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Components/ActorComponent.h"
#include "APPassiveComponent.generated.h"

class AArcanePunkCharacter;
class UAPGameInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPPassiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPPassiveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	void InitPassive();
	void ApplyNewPassive(EPassiveNumber PassiveNum);
	void CheckDamagedGold();
	UFUNCTION()
	void ApplyMoneyPower(int32 AddNum);
	UFUNCTION()
	void RecoveryHP(int32 AddNum);

	FORCEINLINE bool IsDamagedPassive() const {return DamagedPassive;};
	FORCEINLINE float GetDamagedGoldPercent() const {return DamagedGoldPercent;};
	
private:
	void UpdateFromGameInstance();

	void UpdateMaxHP();
	void UpdateATK();
	void UpdateATKSpeed();
	void UpdateMoveSpeed();
	void UpdateMaxMP();
	void UpdateCriticalPercent();
	void UpdateDEF();
	void UpdateGoldPercent();
	void UpdateDamaged();
	void UpdateDrain();

private:
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<UAPGameInstance> APGI;

	UPROPERTY(EditAnywhere)
	FPlayerTotalData Status_Init; // 오리지널 스테이터스 
	UPROPERTY()
	float MPRecoveryTime_Init = 10.0f;
	UPROPERTY()
	float GoldPercent_Init = 0.0f;

	float Previous_MaxHPCoeff = 0.0f;
	float Previous_ATKCoeff = 0.0f;
	float Previous_MaxHPSubCoeff = 0.0f;
	float Previous_ATKSpeedCoeff = 0.0f;
	float Previous_MoveSpeedCoeff = 0.0f;
	uint8 Previous_MaxMPPlus = 0;
	float Previous_CriticalPercentPlus = 0.0f;
	float Previous_DEFCoeff = 0.0f;
	float Previous_DrainCoeff = 0.0f;

	float DamagedGoldPercent = 0.0f;
	float MoneyPowerPercent = 0.0f;
	float HPRecoveryPercent = 0.0f;
	float HPRecoveryAmount = 0.0f;

	bool DamagedPassive = false;
	bool MoneyPower = false;
	bool MoneyRecovery = false;
	// float Previous_GoldPercentPlus = 0.0f;

	FName TEXT_Gold = TEXT("Gold");
};
