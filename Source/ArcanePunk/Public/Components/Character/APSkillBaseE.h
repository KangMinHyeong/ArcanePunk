// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APSkillBaseE.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillBaseE : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSkillBaseE();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SkillBase_E(uint8 Second);

private:
	// 4종류 스킬 함수
	void Skill_EQ();
	void Skill_EE();
	void Skill_EShift();
	void Skill_ESpace();

};
