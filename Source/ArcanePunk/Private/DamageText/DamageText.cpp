// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageText/DamageText.h"

// Sets default values
ADamageText::ADamageText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADamageText::BeginPlay()
{
	Super::BeginPlay();

	StartTimeLine();
	
}

// Called every frame
void ADamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeLine.TickTimeline(DeltaTime);
}

void ADamageText::TimeLineUpdateFunc(float Output)
{
	UE_LOG(LogTemp, Display, TEXT("Your a"));
}

void ADamageText::TimeLineFinishFunc()
{
	UE_LOG(LogTemp, Display, TEXT("Your b"));
	Destroy();
}

void ADamageText::StartTimeLine()
{
	if(TimelineCurve != nullptr)
	{
		TimeLineUpdateDelegate.BindUFunction(this, FName("TimeLineUpdateFunc"));
		TimeLine.AddInterpFloat(TimelineCurve, TimeLineUpdateDelegate);

		TimeLineFinishDelegate.BindUFunction(this, FName("TimeLineFinishFunc"));
		TimeLine.SetTimelineFinishedFunc(TimeLineFinishDelegate);
	
		float Min=0, Max=0;
		TimelineCurve->GetTimeRange(Min,Max);
		TimeLine.SetTimelineLength(Max);
	}
	TimeLine.PlayFromStart();
}
