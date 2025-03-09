// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillDataTable/SkillDataTable.h"
#include "Components/ActorComponent.h"
#include "APSkillType.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillType : public UActorComponent
{
	GENERATED_BODY()
public:	
	UAPSkillType();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetSkillType(ESkillTypeState UpdateSkillType, ESkillCategory UpdateSkillCategory);

private:
	// void Type_Projecitle();
	// void Type_Throw();
	// void Type_Laser();
	// void Type_InvisibleCollider();
	// void Type_Buff();

private:
	ESkillTypeState SkillType;

	ESkillCategory SkillCategory;

};
