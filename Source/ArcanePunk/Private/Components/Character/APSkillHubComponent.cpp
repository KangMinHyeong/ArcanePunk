#include "Components/Character/APSkillHubComponent.h"

#include "Components/Character/APSkillBaseQ.h"
#include "Components/Character/APSkillBaseE.h"
#include "Components/Character/APSkillBaseShift.h"
#include "Components/Character/APSkillBaseSpace.h"
#include "Character/ArcanePunkCharacter.h"

UAPSkillHubComponent::UAPSkillHubComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SkillNumComp = CreateDefaultSubobject<UAPSkillNumber>(TEXT("SkillNumComp"));
	QComp = CreateDefaultSubobject<UAPSkillBaseQ>(TEXT("QComp"));
	EComp = CreateDefaultSubobject<UAPSkillBaseE>(TEXT("EComp"));
	SpaceComp = CreateDefaultSubobject<UAPSkillBaseSpace>(TEXT("SpaceComp"));
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

	if(SkillState == ESkillKey::None)
	{
		SkillState = ESkillKey::Q;
		GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);
	}
	else
	{
		ESkillKey Second = ESkillKey::Q;
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

	if(SkillState == ESkillKey::None)
	{
		SkillState = ESkillKey::E;
		GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);
	}
	else
	{
		ESkillKey Second = ESkillKey::E;
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

	if(SkillState == ESkillKey::None)
	{
		SkillState = ESkillKey::R;
		GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);
	}
	else
	{
		ESkillKey Second = ESkillKey::R;
		GetWorld()->GetTimerManager().ClearTimer(SkillCancleTimerHandle);
		AddSkillState(SkillState);		
		SkillDetermine(SkillState, Second);
	}
}

void UAPSkillHubComponent::PressShift()
{

}

void UAPSkillHubComponent::SetSkillState(ESkillKey NewValue)
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

void UAPSkillHubComponent::AddSkillState(ESkillKey NewValue)
{
	SkillStateArr.Add(NewValue);
}

void UAPSkillHubComponent::RemoveSkillState(ESkillKey NewValue)
{
	if(!SkillStateArr.IsEmpty())
	{
		SkillStateArr.Pop();
		SetSkillState(NewValue);
	}
}

ESkillKey UAPSkillHubComponent::GetSkillState()
{
    return SkillState;
}

void UAPSkillHubComponent::SkillDetermine(ESkillKey First, ESkillKey Second)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	LastSkill = First;
	
	switch(First)
	{
		case ESkillKey::Q:
		QComp->SkillBase_Q(Second);
		break;

		case ESkillKey::E:
		EComp->SkillBase_E(Second);
		break;

		case ESkillKey::R: // R
		SpaceComp->SkillBase_Space(Second);
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
		case ESkillKey::Q:
		CastSkillNumber(OwnerCharacter->GetQSkill(), ESkillKey::Q);
		break;

		case ESkillKey::E:
		CastSkillNumber(OwnerCharacter->GetESkill(), ESkillKey::E);
		break;

		case ESkillKey::R:
		// Space 대신 R 사용중 (확정 사항 아니라 후에 수정 예정)
		CastUltSkillNumber(OwnerCharacter->GetRSkill(), ESkillKey::R);
		break;

		case 4:
		// Shift 공석
		break;
	}
	LastSkill = SkillState;
	SkillState = ESkillKey::None;
}

void UAPSkillHubComponent::CastSkillNumber(ESkillNumber SkillNumber, ESkillKey WhichKey)
{
	switch (SkillNumber)
	{
		case ESkillNumber::Skill_1:
		SkillNumComp->GetSkillNumber1()->PlaySkill(WhichKey);
		break;

		case ESkillNumber::Skill_2:
		SkillNumComp->GetSkillNumber2()->PlaySkill(WhichKey);
		break;

		case ESkillNumber::Skill_3:
		SkillNumComp->GetSkillNumber3()->PlaySkill(WhichKey);
		break;

		case ESkillNumber::Skill_4:
		SkillNumComp->GetSkillNumber4()->PlaySkill(WhichKey);
		break;

		case ESkillNumber::Skill_5:
		SkillNumComp->GetSkillNumber5()->PlaySkill(WhichKey);
		break;

		case ESkillNumber::Skill_6:
		SkillNumComp->GetSkillNumber6()->PlaySkill(WhichKey);
		break;

		case ESkillNumber::Skill_7:
		SkillNumComp->GetSkillNumber7()->PlaySkill(WhichKey);
		break;
	}
}

void UAPSkillHubComponent::CastUltSkillNumber(EUltSkillNumber UltSkillNumber, ESkillKey WhichKey)
{
	if(UltSkillNumber != EUltSkillNumber::UltSkill_1) UE_LOG(LogTemp, Display, TEXT("Your a"));

	switch (UltSkillNumber)
	{
		case EUltSkillNumber::UltSkill_1:
		SkillNumComp->GetUltSkillNumber1()->PlaySkill(WhichKey);
		break;

	}	
}
