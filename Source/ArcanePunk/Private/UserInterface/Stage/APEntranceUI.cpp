#include "UserInterface/Stage/APEntranceUI.h"

#include "Components/TextBlock.h"
#include "GameInstance/APGameInstance.h"
#include "Interfaces/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "DataStructs/Common/FStringDataTable.h"

void UAPEntranceUI::NativeConstruct()
{
    Super::NativeConstruct();

    auto Name = UGameplayStatics::GetCurrentLevelName(GetWorld());
    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;     
    auto DataTable = DataTableGI->GetRowByStruct<FStringDataTable>(FName(*Name), Name); 
    
    if(DataTable) {TEXT_LevelName->SetText(FText::FromString(DataTable->Content));}
    else {TEXT_LevelName->SetText(FText::FromString(Name));}
}