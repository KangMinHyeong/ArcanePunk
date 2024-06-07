
#include "UserInterface/Enemy/APEnemyHP.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Enemy/Enemy_CharacterBase.h"


void UAPEnemyHP::SetEnemy(AEnemy_CharacterBase *OwnerEnemy)
{
    Enemy = OwnerEnemy;

    if(Enemy.IsValid()) Enemy->OnEnemyHPChanged.AddUObject(this, &UAPEnemyHP::GetHealthPercentage);
}

void UAPEnemyHP::GetHealthPercentage()
{
    if(Enemy.IsValid())
    {
        HPPercent->SetPercent(Enemy->GetDefaultHP() / Enemy->GetDefaultMaxHP());
        MaxHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Enemy->GetDefaultMaxHP()))));
        HP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Enemy->GetDefaultHP()))));
    }
}
