#include "Components/Character/SkillNumber/SkillNumberBase.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/SkillRange/APSkillRange_Arrow.h"
#include "Character/SkillRange/APSkillRange_Circle.h"

USkillNumberBase::USkillNumberBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillNumberBase::BeginPlay()
{
	Super::BeginPlay();
	
	CreateInit();
}

void USkillNumberBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckingOtherSkill();
}

void USkillNumberBase::CreateInit()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(false);
	AddAbilityList();
}

void USkillNumberBase::CheckingOtherSkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	if(CheckSmartKey(SkillKey, OwnerCharacter)) 
	{ 
		OnSkill(); return; 
	}
	if(OwnerCharacter->GetAPSkillHubComponent()->GetSkillState() != ESkillKey::None && Skilling)
	{
		if(OwnerCharacter->GetAPSkillHubComponent()->GetSkillState() != OwnerCharacter->GetAPSkillHubComponent()->LastSkill) Remove_Skill(); return;
	}
	if(OwnerCharacter->returnState() != ECharacterState::None && Skilling)
	{
		Remove_Skill(); return;
	}
}

void USkillNumberBase::PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType)
{
}

void USkillNumberBase::OnSkill()
{
}

void USkillNumberBase::Remove_Skill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;

	SetComponentTickEnabled(false);

	if(SkillRange_Target.IsValid()) SkillRange_Target->Destroy();
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->Destroy();
 
	OwnerCharacterPC->bShowMouseCursor = true;
	Skilling = false;
}

void USkillNumberBase::SkillEnd()
{
}

void USkillNumberBase::RemoveEffect()
{
}

void USkillNumberBase::Activate_Skill()
{
}

void USkillNumberBase::Enhance()
{
}

void USkillNumberBase::MarkingOn(AActor *OtherActor, float Time)
{
}

void USkillNumberBase::MarkErase()
{
}

bool USkillNumberBase::CheckSmartKey(ESkillKey WhichKey, AArcanePunkCharacter *OwnerCharacter)
{
	auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!PC) return false;

	if(PC->SmartKeyArr[(uint8)WhichKey])
	{
		PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		OwnerCharacter->SetHomingPoint(HitResult.Location);
		return true;
	}
	else {return false;}

	return false;
}

void USkillNumberBase::SetMouseCursor(AArcanePunkPlayerController *PC, ESkillCursor NewCursor)
{
	switch (NewCursor)
	{
		case ESkillCursor::Default:
		PC->CurrentMouseCursor = EMouseCursor::Default; 
		break;
		case ESkillCursor::Crosshairs:
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

void USkillNumberBase::SkillCancel()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
}

void USkillNumberBase::SetAbility(ESkillKey WhichKey)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;

	switch (WhichKey)
	{
		case ESkillKey::Q:
		CurrentSkillAbility = OwnerCharacter->GetAbilitySkillQ();
		break;
		case ESkillKey::E:
		CurrentSkillAbility = OwnerCharacter->GetAbilitySkillE();
		break;
		case ESkillKey::R:
		CurrentSkillAbility = OwnerCharacter->GetAbilitySkillR();
		break;
	}
	
}

void USkillNumberBase::AddAbilityList()
{
}

void USkillNumberBase::ActivateSkillRange_Round(float Range)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
	
	SkillRange_Circle = GetWorld()->SpawnActor<AAPSkillRange>(OwnerCharacter->GetAPSkillRange(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator + FRotator(90,0,0));
	if(!SkillRange_Circle.IsValid()) return;
	SkillRange_Circle->SetOwner(OwnerCharacter);
	SkillRange_Circle->GetDecalComponent()->DecalSize = FVector(15.0f, Range, Range);
	SkillRange_Circle->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);

	
}

void USkillNumberBase::ActivateSkillRange_Target(float Range_1, float Range_2, ESkillRangeType SkillRangeType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
	
	if(SkillRangeType == ESkillRangeType::Control_Circle)
	{
		SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(OwnerCharacter->GetAPSkillRange_Target(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	}
	else if(SkillRangeType == ESkillRangeType::Arrow)
	{
		SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(OwnerCharacter->GetAPSkillRange_Arrow(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		if(SkillRange_Target.IsValid()) SkillRange_Target->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	}
	else if(SkillRangeType == ESkillRangeType::Around_Circle)
	{
		SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(OwnerCharacter->GetAPSkillRange_Circle(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		if(SkillRange_Target.IsValid()) SkillRange_Target->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	}

	if(!SkillRange_Target.IsValid()) return;
	SkillRange_Target->SetOwner(OwnerCharacter);
	SkillRange_Target->GetDecalComponent()->DecalSize = FVector(5.0f, Range_1, Range_2);
	
}

void USkillNumberBase::CharacterRotation()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return; if(!SkillRange_Target.IsValid()) return;

	FVector Loc = SkillRange_Target->GetDecalComponent()->GetComponentLocation() - OwnerCharacter->GetMesh()->GetComponentLocation();

	Loc.Z = 0.0f; FRotator Rotation = FRotationMatrix::MakeFromX(Loc).Rotator();
	
	OwnerCharacter->SetActorRotation(Rotation);
}