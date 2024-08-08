
#include "UserInterface/Inform/Stage/APStageInformationUI.h"

#include "GameMode/APGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UAPStageInformationUI::NativeConstruct()
{
    Super::NativeConstruct();

    UpdateRemainMonsterNumber();
}

void UAPStageInformationUI::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateRemainMonsterNumber();
}

void UAPStageInformationUI::UpdateRemainMonsterNumber()
{
    auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); if(!GM) return;
    auto Information = GM->GetBattleStageInformation();

    Text_StageTitle->SetText(FText::FromString(Information.StageTitle));
    Text_ChapterNumber->SetText(FText::FromString(FString::FromInt(Information.ChapterNumber)));
    Text_StageClearCondition->SetText(FText::FromString(Information.StageClearCondition));
    Text_RemainMonsterNumber->SetText(FText::FromString(FString::FromInt(GM->GetRemainMonsterNumber())));
    Text_TotalMonsterNumber->SetText(FText::FromString(FString::FromInt(GM->GetTotalMonsterNumber())));
}

