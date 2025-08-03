#include "UserInterface/Stage/APEntranceUI.h"

#include "Components/TextBlock.h"
#include "GameInstance/APGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DataStructs/Common/FLocalizationData.h"

void UAPEntranceUI::NativeConstruct()
{
    Super::NativeConstruct();

    auto Name = UGameplayStatics::GetCurrentLevelName(GetWorld());
    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;     
    auto DataTable = DataTableGI->GetRowByStruct<FLocalizationData>(FName(*Name), Name); 
    
    if(DataTable) {TEXT_LevelName->SetText(FText::FromString(DataTable->Content));}
    else {TEXT_LevelName->SetText(FText::FromString(Name));}
}