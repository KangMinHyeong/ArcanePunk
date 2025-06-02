// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill/SkillDataStructs/FProjectileSkillActorData.h"
#include "UProjectileSkillActorDataManager.generated.h"

class AAPSkillRange;
class AAPSkillRange_Target;
class AAPSkillRange_Arrow;
class AAPSkillRange_Circle;
class AAPSkillRange_TwoCircle;

UCLASS()
class ARCANEPUNK_API UProjectileSkillActorDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	static void CursorImmediately();

	// 공격 범위 액터
	FORCEINLINE TSubclassOf<AAPSkillRange> GetAPSkillRange() const {return SkillRange;};
	FORCEINLINE TSubclassOf<AAPSkillRange_Target> GetAPSkillRange_Target() const {return SkillRange_Target;};
	FORCEINLINE TSubclassOf<AAPSkillRange_Arrow> GetAPSkillRange_Arrow() const {return SkillRange_Arrow;};
	FORCEINLINE TSubclassOf<AAPSkillRange_Arrow> GetAPSkillRange_Square() const {return SkillRange_Square;};
	FORCEINLINE TSubclassOf<AAPSkillRange_Circle> GetAPSkillRange_Circle() const {return SkillRange_Circle;};
	FORCEINLINE TSubclassOf<AAPSkillRange_TwoCircle> GetAPSkillRange_TWoCircle() const {return SkillRange_TWoCircle;};
	FORCEINLINE TSubclassOf<AAPSkillRange_Arrow> GetAPSkillRange_SectorCircle() const {return SkillRange_SectorCircle;};
	
private:
	// 스킬 데이터 맵
	TMap<FName, FSkillActorData> SkillDataMap;

	UPROPERTY()
	TSubclassOf<AAPSkillRange> SkillRange;
	UPROPERTY()
	TSubclassOf<AAPSkillRange_Target> SkillRange_Target;
	UPROPERTY()
	TSubclassOf<AAPSkillRange_Arrow> SkillRange_Arrow;
	UPROPERTY()
	TSubclassOf<AAPSkillRange_Arrow> SkillRange_Square;
	UPROPERTY()
	TSubclassOf<AAPSkillRange_Circle> SkillRange_Circle;
	UPROPERTY()
	TSubclassOf<AAPSkillRange_TwoCircle> SkillRange_TWoCircle;
	UPROPERTY()
	TSubclassOf<AAPSkillRange_Arrow> SkillRange_SectorCircle;

public:
	void ReadSkillData();
	FSkillActorData GetSkillData(FName skillName);
	
private:
	void CheckForInvalidData(FSkillActorData* SkillRow, const FString& Context);
};
