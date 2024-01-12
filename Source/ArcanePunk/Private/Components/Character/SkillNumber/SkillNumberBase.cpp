#include "Components/Character/SkillNumber/SkillNumberBase.h"

#include "Character/ArcanePunkCharacter.h"

USkillNumberBase::USkillNumberBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillNumberBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetComponentTickEnabled(false);
}

void USkillNumberBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillNumberBase::PlaySkill(uint8 SkillType)
{
}

bool USkillNumberBase::CheckSmartKey(uint8 SkillType, AArcanePunkCharacter* OwnerCharacter)
{
	auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!PC) return false;

	if(PC->SmartKeyArr[SkillType])
	{
		PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		OwnerCharacter->SetHomingPoint(HitResult.Location);
		return true;
	}
	else { return false;}

	return false;
}

void USkillNumberBase::SetMouseCursor(AArcanePunkPlayerController *PC, ECursorType NewCursor)
{
	switch (NewCursor)
	{
		case ECursorType::Default:
		PC->CurrentMouseCursor = EMouseCursor::Default; 
		break;
		case ECursorType::Crosshairs:
		PC->CurrentMouseCursor = EMouseCursor::Crosshairs; 
		break;

	}
	
	CursorImmediately();
}

// Unreal Runtime에서 마우스 커서 변경시 변경이 되지 않거나 커서 이동을 해야 변경되는 오류를 수정해주는 코드
void USkillNumberBase::CursorImmediately()
{
	auto& App = FSlateApplication::Get();
	App.SetAllUserFocusToGameViewport();
	App.QueryCursor();
}