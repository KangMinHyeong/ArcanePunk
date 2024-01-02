
#include "UserInterface/Enemy/APEnemyHP.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Enemy/Enemy_CharacterBase.h"


void UAPEnemyHP::SetEnemy(AEnemy_CharacterBase *OwnerEnemy)
{
    Enemy = OwnerEnemy;

    if(Enemy) Enemy->OnEnemyHPChanged.AddUObject(this, &UAPEnemyHP::GetHealthPercentage);
}

void UAPEnemyHP::GetHealthPercentage()
{
    if(Enemy)
    {
        HPPercent->SetPercent(Enemy->GetMonsterHP() / Enemy->GetMonsterMaxHP());
        MaxHP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Enemy->GetMonsterMaxHP()))));
        HP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(Enemy->GetMonsterHP()))));
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Your asda"));
    }
}
