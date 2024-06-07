
#include "UserInterface/Setting/APSmartKeySetting.h"

#include "Components/TextBlock.h"
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

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.TintColor = OnColor;
    Off_ButtonStyle.Normal.TintColor = OffColor;

    if(CopySmartKeyArr[1]) 
    {
        SmartKeyQ_Button_On->SetStyle(On_ButtonStyle); 
        SmartKeyQ_Button_Off->SetStyle(Off_ButtonStyle); 
        SmartKeyQ_Text_On->SetColorAndOpacity(TextOnColor);
        SmartKeyQ_Text_Off->SetColorAndOpacity(TextOffColor);
    }
    else
    {
        SmartKeyQ_Button_On->SetStyle(Off_ButtonStyle); 
        SmartKeyQ_Button_Off->SetStyle(On_ButtonStyle); 
        SmartKeyQ_Text_On->SetColorAndOpacity(TextOffColor);
        SmartKeyQ_Text_Off->SetColorAndOpacity(TextOnColor);
    }

    if(CopySmartKeyArr[2]) 
    {
        SmartKeyE_Button_On->SetStyle(On_ButtonStyle); 
        SmartKeyE_Button_Off->SetStyle(Off_ButtonStyle); 
        SmartKeyE_Text_On->SetColorAndOpacity(TextOnColor);
        SmartKeyE_Text_Off->SetColorAndOpacity(TextOffColor);
    }
    else
    {
        SmartKeyE_Button_On->SetStyle(Off_ButtonStyle); 
        SmartKeyE_Button_Off->SetStyle(On_ButtonStyle); 
        SmartKeyE_Text_On->SetColorAndOpacity(TextOffColor);
        SmartKeyE_Text_Off->SetColorAndOpacity(TextOnColor);
    }

    if(CopySmartKeyArr[3]) 
    {
        SmartKeyR_Button_On->SetStyle(On_ButtonStyle); 
        SmartKeyR_Button_Off->SetStyle(Off_ButtonStyle); 
        SmartKeyR_Text_On->SetColorAndOpacity(TextOnColor);
        SmartKeyR_Text_Off->SetColorAndOpacity(TextOffColor);
    }
    else
    {
        SmartKeyR_Button_On->SetStyle(Off_ButtonStyle); 
        SmartKeyR_Button_Off->SetStyle(On_ButtonStyle); 
        SmartKeyR_Text_On->SetColorAndOpacity(TextOffColor);
        SmartKeyR_Text_Off->SetColorAndOpacity(TextOnColor);
    }
}

void UAPSmartKeySetting::BindSmartKeyButton()
{
    SmartKeyQ_Button_On->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnSmartKeyQ);
    SmartKeyE_Button_On->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnSmartKeyE);
    SmartKeyR_Button_On->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnSmartKeyR);
    SmartKeyQ_Button_Off->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnSmartKeyQ_Off);
    SmartKeyE_Button_Off->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnSmartKeyE_Off);
    SmartKeyR_Button_Off->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnSmartKeyR_Off);

    Apply_Button->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnApply);
    RollBack_Button->OnClicked.AddDynamic(this, &UAPSmartKeySetting::OnCancel);
}

void UAPSmartKeySetting::OnSmartKeyQ()
{
    CopySmartKeyArr[1] = true;

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.TintColor = OnColor;
    Off_ButtonStyle.Normal.TintColor = OffColor;

    SmartKeyQ_Button_On->SetStyle(On_ButtonStyle); 
    SmartKeyQ_Button_Off->SetStyle(Off_ButtonStyle); 
    SmartKeyQ_Text_On->SetColorAndOpacity(TextOnColor);
    SmartKeyQ_Text_Off->SetColorAndOpacity(TextOffColor);
}

void UAPSmartKeySetting::OnSmartKeyE()
{
    CopySmartKeyArr[2] = true;

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.TintColor = OnColor;
    Off_ButtonStyle.Normal.TintColor = OffColor;

    SmartKeyE_Button_On->SetStyle(On_ButtonStyle); 
    SmartKeyE_Button_Off->SetStyle(Off_ButtonStyle); 
    SmartKeyE_Text_On->SetColorAndOpacity(TextOnColor);
    SmartKeyE_Text_Off->SetColorAndOpacity(TextOffColor);
}

void UAPSmartKeySetting::OnSmartKeyR()
{
    CopySmartKeyArr[3] = true;

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.TintColor = OnColor;
    Off_ButtonStyle.Normal.TintColor = OffColor;

    SmartKeyR_Button_On->SetStyle(On_ButtonStyle); 
    SmartKeyR_Button_Off->SetStyle(Off_ButtonStyle); 
    SmartKeyR_Text_On->SetColorAndOpacity(TextOnColor);
    SmartKeyR_Text_Off->SetColorAndOpacity(TextOffColor);
}

void UAPSmartKeySetting::OnSmartKeyQ_Off()
{
    CopySmartKeyArr[1] = false;

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.TintColor = OnColor;
    Off_ButtonStyle.Normal.TintColor = OffColor;

    SmartKeyQ_Button_On->SetStyle(Off_ButtonStyle); 
    SmartKeyQ_Button_Off->SetStyle(On_ButtonStyle); 
    SmartKeyQ_Text_On->SetColorAndOpacity(TextOffColor);
    SmartKeyQ_Text_Off->SetColorAndOpacity(TextOnColor);
}

void UAPSmartKeySetting::OnSmartKeyE_Off()
{
    CopySmartKeyArr[2] = false;

    auto On_ButtonStyle = SmartKeyE_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyE_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.TintColor = OnColor;
    Off_ButtonStyle.Normal.TintColor = OffColor;

    SmartKeyE_Button_On->SetStyle(Off_ButtonStyle); 
    SmartKeyE_Button_Off->SetStyle(On_ButtonStyle); 
    SmartKeyE_Text_On->SetColorAndOpacity(TextOffColor);
    SmartKeyE_Text_Off->SetColorAndOpacity(TextOnColor);
}

void UAPSmartKeySetting::OnSmartKeyR_Off()
{
    CopySmartKeyArr[3] = false;

    auto On_ButtonStyle = SmartKeyR_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyR_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.TintColor = OnColor;
    Off_ButtonStyle.Normal.TintColor = OffColor;

    SmartKeyR_Button_On->SetStyle(Off_ButtonStyle); 
    SmartKeyR_Button_Off->SetStyle(On_ButtonStyle); 
    SmartKeyR_Text_On->SetColorAndOpacity(TextOffColor);
    SmartKeyR_Text_Off->SetColorAndOpacity(TextOnColor);
}

void UAPSmartKeySetting::OnApply()
{
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) {return;}
    OwnerPC->SmartKeyArr = CopySmartKeyArr;
}

void UAPSmartKeySetting::OnCancel()
{
    OnSmartKeyQ_Off();
    OnSmartKeyE_Off();
    OnSmartKeyR_Off();
}
