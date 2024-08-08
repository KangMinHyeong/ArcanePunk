
#include "GameElements/DamageText/DetectText.h"

#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"

ADetectText::ADetectText()
{
 	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MyTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));

	SetRootComponent(Root);
	MyTextRender->SetupAttachment(Root);
}

void ADetectText::BeginPlay()
{
	Super::BeginPlay();
	
	InitLocation = GetActorLocation();
	StartTimeLine();
}

void ADetectText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeLine.TickTimeline(DeltaTime);
}

void ADetectText::TimeLineUpdateFunc(float Output)
{
	MyTextRender->SetXScale(InitScale*Output);
	MyTextRender->SetYScale(InitScale*Output);
}

void ADetectText::TimeLineUpdateFunc2(FVector Output)
{
	SetActorLocation(InitLocation + Output);
}

void ADetectText::TimeLineUpdateFunc3(FLinearColor Output)
{
	MyTextRender->SetTextRenderColor(DetectColor.WithAlpha(Output.A));
}

void ADetectText::TimeLineFinishFunc()
{
	Destroy();
}

void ADetectText::StartTimeLine()
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
