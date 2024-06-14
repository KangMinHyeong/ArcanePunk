
#include "Components/Common/APGhostTrailSpawnComponent.h"

#include "Components/TimelineComponent.h"
#include "GhostTrail/APGhostTrail.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Character/SkillRange/APSkillRange.h"

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

	if(bSkillGhostTrail)
	{
		TimeLineFinishFunc_Skill();
		// TimeLineSkill.TickTimeline(DeltaTime);
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

		
	}
	if(TimelineCurve_Skill)
	{
		TimeLineTrailDelegate.BindUFunction(this, FName("TimeLineRunTrail"));
		TimeLineFinishDelegate_Skill.BindUFunction(this, FName("TimeLineFinishFunc_Skill"));

		TimeLineSkill.AddInterpFloat(TimelineCurve_Skill, TimeLineTrailDelegate);
		TimeLineSkill.SetTimelineFinishedFunc(TimeLineFinishDelegate_Skill);
	}
}

void UAPGhostTrailSpawnComponent::SetRunTrail(bool NewBool)
{
	if(NewBool) {TimeLineRun.PlayFromStart();}
	else { FinalActor = CurrentActor;}

	bRunGhostTrail = NewBool;
}

void UAPGhostTrailSpawnComponent::SetSkillTrail(bool NewBool)
{
	// if(NewBool) {TimeLineSkill.PlayFromStart();}
	// else { FinalSkillActor = CurrentSkillActor;}
	bSkillGhostTrail = NewBool;
	if(bSkillGhostTrail)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
		SkillTrail = Cast<AAPGhostTrail>(GetWorld()->SpawnActor<AActor>(SkillTrailActor, OwnerCharacter->GetActorLocation() + OwnerCharacter->GetMesh()->GetRelativeLocation() ,OwnerCharacter->GetActorRotation() + OwnerCharacter->GetMesh()->GetRelativeRotation(), SpawnParams));
		if (SkillTrail.IsValid()){FinalSkillActor = SkillTrail.Get(); SkillTrail->SetOwner(OwnerCharacter.Get()); SkillTrail->InitSkillPoseFirst(OwnerCharacter->GetMesh(), TraceTime);}

		FloorTrail = UNiagaraFunctionLibrary::SpawnSystemAttached(SkillFloorTrailSystem, OwnerCharacter->GetMesh(), TEXT("FloorTrail"), FVector(0,0,0), FRotator::ZeroRotator, SkillFloorTrailSize, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
		FloorTrail->SetVariableFloat(TEXT("CustomLifetime"),  TraceTime);

	}
	else
	{
		FloorTrail->DestroyComponent();
		SkillTrail->Destroy();
	}
}

void UAPGhostTrailSpawnComponent::CheckRelease(bool NewBool)
{
	bCheckRelease = NewBool;
}

void UAPGhostTrailSpawnComponent::TimeLineTrail(float Output)
{
}

void UAPGhostTrailSpawnComponent::TimeLineFinishFunc()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	auto Character = Cast<ACharacter>(GetOwner()); if(!Character) return;

	if(bRunGhostTrail) 
	{
		auto GTrail = Cast<AAPGhostTrail>(GetWorld()->SpawnActor<AActor>(GhostTrailActor, Character->GetActorLocation() + Character->GetMesh()->GetRelativeLocation() ,Character->GetActorRotation() + Character->GetMesh()->GetRelativeRotation(), SpawnParams));
		if (GTrail){GTrail->InitPose(Character->GetMesh()); CurrentActor = GTrail;}
	}
	// else if(bSkillGhostTrail)
	// {
	// 	auto GTrail = Cast<AAPGhostTrail>(GetWorld()->SpawnActor<AActor>(SkillTrailActor, Character->GetActorLocation() + Character->GetMesh()->GetRelativeLocation() ,Character->GetActorRotation() + Character->GetMesh()->GetRelativeRotation(), SpawnParams));
	// 	if (GTrail){GTrail->InitPose(Character->GetMesh()); CurrentSkillActor = GTrail;}
	// }

	if (TimeLineRun.GetPlaybackPosition() == 0.0f)
		TimeLineRun.Play();
	else if(TimeLineRun.GetPlaybackPosition() == TimeLineRun.GetTimelineLength())
		TimeLineRun.Reverse();
	
}

void UAPGhostTrailSpawnComponent::TimeLineFinishFunc_Skill()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	if(!OwnerCharacter.IsValid()) return; 
	
	// if(SkillTrail.IsValid())
	// {

	// }
	// else
	// {

	// }

	SkillTrail = Cast<AAPGhostTrail>(GetWorld()->SpawnActor<AActor>(SkillTrailActor, OwnerCharacter->GetActorLocation() + OwnerCharacter->GetMesh()->GetRelativeLocation() ,OwnerCharacter->GetActorRotation() + OwnerCharacter->GetMesh()->GetRelativeRotation(), SpawnParams));
	if (SkillTrail.IsValid()){ SkillTrail->SetOwner(OwnerCharacter.Get()); SkillTrail->SetActorHiddenInGame(true); SkillTrail->InitSkillPose(OwnerCharacter->GetMesh(), TraceTime);}

	if(bCheckRelease)
	{
		if(FinalSkillActor) OwnerCharacter->GetRSkillNumber()->GetSkillRange_Circle()->SetActorLocation(FinalSkillActor->GetActorLocation()); // AttachToActor(FinalSkillActor, FAttachmentTransformRules::KeepWorldTransform);
		if(!OwnerCharacter->GetOnRSkill())  OwnerCharacter->GetRSkillNumber()->Activate_Skill();
	}
	// if (TimeLineSkill.GetPlaybackPosition() == 0.0f)
	// 	TimeLineSkill.Play();
	// else if(TimeLineSkill.GetPlaybackPosition() == TimeLineSkill.GetTimelineLength())
	// 	TimeLineSkill.Reverse();
}


