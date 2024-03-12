// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APAnimHubComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPAnimHubComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAPAnimHubComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 몽타주 바인드 함수
	void BindAttackCheck();
	void BindComboCheck();

	// 몽타주 엔드 콜백 함수
	UFUNCTION()
	void PlayerMontageEnded(UAnimMontage *Montage, bool bInterrupted);

private:
	// 스킬 몽타주 엔드 함수
	void OnAttack_A_MontageEnded();
	void OnAttack_B_MontageEnded();
	void OnSkill_1_MontageEnded();
	void OnSkill_2_MontageEnded();
	void OnSkill_3_MontageEnded();
	void OnSkill_4_MontageEnded();
	void OnSkill_5_MontageEnded();
	void OnSkill_6_MontageEnded();
	void OnSkill_7_MontageEnded();
	void OnSkill_8_MontageEnded();
	void OnSkill_9_MontageEnded();
	void OnSkill_10_MontageEnded();
	void OnSkill_11_MontageEnded();
	void OnSkill_12_MontageEnded();
	void OnSkill_13_MontageEnded();
	void OnSkill_14_MontageEnded();

	void OnUltSkill_1_MontageEnded();
	
};
