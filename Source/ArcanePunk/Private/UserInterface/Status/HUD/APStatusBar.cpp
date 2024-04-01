
#include "UserInterface/Status/HUD/APStatusBar.h"

#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UserInterface/APHUD.h"
#include "Character/ArcanePunkCharacter.h"
#include "UserInterface/Status/HUD/APMPBar.h"
#include "Components/HorizontalBoxSlot.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Status/HUD/APSkillSlot.h"
#include "UserInterface/Status/HUD/APUltSkillSlot.h"

void UAPStatusBar::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitStatusBar();
    
}

void UAPStatusBar::NativeConstruct()
{
    Super::NativeConstruct();
}

void UAPStatusBar::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(bChange) UpdatePercentBar(InDeltaTime);
    
}

void UAPStatusBar::InitStatusBar()
{
    // 후에 게임 인스턴스로 치환? 안해도 될듯? 이미 캐릭터에서 게임 인스턴스로 업데이트하니까?
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
    auto PD = OwnerCharacter->GetPlayerStatus();
    HUD = Cast<AAPHUD>(GetOwningPlayer()->GetHUD()); if(!HUD.IsValid()) return;

    HPBar->SetPercent(PD.PlayerDynamicData.HP / PD.PlayerDynamicData.MaxHP); OriginHP = PD.PlayerDynamicData.HP;
    TEXT_CurrentMaxHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(PD.PlayerDynamicData.MaxHP))));
    TEXT_CurrentHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginHP))));

    for(int i = 0; i<PD.PlayerDynamicData.MaxMP; i++)
    {
        auto MP = CreateWidget<UAPMPBar>(this, MPBarClass);
        MPSpaces.Add(MP);
        
        auto Slots = MPBar->AddChildToHorizontalBox(MP);
        FSlateChildSize SlateChildSize = Slots->GetSize(); SlateChildSize.SizeRule = ESlateSizeRule::Fill;
        Slots->SetSize(SlateChildSize);
    }
    // MPBar->SetPercent(PD.PlayerDynamicData.MP / PD.PlayerDynamicData.MaxMP);

    HUD->OnUpdateHPBar.AddUObject(this, &UAPStatusBar::SetHPPercent);
    HUD->OnUpdateMPBar.AddUObject(this, &UAPStatusBar::UpdateMPBar);
    HUD->OnUpdateSkillSlot.AddUObject(this, &UAPStatusBar::UpdateSkillSlot);
    HUD->OnOperateSkill.AddUObject(this, &UAPStatusBar::OperateSkillSlot);
    HUD->OnHightLightSkill.AddUObject(this, &UAPStatusBar::HightLightSkillSlot);
    HUD->OnUsingSkill.AddUObject(this, &UAPStatusBar::UsingSkillSlot);
}

void UAPStatusBar::SetHPPercent(float Origin)
{
    if(!OwnerCharacter.IsValid()) return; auto PD = OwnerCharacter->GetPlayerStatus();

    OriginHP = Origin;

    if(OriginHP != PD.PlayerDynamicData.HP) bChange = true;
}

void UAPStatusBar::UpdatePercentBar(float InDeltaTime)
{
    if(!OwnerCharacter.IsValid()) return; auto PD = OwnerCharacter->GetPlayerStatus();
    if(OriginHP == PD.PlayerDynamicData.HP) {bChange = false; return;}

    OriginHP = FMath::FInterpTo(OriginHP, PD.PlayerDynamicData.HP, InDeltaTime, BarSpeed);

    HPBar->SetPercent(OriginHP / PD.PlayerDynamicData.MaxHP);
    TEXT_CurrentMaxHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(PD.PlayerDynamicData.MaxHP))));
    TEXT_CurrentHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginHP))));
    // MPBar->SetPercent(PD.PlayerDynamicData.MP / PD.PlayerDynamicData.MaxMP);
}

void UAPStatusBar::UpdateMPBar(uint8 Number, bool bUse)
{
    if(!OwnerCharacter.IsValid()) return; auto PD = OwnerCharacter->GetPlayerStatus();
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!PC) return;

    if(bUse)
    {
        for(int32 i = 0; i<Number; i++)
        {
            if(PD.PlayerDynamicData.MP > 0) {PD.PlayerDynamicData.MP--;}
            else{break;}

            if(LastNum == MPSpaces.Num()) break;
            MPSpaces.Last(LastNum)->UseMP();
            LastNum++;
        }
        OwnerCharacter->OnAutoRecoveryMPDelegate.Broadcast();
    }
    else
    {
        for(int32 i = 0; i<Number; i++)
        {
            if(PD.PlayerDynamicData.MP ==  PD.PlayerDynamicData.MaxMP) {break;}
            
            if(LastNum == 0) break;
            LastNum--;
            MPSpaces.Last(LastNum)->RecoveryMP();

            PD.PlayerDynamicData.MP++;
        }
    }
    OwnerCharacter->SetPlayerStatus(PD);
    PC->OnUpdateStatusText.Broadcast();
}

void UAPStatusBar::IncreaseMaxMP(uint8 Number)
{
    for(int i = 0; i<Number; i++)
    {
        auto MP = CreateWidget<UAPMPBar>(this, MPBarClass);
        MPSpaces.Add(MP);
        
        auto Slots = MPBar->AddChildToHorizontalBox(MP);
        FSlateChildSize SlateChildSize = Slots->GetSize(); SlateChildSize.SizeRule = ESlateSizeRule::Fill;
        Slots->SetSize(SlateChildSize);
    }
}

void UAPStatusBar::UpdateSkillSlot(ESkillKey SkillKey, uint8 SkillNumber)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        QSkillSlot->UpdateSkillSlot(SkillNumber);
        break;
    
        case ESkillKey::E:
        ESkillSlot->UpdateSkillSlot(SkillNumber);
        break;

        case ESkillKey::R:
        RSkillSlot->UpdateSkillSlot(SkillNumber);
        break;
    }
}

void UAPStatusBar::OperateSkillSlot(ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        QSkillSlot->OnOperating();
        break;
    
        case ESkillKey::E:
        ESkillSlot->OnOperating();
        break;

        case ESkillKey::R:
        RSkillSlot->OnOperating();
        RSkillSlot->StartSkillCoolTime();
        break;
    }
}

void UAPStatusBar::HightLightSkillSlot(ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        QSkillSlot->OnHightLight();
        break;
    
        case ESkillKey::E:
        ESkillSlot->OnHightLight();
        break;

        case ESkillKey::R:
        RSkillSlot->OnHightLight();
        break;
    }
}

void UAPStatusBar::UsingSkillSlot(ESkillKey SkillKey, bool NewBool)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        QSkillSlot->SetUsingSkill(NewBool);
        break;
    
        case ESkillKey::E:
        ESkillSlot->SetUsingSkill(NewBool);
        break;

        case ESkillKey::R:
        RSkillSlot->SetUsingSkill(NewBool);
        break;
    }
}
