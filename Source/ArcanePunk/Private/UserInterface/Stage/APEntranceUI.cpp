
#include "UserInterface/Stage/APEntranceUI.h"

#include "Components/TextBlock.h"
#include "GameInstance/APGameInstance.h"
#include "Interfaces/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"

void UAPEntranceUI::NativeConstruct()
{
    Super::NativeConstruct();

    auto Name = UGameplayStatics::GetCurrentLevelName(GetWorld());
    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  
    auto DataTable = APGI->GetStringData()->FindRow<FStringDataTable>(FName(*Name), Name); 
    
    if(DataTable) {TEXT_LevelName->SetText(FText::FromString(DataTable->Content));}
    else {TEXT_LevelName->SetText(FText::FromString(Name));}
}