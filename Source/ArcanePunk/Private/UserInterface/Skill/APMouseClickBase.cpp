
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
    
    if(!bLoop) RemoveFromParent();
    
    return Reply.Handled();
}

void UAPMouseClickBase::InputSkillInfo(ESkillNumber UpdateSkillNumber, bool Loop)
{
    UltMode = false; bLoop = Loop;
    SkillNumber = UpdateSkillNumber;
}

void UAPMouseClickBase::InputSkillInfo_Ult(bool Loop)
{
    UltMode = true; bLoop = Loop;
}

void UAPMouseClickBase::SkillCancel()
{
    if(UltMode)
    {
        OwnerCharacter->GetRSkillNumber()->Remove_Skill();
    }
    else
    {
        TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(SkillNumber);
        if(SkillNum.IsValid())
        {
            SkillNum->Remove_Skill();
        } 
    }
    
}

void UAPMouseClickBase::SkillPlay()
{
    if(UltMode)
    {
        OwnerCharacter->GetRSkillNumber()->OnSkill();
    }
    else
    {
        TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(SkillNumber);
        if(SkillNum.IsValid())
        {
            SkillNum->OnSkill();
        }
    }
}
