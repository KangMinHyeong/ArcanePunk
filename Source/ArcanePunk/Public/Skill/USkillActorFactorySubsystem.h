// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillActors/SkillActor.h"
#include "Skill/SkillDataManagers/USkillControllerDataManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "USkillActorFactorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillActorFactorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 등록된 스킬 데이터와 클래스 접근용
	TSharedPtr<ASkillActor> GetSkillActorClass(FName SkillId) const;

private:
	void RegisterAll(); // Map 초기화

	TMap<ESkillType, TSubclassOf<ASkillActor>> SkillClassMap;
};