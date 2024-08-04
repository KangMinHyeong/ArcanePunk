
#include "UserInterface/Save/APSaveCompleteUI.h"

#include "Components/TextBlock.h"
#include "GameInstance/APGameInstance.h"
#include "Interfaces/InteractionInterface.h"

void UAPSaveCompleteUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    // auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  
    // auto DataTable = APGI->GetStringData()->FindRow<FStringDataTable>(SaveComplete, SaveComplete.ToString()); if(!DataTable) return;
    // Text_SaveComplete->SetText(FText::FromString(DataTable->Content));
}