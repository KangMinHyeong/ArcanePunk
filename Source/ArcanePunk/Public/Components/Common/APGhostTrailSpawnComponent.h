// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Components/ActorComponent.h"
#include "APGhostTrailSpawnComponent.generated.h"


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

private:
	void BindTrail();
	UFUNCTION()
	void TimeLineTrail(float Output);
	UFUNCTION()
	void TimeLineFinishFunc();
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GhostTrailActor;

	UPROPERTY(EditAnywhere)
	UCurveFloat* TimelineCurve;

	FTimeline TimeLineRun;
	FTimeline TimeLineSkill;
	FOnTimelineFloat TimeLineTrailDelegate;
	FOnTimelineEvent TimeLineFinishDelegate;

	TWeakObjectPtr<AActor> CurrentActor;
	TWeakObjectPtr<AActor> FinalActor;

	bool bRunGhostTrail = false;
	bool bSkillGhostTrail = false;
};
