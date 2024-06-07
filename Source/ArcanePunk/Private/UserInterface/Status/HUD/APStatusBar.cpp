
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
    // for(int32 i = 0; i<PD.PlayerDynamicData.MaxMP; i++)
    // {
    //     auto MP = CreateWidget<UAPMPBar>(this, MPBarClass);
    //     if(MP)
    //     {
    //         MPSpaces.Add(MP);
    //         Remain_MPBars.Add(MP);
    //     } 
        
    //     auto Slots = MPBar->AddChildToHorizontalBox(MP);
    //     FSlateChildSize SlateChildSize = Slots->GetSize(); // SlateChildSize.SizeRule = ESlateSizeRule::Fill;
    //     Slots->SetSize(SlateChildSize);
    // }
    AddMaxMP(PD.PlayerDynamicData.MaxMP);
    // MPBar->SetPercent(PD.PlayerDynamicData.MP / PD.PlayerDynamicData.MaxMP);

    HUD->OnUpdateMaxHPBar.AddUObject(this, &UAPStatusBar::SetMaxHP);
    HUD->OnUpdateHPBar.AddUObject(this, &UAPStatusBar::SetHPPercent);
    HUD->OnUpdateMaxMPBar.AddUObject(this, &UAPStatusBar::AddMaxMP);
    HUD->OnUpdateMPBar.AddUObject(this, &UAPStatusBar::UpdateMPBar);
    HUD->OnUpdateSkillSlot.AddUObject(this, &UAPStatusBar::UpdateSkillSlot);
    HUD->OnOperateSkill.AddUObject(this, &UAPStatusBar::OperateSkillSlot);
    HUD->OnHightLightSkill.AddUObject(this, &UAPStatusBar::HightLightSkillSlot);
    HUD->OnUsingSkill.AddUObject(this, &UAPStatusBar::UsingSkillSlot);
    HUD->OnStartCoolTime.AddUObject(this, &UAPStatusBar::StartCoolTimeSlot);

    HUD->OnChargeTime.AddUObject(this, &UAPStatusBar::CheckChargeTime); 
    HUD->OnOnlyChargingNumberChange.AddUObject(this, &UAPStatusBar::ChangeCharginNum); 

    HUD->OnChargingGauge.AddUObject(this, &UAPStatusBar::ChargeGauge);
    HUD->OnChargingEnd.AddUObject(this, &UAPStatusBar::ChargeEnd);
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

void UAPStatusBar::ChangeCharginNum(ESkillKey SkillKey)
{
    switch (SkillKey)
    {
        case ESkillKey::Q:
        QSkillSlot->SetChargeTimeText(SkillKey);
        break;
    
        case ESkillKey::E:
        ESkillSlot->SetChargeTimeText(SkillKey);
        break;

        case ESkillKey::R:
        RSkillSlot->SetChargeTimeText(SkillKey);
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



