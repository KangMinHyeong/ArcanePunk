
#include "UserInterface/Status/HUD/APSkillSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"

void UAPSkillSlot::NativeConstruct()
{
    Super::NativeConstruct();
    Text_SkillKey->SetText(FText::FromString(SkillKey));

}

void UAPSkillSlot::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick( MyGeometry, InDeltaTime);

    if(!bStartPercent) return;
    SetCoolTimePercent();
    SetCoolTimeText();
    SetChargeTimePercent();
}

void UAPSkillSlot::UpdateSkillSlot(ESkillKey UpdateSkillKey, uint8 SkillNumber)
{
    auto DataTable = SkillSlotDataTable->FindRow<FSkillSlotDataTable>(Name, Name.ToString()); if(!DataTable){return;}

    SkillImage->SetBrushFromTexture(DataTable->SkillSlotImage[SkillNumber]); 

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());if(!OwnerCharacter.IsValid()) return;

    CurrentCoolTime = 0.0f;
    CurrentChargeTime = 0.0f;
    CurrentSkillKey = UpdateSkillKey;
    SetChargeTimeText();
    bStartPercent = true;
}

void UAPSkillSlot::SetUsingSkill(bool NewBool)
{
    if(NewBool)
    {
        SkillUsingImage->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        SkillUsingImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UAPSkillSlot::SetCoolTimePercent()
{
    float CoolTime = GetCurrentCoolTime() / SkillCoolTime;
    if(CoolTime > 0.001f) {CoolTimePercent->SetVisibility(ESlateVisibility::Visible);}
    else {CoolTimePercent->SetVisibility(ESlateVisibility::Hidden);}

    CoolTimePercent->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), CoolTime);
}

void UAPSkillSlot::SetCoolTimeText()
{   
    float CoolTime = GetCurrentCoolTime();
    if(CoolTime > 0.001f) {CoolTimeText->SetVisibility(ESlateVisibility::Visible);}
    else {CoolTimeText->SetVisibility(ESlateVisibility::Hidden);}

    CoolTime = SkillCoolTime - CoolTime;
    if(CoolTime > 1.0f) {CoolTimeText->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CoolTime))));}
    else {CoolTimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CoolTime)));}
}

void UAPSkillSlot::SetChargeTimePercent()
{
    float CoolTime = GetCurrentChargeTime() / ChargeTime;;
    if(CoolTime > 0.001f) {ChargeTimePercent->SetVisibility(ESlateVisibility::Visible);}
    else {ChargeTimePercent->SetVisibility(ESlateVisibility::Hidden);}    

    ChargeTimePercent->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), CoolTime);
}

void UAPSkillSlot::SetChargeTimeText()
{   
    if(!OwnerCharacter.IsValid()) return;

    uint8 Current = 0;
    uint8 Max = 0;
    switch (CurrentSkillKey)
    {
        case ESkillKey::Q:
        Current = OwnerCharacter->GetQSkillNumber()->GetCurrentChargeNum();
        Max = OwnerCharacter->GetQSkillNumber()->GetMaxChargeNum();
        break;
    
        case ESkillKey::E:
        Current = OwnerCharacter->GetESkillNumber()->GetCurrentChargeNum();
        Max = OwnerCharacter->GetESkillNumber()->GetMaxChargeNum();
        break;

        case ESkillKey::R:
        Current = OwnerCharacter->GetRSkillNumber()->GetCurrentChargeNum();
        Max = OwnerCharacter->GetRSkillNumber()->GetMaxChargeNum();
        break;
    }

    if(Max != 0) {ChargeTimeText->SetVisibility(ESlateVisibility::Visible);}
    else {ChargeTimeText->SetVisibility(ESlateVisibility::Hidden);}

    ChargeTimeText->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Current))));
}

void UAPSkillSlot::StartSkillCoolTime(ESkillKey UpdateSkillKey)
{
    if(!OwnerCharacter.IsValid()) return;
    SetSkillLimit(false);
    switch (UpdateSkillKey)
    {
        case ESkillKey::Q:
        CurrentCoolTime = OwnerCharacter->GetQSkillNumber()->GetCoolTime();
        break;
    
        case ESkillKey::E:
        CurrentCoolTime = OwnerCharacter->GetESkillNumber()->GetCoolTime();
        break;

        case ESkillKey::R:
        CurrentCoolTime = OwnerCharacter->GetRSkillNumber()->GetCoolTime();
        break;
    }
    SkillCoolTime = CurrentCoolTime;
    GetWorld()->GetTimerManager().SetTimer(SkillCoolTimerHandle, this, &UAPSkillSlot::SkillCoolDown, CurrentCoolTime, false);
}

void UAPSkillSlot::CheckChargeTime(ESkillKey UpdateSkillKey)
{
    if(!OwnerCharacter.IsValid()) return;
    
    CurrentSkillKey = UpdateSkillKey;
    // SetSkillLimit(false);
    switch (CurrentSkillKey)
    {
        case ESkillKey::Q:
        if(OwnerCharacter->GetQSkillNumber()->GetCurrentChargeNum() < OwnerCharacter->GetQSkillNumber()->GetMaxChargeNum())
        {
            CurrentChargeTime = OwnerCharacter->GetQSkillNumber()->GetCoolTime();
        }
        break;
    
        case ESkillKey::E:
        if(OwnerCharacter->GetESkillNumber()->GetCurrentChargeNum() < OwnerCharacter->GetESkillNumber()->GetMaxChargeNum())
        {
            CurrentChargeTime = OwnerCharacter->GetESkillNumber()->GetCoolTime();
        }
        break;

        case ESkillKey::R:
        if(OwnerCharacter->GetRSkillNumber()->GetCurrentChargeNum() < OwnerCharacter->GetRSkillNumber()->GetMaxChargeNum())
        {
            CurrentChargeTime = OwnerCharacter->GetRSkillNumber()->GetCoolTime();
        }
        break;
    }
    ChargeTime = CurrentChargeTime;
    SetChargeTimeText();

    if(bAlreadyCharging) return;
    bAlreadyCharging = true;
    
    GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UAPSkillSlot::ChargeCoolDown, CurrentChargeTime, false);
}

void UAPSkillSlot::SkillCoolDown()
{
    GetWorld()->GetTimerManager().ClearTimer(SkillCoolTimerHandle);
    if(!OwnerCharacter.IsValid()) return;
    SetSkillLimit(true);
    CurrentCoolTime = 0.0f;
    OnCoolDown();
}

void UAPSkillSlot::ChargeCoolDown()
{
    GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle); 

    uint8 CurrentChargeNum = 0;
    uint8 MaxChargeNum = 0;
    switch (CurrentSkillKey)
    {
        case ESkillKey::Q:
        CurrentChargeNum = OwnerCharacter->GetQSkillNumber()->GetCurrentChargeNum();
        MaxChargeNum = OwnerCharacter->GetQSkillNumber()->GetMaxChargeNum();
        if(CurrentChargeNum == MaxChargeNum) {break;}
        else {OwnerCharacter->GetQSkillNumber()->SetCurrentChargeNum(++CurrentChargeNum);}
        CurrentChargeTime = OwnerCharacter->GetQSkillNumber()->GetCoolTime();
        break;
    
        case ESkillKey::E:
        CurrentChargeNum = OwnerCharacter->GetESkillNumber()->GetCurrentChargeNum();
        MaxChargeNum = OwnerCharacter->GetESkillNumber()->GetMaxChargeNum();
        if(CurrentChargeNum == MaxChargeNum) {break;}
        else {OwnerCharacter->GetESkillNumber()->SetCurrentChargeNum(++CurrentChargeNum);}
        CurrentChargeTime = OwnerCharacter->GetESkillNumber()->GetCoolTime();
        break;

        case ESkillKey::R:
        CurrentChargeNum = OwnerCharacter->GetRSkillNumber()->GetCurrentChargeNum();
        MaxChargeNum = OwnerCharacter->GetRSkillNumber()->GetMaxChargeNum();
        if(CurrentChargeNum == MaxChargeNum) {break;}
        else {OwnerCharacter->GetRSkillNumber()->SetCurrentChargeNum(++CurrentChargeNum);}
        CurrentChargeTime = OwnerCharacter->GetRSkillNumber()->GetCoolTime();
        break;
    }
    if(CurrentChargeNum == MaxChargeNum) {CurrentChargeTime = 0.0f; bAlreadyCharging = false;}
    else
    {
        ChargeTime = CurrentChargeTime;
        GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UAPSkillSlot::ChargeCoolDown, CurrentChargeTime, false);
    }
    SetChargeTimeText();
}

float UAPSkillSlot::GetCurrentCoolTime()
{
	float CoolTime = CurrentCoolTime - (GetWorld()->GetTimerManager().GetTimerRate(SkillCoolTimerHandle) - GetWorld()->GetTimerManager().GetTimerElapsed(SkillCoolTimerHandle));
    return CoolTime;
}

float UAPSkillSlot::GetCurrentChargeTime()
{
	float CoolTime = CurrentChargeTime - (GetWorld()->GetTimerManager().GetTimerRate(ChargeTimerHandle) - GetWorld()->GetTimerManager().GetTimerElapsed(ChargeTimerHandle));
    return CoolTime;
}

void UAPSkillSlot::SetSkillLimit(bool NewBool)
{
    switch (CurrentSkillKey)
    {
        case ESkillKey::Q:
        OwnerCharacter->SetbCanSkill_Q(NewBool);
        break;
    
        case ESkillKey::E:
        OwnerCharacter->SetbCanSkill_E(NewBool);
        break;

        case ESkillKey::R:
        OwnerCharacter->SetbCanSkill_R(NewBool);
        if(NewBool) OwnerCharacter->GetRSkillNumber()->OnCoolDown();
        break;
    }
}


