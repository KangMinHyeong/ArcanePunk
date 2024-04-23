// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Components/ActorComponent.h"
#include "APGhostTrailSpawnComponent.generated.h"

class AAPGhostTrail;
class UNiagaraComponent;
class UNiagaraSystem;
class AArcanePunkCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPGhostTrailSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPGhostTrailSpawnComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetRunTrail(bool NewBool);
	void SetSkillTrail(bool NewBool);
	FORCEINLINE AActor* GetSkillTrail() const {return FinalSkillActor;};
	FORCEINLINE void SetSkillTrail(AActor* NewActor) {FinalSkillActor = NewActor;};
	void CheckRelease(bool NewBool);

private:
	void BindTrail();
	UFUNCTION()
	void TimeLineTrail(float Output);
	UFUNCTION()
	void TimeLineFinishFunc();

	UFUNCTION()
	void TimeLineFinishFunc_Skill();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GhostTrailActor;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SkillTrailActor;

	UPROPERTY(EditAnywhere)
	UCurveFloat* TimelineCurve;
	UPROPERTY(EditAnywhere)
	UCurveFloat* TimelineCurve_Skill;

	FTimeline TimeLineRun;
	FTimeline TimeLineSkill;

	FOnTimelineFloat TimeLineTrailDelegate;
	FOnTimelineEvent TimeLineFinishDelegate;

	FOnTimelineEvent TimeLineFinishDelegate_Skill;

	TWeakObjectPtr<AActor> CurrentActor;
	TWeakObjectPtr<AActor> FinalActor;

	TWeakObjectPtr<AActor> CurrentSkillActor;
	UPROPERTY()
	AActor* FinalSkillActor;
	

	bool bRunGhostTrail = false;
	bool bSkillGhostTrail = false;

	FTimerHandle GhostTimerHandle;

	TWeakObjectPtr<AAPGhostTrail> SkillTrail;

	UPROPERTY(EditAnywhere)
	float TraceTime = 3.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SkillFloorTrailSystem;

	UPROPERTY()
	UNiagaraComponent* FloorTrail;

	UPROPERTY(EditAnywhere)
	FVector SkillFloorTrailSize = FVector(1.0f,1.0f,1.0f);

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	bool bCheckRelease = false;
};
