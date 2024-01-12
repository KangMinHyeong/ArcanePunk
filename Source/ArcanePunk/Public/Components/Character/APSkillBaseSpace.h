// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APSkillBaseSpace.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillBaseSpace : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSkillBaseSpace();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SkillBase_Space(uint8 Second);

private:
	// 4종류 스킬 함수
	void Skill_SpaceQ();
	void Skill_SpaceE();
	void Skill_SpaceShift();
	void Skill_SpaceSpace();	
};
