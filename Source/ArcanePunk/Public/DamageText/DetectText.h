// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "DetectText.generated.h"

class UTextRenderComponent;

UCLASS()
class ARCANEPUNK_API ADetectText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADetectText();

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
	void TimeLineUpdateFunc2(FVector Output);
	UFUNCTION()
	void TimeLineUpdateFunc3(FLinearColor Output);
 
	UFUNCTION()
	void TimeLineFinishFunc();

	void StartTimeLine();

private:
	UPROPERTY(EditAnywhere)
	UCurveFloat* TimelineCurve;
	UPROPERTY(EditAnywhere)
	UCurveVector* TimelineCurve2;
	UPROPERTY(EditAnywhere)
	UCurveLinearColor* TimelineCurve3;

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* MyTextRender;

	UPROPERTY(EditAnywhere)
	float MaxLength = 2.0f;

	FVector InitLocation;

	float InitScale = 1.0f;

	UPROPERTY(EditAnywhere)
	FColor DetectColor;

	FTimeline TimeLine;
 
	FOnTimelineFloat TimeLineUpdateDelegate;
	FOnTimelineVector TimeLineUpdateDelegate2;
	FOnTimelineLinearColor TimeLineUpdateDelegate3;
	FOnTimelineEvent TimeLineFinishDelegate;

};
