
#include "UserInterface/Stage/APEntranceUI.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UAPEntranceUI::NativeConstruct()
{
    Super::NativeConstruct();

    TEXT_LevelName->SetText(FText::FromString(UGameplayStatics::GetCurrentLevelName(GetWorld())));
}