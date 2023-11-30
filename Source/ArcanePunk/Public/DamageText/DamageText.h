// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "DamageText.generated.h"


UCLASS()
class ARCANEPUNK_API ADamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void TimeLineUpdateFunc(float Output);
 
	UFUNCTION()
	void TimeLineFinishFunc();

	void StartTimeLine();

private:
	UPROPERTY(EditAnywhere)
	UCurveFloat* TimelineCurve;

	FTimeline TimeLine;
 
	FOnTimelineFloat TimeLineUpdateDelegate;
 
	FOnTimelineEvent TimeLineFinishDelegate;

};
