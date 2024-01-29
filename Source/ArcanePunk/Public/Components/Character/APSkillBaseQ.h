// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/ActorComponent.h"
#include "APSkillBaseQ.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillBaseQ : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAPSkillBaseQ();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 스킬 분류 함수
	void SkillBase_Q(ESkillKey Second);

private:
	// 4종류 스킬 함수
	void Skill_QQ();
	void Skill_QE();
	void Skill_QShift();
	void Skill_QSpace();

};
