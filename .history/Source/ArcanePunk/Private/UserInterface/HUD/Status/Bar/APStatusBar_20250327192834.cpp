
#include "UserInterface/HUD/Status/Bar/APStatusBar.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "UserInterface/HUD/APHUD.h"
#include "UserInterface/HUD/Status/Bar/APHPBar.h"
#include "UserInterface/HUD/Status/Bar/APMPBar.h"
#include "Components/HorizontalBoxSlot.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/HUD/Status/Bar/APSkillSlot.h"
#include "UserInterface/HUD/Status/Bar/APUltSkillSlot.h"
#include "UserInterface/HUD/Status/Bar/APSkillChargingGauge.h"
#include "UserInterface/HUD/Status/Bar/APDashSlot.h"

void UAPStatusBar::NativeOnInitialized()
{
    Super::NativeOnInitialized();    
}

void UAPStatusBar::NativeConstruct()
{
    Super::NativeConstruct();    
}

void UAPStatusBar::BindStatusBar(AArcanePunkCharacter* OwnerPlayer)
{
    OwnerCharacter = OwnerPlayer; if(!OwnerCharacter.IsValid()) return;
    
    const auto PD = OwnerCharacter->GetPlayerStatus();
    HUD = Cast<AAPHUD>(GetOwningPlayer()->GetHUD()); if(!HUD.IsValid()) return;

    AddMaxMP(PD.StatusData.MaxMP);
}

void UAPStatusBar::SetMaxHP(float NewValue)
{
    APHPBar->SetMaxHP(NewValue);
}

void UAPStatusBar::SetHPPercent(float Origin)
{
    APHPBar->SetHPPercent(Origin);
}

void UAPStatusBar::AddMaxMP(uint8 NewValue)
{
    MPBar->ClearChildren();
    
    for(int32 i = 0; i<NewValue; i++)
    {
        auto MP = CreateWidget<UAPMPBar>(this, MPBarClass);
        if(!MP) return;

        
        // MPSpaces.Emplace(MP);
        // Remain_MPBars.Emplace(MP);
        MPSpaces.Insert(MP, i);
        Remain_MPBars.Insert(MP, i);
        
        
        // auto Slots = Cast<UHorizontalBoxSlot>(MPBar->InsertChildAt( i, MP));  
        
    }

    for(auto MP : MPSpaces)
    {
        auto Slots = MPBar->AddChildToHorizontalBox(MP);  
        // MPBar->ShiftChild(0,MP);
        FSlateChildSize SlateChildSize = Slots->GetSize(); // SlateChildSize.SizeRule = ESlateSizeRule::Fill;
        Slots->SetSize(SlateChildSize);
    }
}

void UAPStatusBar::UpdateMPBar(uint8 Number, bool bUse)
{
    if(!OwnerCharacter.IsValid()) return; auto PD = OwnerCharacter->GetPlayerStatus();
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!PC) return;

    if(bUse)
    {
        for(int32 i = 0; i<Number; i++)
        {
            if(PD.StatusData.MP > 0) {PD.StatusData.MP--;}
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
            if(PD.StatusData.MP ==  PD.StatusData.MaxMP) {break;}
            else {PD.StatusData.MP++;}

            Remain_MPBars.Add(MPSpaces[Remain_MPBars.Num()]);
            Remain_MPBars.Top()->FadeIn();
        }
    }
    OwnerCharacter->SetDefaultMP(PD.StatusData.MP);
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

void UAPStatusBar::UpdateSkillSlot(AArcanePunkCharacter* Character, ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        Slot_QSkill->UpdateSkillSlot(Character, SkillKey);
        break;
    
        case ESkillKey::E:
        Slot_ESkill->UpdateSkillSlot(Character, SkillKey);
        break;

        case ESkillKey::R:
        Slot_RSkill->UpdateSkillSlot(Character, SkillKey);
        break;

        case ESkillKey::Dash:
        Slot_Dash->UpdateSkillSlot(Character, SkillKey);
        break;

        case ESkillKey::Parrying:
        Slot_Parrying->UpdateSkillSlot(Character, SkillKey);
        break;
    }
}

void UAPStatusBar::OperateSkillSlot(ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        Slot_QSkill->OnOperating();
        break;
    
        case ESkillKey::E:
        Slot_ESkill->OnOperating();
        break;

        case ESkillKey::R:
        Slot_RSkill->OnOperating();
        break;
    }
}

void UAPStatusBar::StartCoolTimeSlot(ESkillKey SkillKey, float CoolTime)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        Slot_QSkill->StartSkillCoolTime(SkillKey, CoolTime);
        break;
    
        case ESkillKey::E:
        Slot_ESkill->StartSkillCoolTime(SkillKey, CoolTime);
        break;

        case ESkillKey::R:
        Slot_RSkill->StartSkillCoolTime(SkillKey, CoolTime);
        break;

        case ESkillKey::Dash:
        Slot_Dash->StartSkillCoolTime(SkillKey, CoolTime);
        break;

        case ESkillKey::Parrying:
        Slot_Parrying->StartSkillCoolTime(SkillKey, CoolTime);
        break;
    }
}

void UAPStatusBar::UpdateSkillSlotImage(ESkillKey SkillKey, UTexture2D* SkillSlotImage)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        Slot_QSkill->UpdateSkillSlotImage(SkillSlotImage);
        break;
    
        case ESkillKey::E:
        Slot_ESkill->UpdateSkillSlotImage(SkillSlotImage);
        break;

        case ESkillKey::R:
        Slot_RSkill->UpdateSkillSlotImage(SkillSlotImage);
        break;
    }
}

void UAPStatusBar::CheckChargeTime(ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        Slot_QSkill->CheckChargeTime(SkillKey);
        break;
    
        case ESkillKey::E:
        Slot_ESkill->CheckChargeTime(SkillKey);
        break;

        case ESkillKey::R:
        Slot_RSkill->CheckChargeTime(SkillKey);
        break;
    }
}

void UAPStatusBar::ChangeCharginNum(ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        Slot_QSkill->SetChargeTimeText(SkillKey);
        break;
    
        case ESkillKey::E:
        Slot_ESkill->SetChargeTimeText(SkillKey);
        break;

        case ESkillKey::R:
        Slot_RSkill->SetChargeTimeText(SkillKey);
        break;
    }
}

void UAPStatusBar::HightLightSkillSlot(ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        Slot_QSkill->OnHightLight();
        break;
    
        case ESkillKey::E:
        Slot_ESkill->OnHightLight();
        break;

        case ESkillKey::R:
        Slot_RSkill->OnHightLight();
        break;
    }
}

void UAPStatusBar::UsingSkillSlot(ESkillKey SkillKey, bool NewBool)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        Slot_QSkill->SetUsingSkill(NewBool);
        break;
    
        case ESkillKey::E:
        Slot_ESkill->SetUsingSkill(NewBool);
        break;

        case ESkillKey::R:
        Slot_RSkill->SetUsingSkill(NewBool);
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

void UAPStatusBar::AddSkillCoolTime(ESkillKey SkillKey, float AddTime)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        Slot_QSkill->AddSkillCoolTime(AddTime);
        break;
    
        case ESkillKey::E:
        Slot_ESkill->AddSkillCoolTime(AddTime);
        break;

        case ESkillKey::R:
        Slot_RSkill->AddSkillCoolTime(AddTime);
        break;
    }
}

void UAPStatusBar::StartDashTime(float DashTime)
{
    Slot_Dash->StartDashTime(DashTime);
}

void UAPStatusBar::SetHiddenGuideBar(bool bVisible)
{
    if(bVisible)
    {
        GuideBar->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        GuideBar->SetVisibility(ESlateVisibility::Collapsed);
    }
}
