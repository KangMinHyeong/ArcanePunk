#include "UserInterface/HUD/Status/Bar/APSwapBarUI.h"

#include "UserInterface/HUD/Status/Bar/ChangeCharacterUI.h"
#include "GameInstance/APGameInstance.h"
#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/WrapBox.h"

void UAPSwapBarUI::Init(TArray<uint8> UI_ID)
{
    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return; 
    
    uint8 KeyNum = 1;

    for(auto UI : UI_ID)
    {
        UE_LOG(LogTemp, Display, TEXT("Your KeyNum %d"), UI);
        auto SwapUI = CreateWidget<UChangeCharacterUI>(GetWorld(), SwapUIClass);

        auto CharacterUIData = DataTableGI->GetCharacterUIDataTable()->FindRow<FCharaterUIData>(FName(*FString::FromInt(UI)), TEXT("None")); 
        if(CharacterUIData)
        {
            SwapUI->Init(KeyNum, CharacterUIData->Portraits[1]);
            SwapUIs.Emplace(SwapUI);
            WrapBox->AddChild(SwapUI);
        }
        
        KeyNum++;
    }
}

void UAPSwapBarUI::Active(int32 PlayerID)
{
    SwapUIs[PlayerID]->ChangePlayer(true);
    VinetteActive();
}

void UAPSwapBarUI::Deactive(int32 PlayerID, float Gauge)
{
    SwapUIs[PlayerID]->ChangePlayer(false);
    SwapUIs[PlayerID]->StartSwapCoolDown(SwapCoolTime); 
}

bool UAPSwapBarUI::CheckSwapCoolDown(uint8 PlayerID)
{
    return SwapUIs[PlayerID]->CheckSwapCoolDown();
}

void UAPSwapBarUI::UpdateSwapGauge(uint8 PlayerID, float Add)
{
    return SwapUIs[PlayerID]->AddSwapGauge(Add); 
}
