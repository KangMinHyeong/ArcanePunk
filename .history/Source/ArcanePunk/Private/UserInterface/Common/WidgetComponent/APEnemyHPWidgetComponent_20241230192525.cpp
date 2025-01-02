
#include "UserInterface/Common/WidgetComponent/APEnemyHPWidgetComponent.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "UserInterface/Common/APShieldHPBar.h"

void UAPEnemyHPWidgetComponent::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(bActivate) UpdateHealthPercentage(InDeltaTime);
}

void UAPEnemyHPWidgetComponent::SetEnemy(AEnemy_CharacterBase *OwnerEnemy)
{
    Enemy = OwnerEnemy; if(!Enemy.IsValid()) return;
    UE_LOG(LogTemp, Display, TEXT("Your message"));
    bActivate = true;
    OriginHP = Enemy->GetDefaultHP(); OriginMaxHP = Enemy->GetDefaultMaxHP();
    
    HPPercent->SetPercent(OriginHP / OriginMaxHP);
    MaxHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginHP))));
    HP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginMaxHP))));

    Enemy->OnCheckingShield.AddDynamic(ShieldHPBar, &UAPShieldHPBar::CheckShieldHP);
    ShieldHPBar->CheckShieldHP(Enemy.Get(), 0.0f);
}

void UAPEnemyHPWidgetComponent::UpdateHealthPercentage(float InDeltaTime)
{
    if(Enemy.IsValid()) return;
    float TargetHP = Enemy->GetDefaultHP();
    
    OriginHP = FMath::FInterpConstantTo(OriginHP, TargetHP, InDeltaTime, BarSpeed);
    HPPercent->SetPercent(OriginHP / OriginMaxHP);
    HP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginHP))));

    if(OriginHP <= KINDA_SMALL_NUMBER && Enemy.IsValid()) Enemy->RemoveHPUI();
}
