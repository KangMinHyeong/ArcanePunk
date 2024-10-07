
#include "UserInterface/Stage/APStageSelectingUI.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "GameInstance/APGameInstance.h"

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

    Button_Chapter1->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Chapter2->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Chapter3->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Back->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
}

void UAPStageSelectingUI::OnClickButton_Chapter1()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    Switcher_StageSlot->SetActiveWidget(StageSlot_1);
}

void UAPStageSelectingUI::OnClickButton_Chapter2()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    Switcher_StageSlot->SetActiveWidget(StageSlot_2);
}

void UAPStageSelectingUI::OnClickButton_Chapter3()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    Switcher_StageSlot->SetActiveWidget(StageSlot_3);
}

void UAPStageSelectingUI::OnClickButton_Back()
{
    UAPSoundSubsystem::PlayUICloseSound(UAPGameInstance::GetSoundGI(GetWorld()));

    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!PC) return;
    PC->CloseStageSelectingUI();
    RemoveFromParent();
}
