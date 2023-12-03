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

	void SetDamageText(float Damage);

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

	FTimeline TimeLine;
 
	FOnTimelineFloat TimeLineUpdateDelegate;
	FOnTimelineVector TimeLineUpdateDelegate2;
	FOnTimelineLinearColor TimeLineUpdateDelegate3;
 
	FOnTimelineEvent TimeLineFinishDelegate;

	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	class UTextRenderComponent* MyTextRender;

	UPROPERTY(EditAnywhere)
	float MaxLength = 2.0f;

	FVector InitLocation;

	FColor InitColor;

};
