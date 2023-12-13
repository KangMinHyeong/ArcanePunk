#include "Components/Character/APSkillHubComponent.h"

#include "Components/Character/APSkillBaseQ.h"
#include "Components/Character/APSkillBaseE.h"
#include "Components/Character/APSkillBaseShift.h"
#include "Components/Character/APSkillBaseSpace.h"
#include "Character/ArcanePunkCharacter.h"

UAPSkillHubComponent::UAPSkillHubComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	QSkillComp = CreateDefaultSubobject<UAPSkillBaseQ>(TEXT("QSkillComp"));
	ESkillComp = CreateDefaultSubobject<UAPSkillBaseE>(TEXT("ESkillComp"));
	ShiftSkillComp = CreateDefaultSubobject<UAPSkillBaseShift>(TEXT("ShiftSkillComp"));
	SpaceSkillComp = CreateDefaultSubobject<UAPSkillBaseSpace>(TEXT("SpaceSkillComp"));
}

void UAPSkillHubComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAPSkillHubComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSkillHubComponent::PressQ()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	if(OwnerCharacter->GetDoing()) return;

	if(SkillState == 0)
	{
		SkillState = 1;
	}
	else
	{
		AddSkillState(SkillState);		
		uint8 Second = 1;
		SkillDetermine(SkillState, Second);
	}
}

void UAPSkillHubComponent::PressE()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	if(OwnerCharacter->GetDoing()) return;

	if(SkillState == 0)
	{
		SkillState = 2;
	}
	else
	{
		AddSkillState(SkillState);		
		uint8 Second = 2;
		SkillDetermine(SkillState, Second);
	}
}

void UAPSkillHubComponent::PressShift()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	if(OwnerCharacter->GetDoing()) return;

	if(SkillState == 0)
	{
		SkillState = 3;
	}
	else
	{
		AddSkillState(SkillState);		
		uint8 Second = 3;
		SkillDetermine(SkillState, Second);
	}
}

void UAPSkillHubComponent::PressSpace()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	if(OwnerCharacter->GetDoing()) return;
	
	if(SkillState == 0)
	{
		SkillState = 4;
	}
	else
	{
		AddSkillState(SkillState);		
		uint8 Second = 4;
		SkillDetermine(SkillState, Second);
	}
}

void UAPSkillHubComponent::SetSkillState(uint8 NewValue)
{
	if(SkillStateArr.IsEmpty())
	{
		SkillState = NewValue;
	}
	else
	{
		SkillState = SkillStateArr.Top();
	}
}

void UAPSkillHubComponent::AddSkillState(uint8 NewValue)
{
	SkillStateArr.Add(NewValue);
}

void UAPSkillHubComponent::RemoveSkillState(uint8 NewValue)
{
	if(!SkillStateArr.IsEmpty())
	{
		SkillStateArr.Pop();
		SetSkillState(NewValue);
	}
}

uint8 UAPSkillHubComponent::GetSkillState()
{
    return SkillState;
}

void UAPSkillHubComponent::SkillDetermine(uint8 First, uint8 Second)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	OwnerCharacter->SetDoing(true);

	switch(First)
	{
		case 1:
		QSkillComp->SkillBase_Q(Second);
		break;

		case 2:
		RemoveSkillState(); // 후에 삭제
		OwnerCharacter->SetDoing(false); // 후에 삭제
		//ESkillComp->SkillBase_E(Second);
		break;

		case 3:
		RemoveSkillState(); // 후에 삭제
		OwnerCharacter->SetDoing(false); // 후에 삭제
		//ShiftSkillComp->SkillBase_Shift(Second);
		break;

		case 4:
		RemoveSkillState(); // 후에 삭제
		OwnerCharacter->SetDoing(false); // 후에 삭제
		//SpaceSkillComp->SkillBase_Space(Second);
		break;
	}
}
