
#include "Components/Common/APGhostTrailSpawnComponent.h"

#include "Components/TimelineComponent.h"
#include "GhostTrail/APGhostTrail.h"
#include "GameFramework/Character.h"

UAPGhostTrailSpawnComponent::UAPGhostTrailSpawnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAPGhostTrailSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	BindTrail();
}

void UAPGhostTrailSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bRunGhostTrail) {TimeLineRun.TickTimeline(DeltaTime);}
	else
	{
		if(FinalActor.IsValid()) {TimeLineRun.TickTimeline(DeltaTime);}
	}
}

void UAPGhostTrailSpawnComponent::BindTrail()
{
	if(TimelineCurve)
	{
		TimeLineTrailDelegate.BindUFunction(this, FName("TimeLineRunTrail"));
		TimeLineFinishDelegate.BindUFunction(this, FName("TimeLineFinishFunc"));

		TimeLineRun.AddInterpFloat(TimelineCurve, TimeLineTrailDelegate);
		TimeLineRun.SetTimelineFinishedFunc(TimeLineFinishDelegate);
		TimeLineSkill.AddInterpFloat(TimelineCurve, TimeLineTrailDelegate);
		TimeLineSkill.SetTimelineFinishedFunc(TimeLineFinishDelegate);

	}
}

void UAPGhostTrailSpawnComponent::SetRunTrail(bool NewBool)
{
	if(NewBool) {TimeLineRun.PlayFromStart();}
	else { FinalActor = CurrentActor;}

	bRunGhostTrail = NewBool;
}

void UAPGhostTrailSpawnComponent::TimeLineTrail(float Output)
{
}

void UAPGhostTrailSpawnComponent::TimeLineFinishFunc()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	auto Character = Cast<ACharacter>(GetOwner()); if(!Character) return;

	auto GTrail = Cast<AAPGhostTrail>(GetWorld()->SpawnActor<AActor>(GhostTrailActor, Character->GetActorLocation() + Character->GetMesh()->GetRelativeLocation() ,Character->GetActorRotation() + Character->GetMesh()->GetRelativeRotation(), SpawnParams));
	if (GTrail){GTrail->InitPose(Character->GetMesh()); CurrentActor = GTrail;}


	if (TimeLineRun.GetPlaybackPosition() == 0.0f)
		TimeLineRun.Play();
	else if(TimeLineRun.GetPlaybackPosition() == TimeLineRun.GetTimelineLength())
		TimeLineRun.Reverse();
	
}
