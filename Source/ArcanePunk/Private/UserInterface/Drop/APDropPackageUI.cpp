
#include "UserInterface/Drop/APDropPackageUI.h"

#include "UserInterface/Common/APSlotSwitcher.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Overlay.h"
#include "Components/WrapBox.h"
#include "Character/ArcanePunkCharacter.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "Components/Button.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void UAPDropPackageUI::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void UAPDropPackageUI::NativeConstruct()
{
    Super::NativeConstruct();

    AddWidget();
    AddPackage();
    BindButton();
}

void UAPDropPackageUI::AddWidget()
{
    for(int32 i = 0; i<SlotNum/2; i++)
    {
        auto DropSlot = CreateWidget<UAPSlotSwitcher>(this, DropSlotSwitcher);
        TopWrapBox->AddChildToWrapBox(DropSlot);
        DropSlots.Add(DropSlot);
    }
    for(int32 i = SlotNum/2; i<SlotNum; i++)
    {
        auto DropSlot = CreateWidget<UAPSlotSwitcher>(this, DropSlotSwitcher);
        BottomWrapBox->AddChildToWrapBox(DropSlot);
        DropSlots.Add(DropSlot);
    }
}

void UAPDropPackageUI::AddPackage()
{
    if(DropSlots.Num() != SlotNum) { return;}

    for(int32 i = 0; i<ItemsInPackage.Num(); i++)
    {
        if(i >= DropSlots.Num()) return;

        auto DropItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
        if(DropItemSlot)
        {
            DropItemSlot->SetInInventory(ESlotType::DropPackage);
            DropItemSlot->SetItemReference(ItemsInPackage[i]);
            DropSlots[i]->Switcher_Overlay->AddChildToOverlay(DropItemSlot);
            SlotArr.Add(DropItemSlot);
        }
    }
}

FReply UAPDropPackageUI::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) // 클릭 아이템 인벤토리로 (오른쪽 마우스)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    return Reply.Handled();
}

FReply UAPDropPackageUI::NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) // 전부 인벤토리로 (F키)
{
    FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    UE_LOG(LogTemp, Display, TEXT("Your message"));
    // 후에 인벤토리 제한사항에서 못넣는 상황 코드 추가
    if(InKeyEvent.GetKey() == EKeys::F)
    {
        AddAllItem();
        return Reply.Handled();
    }

	return Reply.Unhandled();
}

void UAPDropPackageUI::BindButton()
{   
    SetIsFocusable(true);
    SetKeyboardFocus();

    Exit_Button->OnClicked.AddDynamic(this, &UAPDropPackageUI::ExitPackage);
    AddAll_Button->OnClicked.AddDynamic(this, &UAPDropPackageUI::AddAllItem);
}

void UAPDropPackageUI::ExitPackage()
{
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!PC) return;
    PC->SetPause(false);
    RemoveFromParent();
}

void UAPDropPackageUI::AddAllItem()
{
    auto Character = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!Character) return;

    for(auto DropItemSlot : SlotArr)
    {
        if(!DropItemSlot) continue; if(!DropItemSlot->GetItemReference()) continue;
            
        Character->GetInventory()->HandleAddItem(DropItemSlot->GetItemReference());
        DropItemSlot->RemoveFromParent();
        DropItemSlot->SetItemReference(nullptr);
    }
}
