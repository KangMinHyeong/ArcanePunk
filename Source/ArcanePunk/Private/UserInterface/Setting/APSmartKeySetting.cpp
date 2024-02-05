
#include "UserInterface/Setting/APSmartKeySetting.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void UAPSmartKeySetting::NativeConstruct()
{
    Super::NativeConstruct();

    InitSmartKey();
    BindSmartKeyButton();
}

FReply UAPSmartKeySetting::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
        return Reply.Handled();
    }

    return Reply.Unhandled();
}

void UAPSmartKeySetting::InitSmartKey()
{
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) {return;}

    CopySmartKeyArr = OwnerPC->SmartKeyArr;

    if(CopySmartKeyArr[1]) {SmartKeyQ_Switcher->SetActiveWidget(EnableSmartKeyQ);}
    else{SmartKeyQ_Switcher->SetActiveWidget(DisableSmartKeyQ);}
    if(CopySmartKeyArr[2]) {SmartKeyE_Switcher->SetActiveWidget(EnableSmartKeyE);}
    else{SmartKeyE_Switcher->SetActiveWidget(DisableSmartKeyE);}
    if(CopySmartKeyArr[3]) {SmartKeyR_Switcher->SetActiveWidget(EnableSmartKeyR);}
    else{SmartKeyR_Switcher->SetActiveWidget(DisableSmartKeyR);}
}

void UAPSmartKeySetting::BindSmartKeyButton()
{
    SmartKeyQ_Button->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnSmartKeyQ);
    SmartKeyE_Button->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnSmartKeyE);
    SmartKeyR_Button->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnSmartKeyR);

    Apply_Button->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnApply);
    Cancel_Button->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnCancel);
}

void UAPSmartKeySetting::OnSmartKeyQ()
{
    CopySmartKeyArr[1] = !CopySmartKeyArr[1];
    if(CopySmartKeyArr[1]) {SmartKeyQ_Switcher->SetActiveWidget(EnableSmartKeyQ);}
    else{SmartKeyQ_Switcher->SetActiveWidget(DisableSmartKeyQ);}
}

void UAPSmartKeySetting::OnSmartKeyE()
{
    CopySmartKeyArr[2] = !CopySmartKeyArr[2];
    if(CopySmartKeyArr[2]) {SmartKeyE_Switcher->SetActiveWidget(EnableSmartKeyE);}
    else{SmartKeyE_Switcher->SetActiveWidget(DisableSmartKeyE);}
}

void UAPSmartKeySetting::OnSmartKeyR()
{
    CopySmartKeyArr[3] = !CopySmartKeyArr[3];
    if(CopySmartKeyArr[3]) {SmartKeyR_Switcher->SetActiveWidget(EnableSmartKeyR);}
    else{SmartKeyR_Switcher->SetActiveWidget(DisableSmartKeyR);}
}

void UAPSmartKeySetting::OnApply()
{
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) {return;}
    OwnerPC->SmartKeyArr = CopySmartKeyArr;

    OwnerPC->OptionSetting();
    RemoveFromParent();
}

void UAPSmartKeySetting::OnCancel()
{
    RemoveFromParent();
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) return;
    OwnerPC->OptionSetting();
}
