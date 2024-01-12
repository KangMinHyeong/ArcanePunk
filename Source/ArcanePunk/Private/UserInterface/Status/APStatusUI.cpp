#include "UserInterface/Status/APStatusUI.h"

#include "Character/ArcanePunkCharacter.h"
#include "Items/APItemBase.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Components/Overlay.h"

void UAPStatusUI::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitEquipSlot();
}

void UAPStatusUI::NativeConstruct() 
{
    Super::NativeConstruct();

    InitEquipSlot();
}

void UAPStatusUI::InitEquipSlot()
{
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());
    if(!OwnerCharacter) return;

    if(WeaponSlot) WeaponSlot->RemoveFromParent();
    
    if(OwnerCharacter->GetEquipData(1) && InventorySlotClass)
    { 
        WeaponSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
        WeaponSlot->SetItemReference(OwnerCharacter->GetEquipData(1));
        WeaponOverlay->AddChildToOverlay(WeaponSlot);
        WeaponSlot->SetInInventory(false);
        // WeaponBorder->AddChildto(WeaponSlot);
    }
}
