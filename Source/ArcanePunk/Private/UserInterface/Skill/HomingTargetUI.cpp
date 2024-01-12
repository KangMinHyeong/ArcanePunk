
#include "UserInterface/Skill/HomingTargetUI.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/Character/APSkillNumber.h"
#include "Components/Character/SkillNumber/SkillNumber1.h"
#include "Components/Character/SkillNumber/SkillNumber2.h"

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

    UE_LOG(LogTemp, Display, TEXT("Your a"));

    if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
        UE_LOG(LogTemp, Display, TEXT("Your b"));

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

void UHomingTargetUI::InputSkillInfo(uint8 UpdateSkillNumber, uint8 UpdateSkillType)
{
    SkillNumber = UpdateSkillNumber;
    SkillType = UpdateSkillType;
}

void UHomingTargetUI::HomingSkill()
{
    UE_LOG(LogTemp, Display, TEXT("Your c"));
    switch (SkillNumber)
    {
        case 1:
        OwnerCharacter->GetAPSkillNumberComponent()->GetSkillNumber1()->OnSkill(SkillType);
        break;
    
        case 2:
        OwnerCharacter->GetAPSkillNumberComponent()->GetSkillNumber2()->OnSkill(SkillType);
        break;
    }
}