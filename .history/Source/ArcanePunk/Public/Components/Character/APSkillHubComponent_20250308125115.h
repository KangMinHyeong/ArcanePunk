// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillDataTable/SkillDataTable.h"
#include "Components/ActorComponent.h"
#include "APSkillHubComponent.generated.h"

class AArcanePunkCharacter;
class AArcanePunkPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillHubComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSkillHubComponent();	
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AutoRecoveryMP();
	void RecoveryMP();

	FORCEINLINE float GetRecoveryTime_MP() const {return RecoveryTime_MP;};
	FORCEINLINE void SetRecoveryTime_MP(float NewValue) {RecoveryTime_MP = NewValue;};
	
private:
	// // 스킬 Base State
	// ESkillKey SkillState = ESkillKey::None;

	// TArray<ESkillKey>SkillStateArr;

	//TimerHandle
	FTimerHandle SkillCancleTimerHandle;
	FTimerHandle RecoveryMPTimerHandle;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<AArcanePunkPlayerController> OwnerCharacterPC;

	bool Proceeding = false;

	UPROPERTY()
	float RecoveryTime_MP = 10.0f;


};
