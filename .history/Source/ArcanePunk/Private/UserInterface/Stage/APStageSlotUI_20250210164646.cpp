
#include "UserInterface/Stage/APStageSlotUI.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "GameInstance/APGameInstance.h"

void UAPStageSlotUI::NativeConstruct()
{
    Super::NativeConstruct();

    TEXT_StageNumber->SetText(FText::FromString(FString::FromInt(StageNumber)));
    TEXT_SubStageNumber->SetText(FText::FromString(FString::FromInt(SubStageNumber)));

    CheckLock();
}

FReply UAPStageSlotUI::NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
    FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return Reply.Handled();

    if(!GI->GetCanEnterStage().Contains(BattleStageLevelName)) return Reply.Handled();

    if(GI->GetCanEnterStage()[BattleStageLevelName])
    {   
        GI->OpenLevel(GetWorld(), BattleStageLevelName);
    }

	return Reply.Handled();
}

void UAPStageSlotUI::CheckLock()
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    if(!GI->GetCanEnterStage().Contains(BattleStageLevelName)) return;

    if(GI->GetCanEnterStage()[BattleStageLevelName])
    {   
        Switcher_CanEnter->SetActiveWidget(Unlocked);
    }
    else
    {
        Switcher_CanEnter->SetActiveWidget(Locked);
    }
}
