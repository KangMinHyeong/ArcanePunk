
#include "UserInterface/DisplayOnly/APDisplayOnlyUI.h"

#include "GameInstance/APGameInstance.h"
#include "Components/TextBlock.h"

void UAPDisplayOnlyUI::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  

    APGI->SetTextBlock_Name(TextBlock_1, Text_1);
}

void UAPDisplayOnlyUI::OnEndedUIAnim_Implementation()
{
}