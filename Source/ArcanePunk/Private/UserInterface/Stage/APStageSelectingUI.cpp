
#include "UserInterface/Stage/APStageSelectingUI.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void UAPStageSelectingUI::NativeConstruct()
{
    Super::NativeConstruct();
}

void UAPStageSelectingUI::BindButton()
{
    Button_Chapter1->OnClicked.AddDynamic(this, &UAPStageSelectingUI::OnClickButton_Chapter1);
    Button_Chapter2->OnClicked.AddDynamic(this, &UAPStageSelectingUI::OnClickButton_Chapter2);
    Button_Chapter3->OnClicked.AddDynamic(this, &UAPStageSelectingUI::OnClickButton_Chapter3);

    Button_Back->OnClicked.AddDynamic(this, &UAPStageSelectingUI::OnClickButton_Back);
}

void UAPStageSelectingUI::OnClickButton_Chapter1()
{
    Switcher_StageSlot->SetActiveWidget(StageSlot_1);
}

void UAPStageSelectingUI::OnClickButton_Chapter2()
{
    Switcher_StageSlot->SetActiveWidget(StageSlot_2);
}

void UAPStageSelectingUI::OnClickButton_Chapter3()
{
    Switcher_StageSlot->SetActiveWidget(StageSlot_3);
}

void UAPStageSelectingUI::OnClickButton_Back()
{
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!PC) return;
    PC->CloseStageSelectingUI();
    RemoveFromParent();
}
