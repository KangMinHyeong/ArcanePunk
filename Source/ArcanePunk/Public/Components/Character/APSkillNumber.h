// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APSkillNumber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillNumber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSkillNumber();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindSkill(uint8 SkillNumber);

	class USkillNumber1* GetSkillNumber1();
	class USkillNumber2* GetSkillNumber2();
	class USkillNumber3* GetSkillNumber3();
private:
	UPROPERTY()
	class USkillNumber1* Skill_1;
	UPROPERTY()
	class USkillNumber2* Skill_2; 
	UPROPERTY()
	class USkillNumber3* Skill_3;
};
