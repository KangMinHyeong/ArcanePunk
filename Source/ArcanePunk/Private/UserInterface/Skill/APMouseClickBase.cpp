
#include "UserInterface/Skill/APMouseClickBase.h"

#include "Components/Character/APSkillHubComponent.h"

void UAPMouseClickBase::NativeConstruct()
{
    Super::NativeConstruct();
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
    OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerCharacterPC.IsValid()) return;
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
    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(SkillNumber);
	if(SkillNum.IsValid())
	{
		SkillNum->Remove_Skill();
	} 

    // switch (SkillNumber)
    // {
    //     case ESkillNumber::Skill_3:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber3()->Remove_Skill();
    //     break;
    //     case ESkillNumber::Skill_4:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber4()->Remove_Skill();
    //     break;
    //     case ESkillNumber::Skill_7:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber7()->Remove_Skill();
    //     break;
    //     case ESkillNumber::Skill_8:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber8()->Remove_Skill();
    //     break;
    //     case ESkillNumber::Skill_10:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber10()->Remove_Skill();
    //     break;
    //     case ESkillNumber::Skill_14:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber14()->Remove_Skill();
    //     break;
    // }
}

void UAPMouseClickBase::SkillPlay()
{
    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(SkillNumber);
	if(SkillNum.IsValid())
	{
		SkillNum->OnSkill();
	}

    // switch (SkillNumber)
    // {
    //     case ESkillNumber::Skill_3:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber3()->OnSkill();
    //     break;
    //     case ESkillNumber::Skill_4:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber4()->OnSkill();
    //     break;
    //     case ESkillNumber::Skill_7:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber7()->OnSkill();
    //     break;
    //     case ESkillNumber::Skill_8:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber8()->OnSkill();
    //     break;
    //     case ESkillNumber::Skill_10:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber10()->OnSkill();
    //     break;
    //     case ESkillNumber::Skill_14:
    //     OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber14()->OnSkill();
    //     break;
    // }
}
