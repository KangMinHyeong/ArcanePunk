#include "Components/Character/APSkillNumber.h"

#include "Components/Character/SkillNumber/SkillNumber1.h"
#include "Components/Character/SkillNumber/SkillNumber2.h"
#include "Components/Character/SkillNumber/SkillNumber3.h"
#include "Components/Character/SkillNumber/SkillNumber4.h"
#include "Character/ArcanePunkCharacter.h"

UAPSkillNumber::UAPSkillNumber()
{
	PrimaryComponentTick.bCanEverTick = false;

	SkillNum_1 = CreateDefaultSubobject<USkillNumber1>(TEXT("SkillNum_1"));
	SkillNum_2 = CreateDefaultSubobject<USkillNumber2>(TEXT("SkillNum_2"));
	SkillNum_3 = CreateDefaultSubobject<USkillNumber3>(TEXT("SkillNum_3"));
	SkillNum_4 = CreateDefaultSubobject<USkillNumber4>(TEXT("SkillNum_4"));
}

void UAPSkillNumber::BeginPlay()
{
	Super::BeginPlay();	
}

void UAPSkillNumber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSkillNumber::BindSkill(uint8 SkillNumber, uint8 SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	
	switch(SkillNumber)
	{
		case 1:
		SkillNum_1->PlaySkill(SkillType);
		break;

		case 2:
		SkillNum_2->PlaySkill(SkillType);
		break;

		case 3:
		SkillNum_3->PlaySkill(SkillType);
		break;

		// 현재 스킬 3가지 추가 스킬 생길때마다 추가 
	}
}
