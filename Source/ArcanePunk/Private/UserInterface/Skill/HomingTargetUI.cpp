
#include "UserInterface/Skill/HomingTargetUI.h"

#include "Components/Character/APSkillHubComponent.h"

void UHomingTargetUI::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter) return;
    OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerCharacterPC) return;
}

FReply UHomingTargetUI::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    if(!OwnerCharacterPC) return Reply.Unhandled(); if(!OwnerCharacter) return Reply.Unhandled();

    if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumberBase()->SkillCancel();
        OwnerCharacterPC->ReturnToDefault();
        RemoveFromParent();
		return Reply.Handled();
	}
    if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
        FHitResult HitResult;
        OwnerCharacterPC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

        OwnerCharacter->SetHomingPoint(HitResult.Location);

        HomingSkill();
        OwnerCharacterPC->ReturnToDefault();
        RemoveFromParent();
		return Reply.Handled();
	}

    OwnerCharacterPC->ReturnToDefault();
    return Reply.Unhandled();
}

void UHomingTargetUI::InputSkillInfo(ESkillNumber UpdateSkillNumber)
{
    SkillNumber = UpdateSkillNumber;
}

void UHomingTargetUI::HomingSkill()
{
    switch (SkillNumber)
    {
        case ESkillNumber::Skill_1:
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber1()->OnSkill();
        break;
    
        case ESkillNumber::Skill_2:
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber2()->OnSkill();
        break;

        case ESkillNumber::Skill_4:
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber4()->OnSkill();
        break;
    }
}
