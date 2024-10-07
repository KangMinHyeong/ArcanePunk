// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "DamageText.generated.h"

class UTextRenderComponent;

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

	void SetDamageText(float Damage, bool bCriticalAttack);

private:
	UFUNCTION()
	void TimeLineUpdateFunc(float Output);
	UFUNCTION()
	void TimeLineUpdateFunc2(const FVector & Output);
	UFUNCTION()
	void TimeLineUpdateFunc3(const FLinearColor & Output);
 
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
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* MyTextRender;

	UPROPERTY(EditAnywhere)
	float MaxLength = 2.0f;

	FVector InitLocation;

	FColor InitColor;

	float InitScale;

	UPROPERTY(EditAnywhere)
	FColor NormalAttackColor;

	UPROPERTY(EditAnywhere)
	FColor CriticalAttackColor;

	UPROPERTY(EditAnywhere)
	float NormalAttackScale = 6.0f;

	UPROPERTY(EditAnywhere)
	float CriticalAttackScale = 9.0f;

	FTimerHandle StopTimerHandle;
};
