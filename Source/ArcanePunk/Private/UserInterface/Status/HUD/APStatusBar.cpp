
#include "UserInterface/Status/HUD/APStatusBar.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "UserInterface/APHUD.h"
#include "UserInterface/Status/HUD/APHPBar.h"
#include "UserInterface/Status/HUD/APMPBar.h"
#include "Components/HorizontalBoxSlot.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Status/HUD/APSkillSlot.h"
#include "UserInterface/Status/HUD/APUltSkillSlot.h"
#include "UserInterface/Status/HUD/APSkillChargingGauge.h"

void UAPStatusBar::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitStatusBar();
    
}

void UAPStatusBar::NativeConstruct()
{
    Super::NativeConstruct();
}

void UAPStatusBar::InitStatusBar()
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
    const auto PD = OwnerCharacter->GetPlayerStatus();
    HUD = Cast<AAPHUD>(GetOwningPlayer()->GetHUD()); if(!HUD.IsValid()) return;

    // 후에 게임 인스턴스로 치환? 안해도 될듯? 이미 캐릭터에서 게임 인스턴스로 업데이트하니까?
    for(int i = 0; i<PD.PlayerDynamicData.MaxMP; i++)
    {
        auto MP = CreateWidget<UAPMPBar>(this, MPBarClass);
        if(MP)
        {
            MPSpaces.Add(MP);
            Remain_MPBars.Add(MP);
        } 
        
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
    HUD->OnStartCoolTime.AddUObject(this, &UAPStatusBar::StartCoolTimeSlot);
    HUD->OnChargeTime.AddUObject(this, &UAPStatusBar::CheckChargeTime); 
    HUD->OnChargingGauge.AddUObject(this, &UAPStatusBar::ChargeGauge);
    HUD->OnChargingEnd.AddUObject(this, &UAPStatusBar::ChargeEnd);
}

void UAPStatusBar::SetHPPercent(float Origin)
{
    APHPBar->SetHPPercent(Origin);
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

            Remain_MPBars.Top()->FadeOut();
            Remain_MPBars.Pop();
        }
        OwnerCharacter->OnAutoRecoveryMPDelegate.Broadcast();
    }
    else
    {
        for(int32 i = 0; i<Number; i++)
        {
            if(PD.PlayerDynamicData.MP ==  PD.PlayerDynamicData.MaxMP) {break;}
            else {PD.PlayerDynamicData.MP++;}

            Remain_MPBars.Add(MPSpaces[Remain_MPBars.Num()]);
            Remain_MPBars.Top()->FadeIn();
        }
    }
    OwnerCharacter->SetDefaultMP(PD.PlayerDynamicData.MP);
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
        QSkillSlot->UpdateSkillSlot(SkillKey, SkillNumber);
        break;
    
        case ESkillKey::E:
        ESkillSlot->UpdateSkillSlot(SkillKey, SkillNumber);
        break;

        case ESkillKey::R:
        RSkillSlot->UpdateSkillSlot(SkillKey, SkillNumber);
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
        break;
    }
}

void UAPStatusBar::StartCoolTimeSlot(ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        QSkillSlot->StartSkillCoolTime(SkillKey);
        break;
    
        case ESkillKey::E:
        ESkillSlot->StartSkillCoolTime(SkillKey);
        break;

        case ESkillKey::R:
        RSkillSlot->StartSkillCoolTime(SkillKey);
        break;
    }
}

void UAPStatusBar::CheckChargeTime(ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        QSkillSlot->CheckChargeTime(SkillKey);
        break;
    
        case ESkillKey::E:
        ESkillSlot->CheckChargeTime(SkillKey);
        break;

        case ESkillKey::R:
        RSkillSlot->CheckChargeTime(SkillKey);
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

void UAPStatusBar::ChargeGauge(float Currnet, float Max)
{
    SkillChargingGauge->SetSkillChargingPercent(Currnet, Max);
}

void UAPStatusBar::ChargeEnd(bool Start)
{
    SkillChargingGauge->ChargeEnd(Start);
}



