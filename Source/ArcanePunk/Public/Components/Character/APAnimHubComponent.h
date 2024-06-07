// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APAnimHubComponent.generated.h"

class AAPCharacterBase;
class UArcanePunkCharacterAnimInstance;

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
	void ComboCheck();

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
	void OnSkill_15_MontageEnded();
	void OnSkill_18_MontageEnded();
	void OnSkill_19_MontageEnded();
	void OnSkill_20_MontageEnded();
	
	void OnSkill_5_Fire_MontageEnded();

	void OnUltSkill_1_MontageEnded();
	void OnUltSkill_4_MontageEnded();
	void OnUltSkill_5_MontageEnded();
	void OnUltSkill_6_MontageEnded();
	void OnUltSkill_8_MontageEnded();
	void OnUltSkill_9_MontageEnded();
	void OnUltSkill_10_MontageEnded();
	void OnUltSkill_11_MontageEnded();
	void OnUltSkill_12_MontageEnded();
	void OnUltSkill_13_MontageEnded();
	void OnUltSkill_14_MontageEnded();
	void OnUltSkill_15_MontageEnded();
	void OnUltSkill_17_MontageEnded();
	
	void OnUltSkill_Fire_MontageEnded();

private:
	TWeakObjectPtr<AAPCharacterBase> OwnerCharacter;
	TWeakObjectPtr<UArcanePunkCharacterAnimInstance> OwnerAnim;
};
