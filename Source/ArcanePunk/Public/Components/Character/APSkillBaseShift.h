// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/ActorComponent.h"
#include "APSkillBaseShift.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillBaseShift : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSkillBaseShift();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SkillBase_Shift(uint8 Second);
};
