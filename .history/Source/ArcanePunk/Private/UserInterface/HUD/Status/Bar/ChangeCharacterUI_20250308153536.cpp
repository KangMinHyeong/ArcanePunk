
#include "UserInterface/HUD/Status/Bar/ChangeCharacterUI.h"

#include "Components/Border.h"
#include "Components/Image.h"

void UChangeCharacterUI::NativeConstruct()
{
}

void UChangeCharacterUI::ChangePlayer()
{
    CurrentPlayerRing->SetVisibility(ESlateVisibility::Visible);
}