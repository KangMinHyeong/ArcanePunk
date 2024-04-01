
#include "UserInterface/Status/HUD/APUltSkillSlot.h"

#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"

void UAPUltSkillSlot::NativeConstruct()
{
    Super::NativeConstruct();
}

void UAPUltSkillSlot::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick( MyGeometry, InDeltaTime);

    if(!bStartPercent) return;
    SetCoolTimePercent();
    SetCoolTimeText();
}

void UAPUltSkillSlot::UpdateSkillSlot(uint8 SkillNumber)
{
    Super::UpdateSkillSlot(SkillNumber);

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());if(!OwnerCharacter.IsValid()) return;

    CurrentCoolTime = 0.0f;
    SkillCoolTime = OwnerCharacter->GetRSkillNumber()->GetUltCoolTime();

    bStartPercent = true;
}

void UAPUltSkillSlot::SetCoolTimePercent()
{
    float CoolTime = GetCurrentCoolTime() / SkillCoolTime;
    if(CoolTime > 0.001f) {CoolTimePercent->SetVisibility(ESlateVisibility::Visible);}
    else {CoolTimePercent->SetVisibility(ESlateVisibility::Hidden);}

    CoolTimePercent->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), CoolTime);
}

void UAPUltSkillSlot::SetCoolTimeText()
{   
    float CoolTime = GetCurrentCoolTime();
    if(CoolTime > 0.001f) {CoolTimePercent->SetVisibility(ESlateVisibility::Visible);}
    else {CoolTimePercent->SetVisibility(ESlateVisibility::Hidden);}

    CoolTime = SkillCoolTime - CoolTime;
    if(CoolTime > 1.0f) {CoolTimeText->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CoolTime))));}
    else {CoolTimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CoolTime)));}
}

void UAPUltSkillSlot::StartSkillCoolTime()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->SetbCanSkill(false);
    CurrentCoolTime = SkillCoolTime;
    GetWorld()->GetTimerManager().SetTimer(SkillCoolTimerHandle, this, &UAPUltSkillSlot::SkillCoolDown, CurrentCoolTime, false);
}

void UAPUltSkillSlot::SkillCoolDown()
{
    GetWorld()->GetTimerManager().ClearTimer(SkillCoolTimerHandle);
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->SetbCanSkill(true);
    CurrentCoolTime = 0.0f;
    OnCoolDown();
}

float UAPUltSkillSlot::GetCurrentCoolTime()
{
	float CoolTime = CurrentCoolTime - (GetWorld()->GetTimerManager().GetTimerRate(SkillCoolTimerHandle) - GetWorld()->GetTimerManager().GetTimerElapsed(SkillCoolTimerHandle));
    return CoolTime;
}