
#include "UserInterface/Enemy/APEnemyHP.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "UserInterface/Common/APShieldHPBar.h"

void UAPEnemyHP::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(bActivate) UpdateHealthPercentage(InDeltaTime);
}

void UAPEnemyHP::SetEnemy(AEnemy_CharacterBase *OwnerEnemy)
{
    Enemy = OwnerEnemy;
    if(!Enemy.IsValid()) return;

    bActivate = true;
    OriginHP = Enemy->GetDefaultHP();
    OriginMaxHP = Enemy->GetDefaultMaxHP();
    
    HPPercent->SetPercent(OriginHP / OriginMaxHP);
    MaxHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginHP))));
    HP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginMaxHP))));

    Enemy->OnCheckingShield.AddDynamic(ShieldHPBar, &UAPShieldHPBar::CheckShieldHP);
    ShieldHPBar->CheckShieldHP(Enemy.Get());
}

void UAPEnemyHP::UpdateHealthPercentage(float InDeltaTime)
{
    float TargetHP = 0.0f;
    if(Enemy.IsValid()) TargetHP = Enemy->GetDefaultHP();
    
    OriginHP = FMath::FInterpConstantTo(OriginHP, TargetHP, InDeltaTime, BarSpeed);
    HPPercent->SetPercent(OriginHP / OriginMaxHP);
    HP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OriginHP))));

    if(OriginHP <= KINDA_SMALL_NUMBER && Enemy.IsValid()) Enemy->RemoveHPUI();
}
