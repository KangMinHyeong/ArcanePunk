// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageText/DamageText.h"

#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ADamageText::ADamageText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MyTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));

	SetRootComponent(Root);
	MyTextRender->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ADamageText::BeginPlay()
{
	Super::BeginPlay();

	InitLocation = GetActorLocation();
	StartTimeLine();
	
}

// Called every frame
void ADamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeLine.TickTimeline(DeltaTime);
}

void ADamageText::SetDamageText(float Damage, bool bCriticalAttack)
{
    MyTextRender->SetText(FText::FromString(FString::FromInt(round(Damage))));
	
	if(bCriticalAttack)
	{
		MyTextRender->SetTextRenderColor(CriticalAttackColor);
		InitColor = CriticalAttackColor;
		MyTextRender->SetXScale(CriticalAttackScale); MyTextRender->SetYScale(CriticalAttackScale);
		InitScale = CriticalAttackScale;
	}
	else
	{
		MyTextRender->SetTextRenderColor(NormalAttackColor);
		InitColor = NormalAttackColor;
		MyTextRender->SetXScale(NormalAttackScale); MyTextRender->SetYScale(NormalAttackScale);
		InitScale = NormalAttackScale;
	}
}

void ADamageText::TimeLineUpdateFunc(float Output)
{
	MyTextRender->SetXScale(InitScale*Output);
	MyTextRender->SetYScale(InitScale*Output);
}

void ADamageText::TimeLineUpdateFunc2(FVector Output)
{
	SetActorLocation(InitLocation + FVector(0,0,Output.Z));
}

void ADamageText::TimeLineUpdateFunc3(FLinearColor Output)
{
	MyTextRender->SetTextRenderColor(InitColor.WithAlpha(0));
}

void ADamageText::TimeLineFinishFunc()
{
	Destroy();
}

void ADamageText::StartTimeLine()
{
	if(TimelineCurve && TimelineCurve2 && TimelineCurve3)
	{
		TimeLineUpdateDelegate.BindUFunction(this, FName("TimeLineUpdateFunc"));
		TimeLine.AddInterpFloat(TimelineCurve, TimeLineUpdateDelegate);
		TimeLineUpdateDelegate2.BindUFunction(this, FName("TimeLineUpdateFunc2"));
		TimeLine.AddInterpVector(TimelineCurve2, TimeLineUpdateDelegate2);
		TimeLineUpdateDelegate3.BindUFunction(this, FName("TimeLineUpdateFunc3"));
		TimeLine.AddInterpLinearColor(TimelineCurve3, TimeLineUpdateDelegate3);

		TimeLineFinishDelegate.BindUFunction(this, FName("TimeLineFinishFunc"));
		TimeLine.SetTimelineFinishedFunc(TimeLineFinishDelegate);
	
		TimeLine.SetTimelineLength(MaxLength);
	}
	TimeLine.PlayFromStart();
}
