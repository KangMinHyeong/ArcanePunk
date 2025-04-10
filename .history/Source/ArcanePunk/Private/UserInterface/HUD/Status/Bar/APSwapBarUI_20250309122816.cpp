#include "UserInterface/HUD/Status/Bar/APSwapBarUI.h"

#include "UserInterface/HUD/Status/Bar/ChangeCharacterUI.h"

void UAPSwapBarUI::Init(TArray<uint8> UI_ID)
{
    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI.IsValid()) return; 
    CharacterUIData = DataTableGI->GetCharacterUIDataTable();
    auto Data = CharacterUIData->FindRow<FCharaterUIData>(FName(*FString::FromInt(Dialogues[Message_Sort].Character_ID)), TEXT("None")); 

    uint8 KeyNum = 1;

    for(auto UI : UI_ID)
    {
        SwapUI = CreateWidget<UChangeCharacterUI>(GetWorld(), SwapUIClass);
        ConversationPortrait->SetBrushFromTexture();
        
        SwapUI->Init(KeyNum, );
        SwapUIs.Emplace(SwapUI, CharacterUIData.Portraits[1]);

        KeyNum++;
    }
}

void UAPSwapBarUI::Active(int32 PlayerID)
{
    // switch (PlayerID)
    // {
    // case 0:
    //     SwapUI_1->ChangePlayer(true);
    //     break;
    
    // case 1:
    //     SwapUI_2->ChangePlayer(true);
    //     break;

    // case 2:
    //     SwapUI_3->ChangePlayer(true);
    //     break;
    // }
}

void UAPSwapBarUI::Deactive(int32 PlayerID)
{
    // switch (PlayerID)
    // {
    // case 0:
    //     SwapUI_1->ChangePlayer(false);
    //     break;
    
    // case 1:
    //     SwapUI_2->ChangePlayer(false);
    //     break;

    // case 2:
    //     SwapUI_3->ChangePlayer(false);
    //     break;
    // }
}
