
#include "UserInterface/Skill/APMouseClickBase.h"

#include "Components/Character/APSkillHubComponent.h"

void UAPMouseClickBase::NativeConstruct()
{
    Super::NativeConstruct();
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter) return;
    OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerCharacterPC) return;
}

FReply UAPMouseClickBase::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
        // 스킬 취소
        SkillCancel();
	}
    if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
        SkillPlay();
	}
    
    RemoveFromParent();
    
    return Reply.Handled();
}

void UAPMouseClickBase::InputSkillInfo(ESkillNumber UpdateSkillNumber)
{
    SkillNumber = UpdateSkillNumber;
}

void UAPMouseClickBase::SkillCancel()
{
    switch (SkillNumber)
    {
        case ESkillNumber::Skill_3:
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber3()->Remove_Skill();
        break;
        case ESkillNumber::Skill_4:
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber4()->Remove_Skill();
        break;
        case ESkillNumber::Skill_7:
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber7()->Remove_Skill();
        break;
    }
}

void UAPMouseClickBase::SkillPlay()
{
    switch (SkillNumber)
    {
        case ESkillNumber::Skill_3:
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber3()->OnSkill();
        break;
        case ESkillNumber::Skill_4:
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber4()->OnSkill();
        break;
        case ESkillNumber::Skill_7:
        OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber7()->OnSkill();
        break;
    }
}
