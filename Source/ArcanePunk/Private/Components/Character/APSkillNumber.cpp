#include "Components/Character/APSkillNumber.h"

UAPSkillNumber::UAPSkillNumber()
{
	PrimaryComponentTick.bCanEverTick = false;

	SkillNumBase = CreateDefaultSubobject<USkillNumberBase>(TEXT("SkillNumBase"));
	SkillNum_1 = CreateDefaultSubobject<USkillNumber1>(TEXT("SkillNum_1"));
	SkillNum_2 = CreateDefaultSubobject<USkillNumber2>(TEXT("SkillNum_2"));
	SkillNum_3 = CreateDefaultSubobject<USkillNumber3>(TEXT("SkillNum_3"));
	SkillNum_4 = CreateDefaultSubobject<USkillNumber4>(TEXT("SkillNum_4"));
	SkillNum_5 = CreateDefaultSubobject<USkillNumber5>(TEXT("SkillNum_5"));
	SkillNum_6 = CreateDefaultSubobject<USkillNumber6>(TEXT("SkillNum_6"));
	SkillNum_7 = CreateDefaultSubobject<USkillNumber7>(TEXT("SkillNum_7"));
	SkillNum_8 = CreateDefaultSubobject<USkillNumber8>(TEXT("SkillNum_8"));
	SkillNum_9 = CreateDefaultSubobject<USkillNumber9>(TEXT("SkillNum_9"));
	SkillNum_10 = CreateDefaultSubobject<USkillNumber10>(TEXT("SkillNum_10"));
	SkillNum_11 = CreateDefaultSubobject<USkillNumber11>(TEXT("SkillNum_11"));
	SkillNum_12 = CreateDefaultSubobject<USkillNumber12>(TEXT("SkillNum_12"));
	SkillNum_13 = CreateDefaultSubobject<USkillNumber13>(TEXT("SkillNum_13"));
	SkillNum_14 = CreateDefaultSubobject<USkillNumber14>(TEXT("SkillNum_14"));

	UltSkillNum_1 = CreateDefaultSubobject<UUltSkillNumber_1>(TEXT("UltSkillNum_1"));
}

void UAPSkillNumber::BeginPlay()
{
	Super::BeginPlay();	
}

void UAPSkillNumber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSkillNumber::BindSkill(ESkillNumber SkillNumber, ESkillTypeState SkillType, ESkillKey WhichKey)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	
	switch(SkillNumber)
	{
		case ESkillNumber::Skill_1:
		SkillNum_1->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_2:
		SkillNum_2->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_3:
		SkillNum_3->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_4:
		SkillNum_4->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_5:
		SkillNum_5->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_6:
		SkillNum_6->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_7:
		SkillNum_7->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_8:
		SkillNum_8->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_9:
		SkillNum_9->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_10:
		SkillNum_10->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_11:
		SkillNum_11->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_12:
		SkillNum_12->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_13:
		SkillNum_13->PlaySkill(WhichKey, SkillType);
		break;

		case ESkillNumber::Skill_14:
		SkillNum_14->PlaySkill(WhichKey, SkillType);
		break;

		// 추가 스킬 생길때마다 추가 
	}
}

void UAPSkillNumber::BindUltSkill(EUltSkillNumber UltSkillNumber, ESkillTypeState SkillType, ESkillKey WhichKey)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	
	switch(UltSkillNumber)
	{
		case EUltSkillNumber::UltSkill_1:
		UltSkillNum_1->PlaySkill(WhichKey, SkillType);
		break;

		// 추가 스킬 생길때마다 추가 
	}
}

