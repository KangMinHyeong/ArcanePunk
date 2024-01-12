#include "Components/Character/APSkillHubComponent.h"

#include "Components/Character/APSkillBaseQ.h"
#include "Components/Character/APSkillBaseE.h"
#include "Components/Character/APSkillBaseShift.h"
#include "Components/Character/APSkillBaseSpace.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "Components/Character/APSkillNumber.h"
#include "Components/Character/SkillNumber/SkillNumber1.h"
#include "Components/Character/SkillNumber/SkillNumber2.h"
#include "Components/Character/SkillNumber/SkillNumber3.h"
#include "Components/Character/SkillNumber/SkillNumber4.h"

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
		if(!OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasQSkillType && !OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasESkillType && !OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasRSkillType) {PlayBasicSkill(); return;}
		else { GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);}
	}
	else
	{
		uint8 Second = 1;
		if(!OwnerCharacter->HasSkillType(Second)) return;
		GetWorld()->GetTimerManager().ClearTimer(SkillCancleTimerHandle);
		AddSkillState(SkillState);		
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
		if(!OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasQSkillType && !OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasESkillType && !OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasRSkillType) {PlayBasicSkill(); return;}
		else { GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);}
	}
	else
	{
		uint8 Second = 2;
		if(!OwnerCharacter->HasSkillType(Second)) return;
		GetWorld()->GetTimerManager().ClearTimer(SkillCancleTimerHandle);
		AddSkillState(SkillState);		
		SkillDetermine(SkillState, Second);
	}
}

void UAPSkillHubComponent::PressSpace() // R
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	if(OwnerCharacter->GetDoing()) return;

	if(SkillState == 0)
	{
		SkillState = 3;
		if(!OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasQSkillType && !OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasESkillType && !OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasRSkillType) {PlayBasicSkill(); return;}
		else { GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);}
	}
	else
	{
		uint8 Second = 3;
		if(!OwnerCharacter->HasSkillType(Second)) return;
		GetWorld()->GetTimerManager().ClearTimer(SkillCancleTimerHandle);
		AddSkillState(SkillState);		
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
		SkillState = 4;

		if(!OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasQSkillType && !OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasESkillType && !OwnerCharacter->GetPlayerStatus().PlayerDynamicData.HasRSkillType) {PlayBasicSkill(); return;}
		else { GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);}
	}
	else
	{
		uint8 Second = 4;
		GetWorld()->GetTimerManager().ClearTimer(SkillCancleTimerHandle);
		AddSkillState(SkillState);		
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

	switch(First)
	{
		case 1:
		QSkillComp->SkillBase_Q(Second);
		break;

		case 2:
		ESkillComp->SkillBase_E(Second);
		break;

		case 3: // R
		SpaceSkillComp->SkillBase_Space(Second);
		break;

		case 4: 
		RemoveSkillState(); // 후에 삭제
		OwnerCharacter->SetDoing(false); // 후에 삭제
		//ShiftSkillComp->SkillBase_Shift(Second);
		break;
	}
}

void UAPSkillHubComponent::PlayBasicSkill()
{
	GetWorld()->GetTimerManager().ClearTimer(SkillCancleTimerHandle);

	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	UE_LOG(LogTemp, Display, TEXT("Your 1"));

	switch(SkillState)
	{
		case 1:
		CastSkillNumber(OwnerCharacter->GetQSkill());
		break;

		case 2:
		CastSkillNumber(OwnerCharacter->GetESkill());
		break;

		case 3:
		// Space 대신 R 사용중 (확정 사항 아니라 후에 수정 예정)
		CastSkillNumber(OwnerCharacter->GetRSkill());
		break;

		case 4:
		// Shift 공석
		break;
	}
	SkillState = 0;
}

void UAPSkillHubComponent::CastSkillNumber(uint8 SkillNumber)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	switch (SkillNumber)
	{
		case 1:
		OwnerCharacter->GetAPSkillNumberComponent()->GetSkillNumber1()->PlaySkill();
		break;

		case 2:
		OwnerCharacter->GetAPSkillNumberComponent()->GetSkillNumber2()->PlaySkill();
		break;

		case 3:
		OwnerCharacter->GetAPSkillNumberComponent()->GetSkillNumber3()->PlaySkill();
		break;
	}
}
