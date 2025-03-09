#include "UserInterface/HUD/Status/Bar/APSwapBarUI.h"

#include "UserInterface/HUD/Status/Bar/ChangeCharacterUI.h"

void UAPSwapBarUI::Active(int32 PlayerID)
{
    switch (PlayerID)
    {
    case 0:
        SwapUI_1->ChangePlayer(true);
        break;
    
    case 1:
        SwapUI_2->ChangePlayer(true);
        break;

    case 2:
        SwapUI_3->ChangePlayer(true);
        break;
    }
}

void UAPSwapBarUI::Deactive(int32 PlayerID)
{
    switch (PlayerID)
    {
    case 0:
        SwapUI_1->ChangePlayer(false);
        break;
    
    case 1:
        SwapUI_2->ChangePlayer(false);
        break;

    case 2:
        SwapUI_3->ChangePlayer(false);
        break;
    }
}
