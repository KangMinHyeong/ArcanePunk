
#include "UserInterface/Setting/APOptionSetting.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Setting/APAudioSetting.h"
#include "UserInterface/Setting/APSmartKeySetting.h"
#include "UserInterface/Setting/APGraphicsSetting.h"
#include "UserInterface/Setting/APScreenSetting.h"
#include "GameInstance/APGameInstance.h"

void UAPOptionSetting::NativeConstruct()
{
    Super::NativeConstruct();

    SetIsFocusable(true);
    SetKeyboardFocus();
    BindButton();

    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  

    APGI->SetTextBlock(TextBlock_SettingTitle, EStringRowName::GameSetting);
    APGI->SetTextBlock(Text_Graphic, EStringRowName::Setting_Graphic);
    APGI->SetTextBlock(Text_Audio, EStringRowName::Setting_Audio);
    APGI->SetTextBlock(Text_SmartKey, EStringRowName::Setting_SmartKey);
    APGI->SetTextBlock(Text_Screen, EStringRowName::Setting_Screen);
}

FReply UAPOptionSetting::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    return Reply.Handled();
}

FReply UAPOptionSetting::NativeOnMouseWheel(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	return Reply.Handled();
}

FReply UAPOptionSetting::NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) // 전부 인벤토리로 (F키)
{
    FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    if(InKeyEvent.GetKey() == EKeys::Escape)
    {
        RemoveFromParent();
        return Reply.Handled();
    }
    
	return Reply.Handled();
}

void UAPOptionSetting::BindButton()
{
    Button_Graphics->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Graphics);
    Button_Audio->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Audio);
    Button_Key->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Key);
    Button_Screen->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Screen);

    Button_Back->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Back);
}

void UAPOptionSetting::OnClickButton_Graphics()
{
    Switcher_Option->SetActiveWidget(GraphicsSettingUI);
    Text_Graphic->SetColorAndOpacity(OnColor);
    Text_Audio->SetColorAndOpacity(OffColor);
    Text_SmartKey->SetColorAndOpacity(OffColor);
    Text_Screen->SetColorAndOpacity(OffColor);

    auto ButtonStyle = Button_Graphics->GetStyle(); 
    ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Button_Graphics->SetStyle(ButtonStyle); 
    ButtonStyle.Normal.SetResourceObject(TabImage_OFF);
    Button_Audio->SetStyle(ButtonStyle); 
    Button_Key->SetStyle(ButtonStyle); 
    Button_Screen->SetStyle(ButtonStyle); 
}

void UAPOptionSetting::OnClickButton_Audio()
{
    Switcher_Option->SetActiveWidget(AudioSettingUI);
    Text_Graphic->SetColorAndOpacity(OffColor);
    Text_Audio->SetColorAndOpacity(OnColor);
    Text_SmartKey->SetColorAndOpacity(OffColor);
    Text_Screen->SetColorAndOpacity(OffColor);

    auto ButtonStyle = Button_Audio->GetStyle(); 
    ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Button_Audio->SetStyle(ButtonStyle); 
    ButtonStyle.Normal.SetResourceObject(TabImage_OFF);
    Button_Graphics->SetStyle(ButtonStyle); 
    Button_Key->SetStyle(ButtonStyle); 
    Button_Screen->SetStyle(ButtonStyle); 
}

void UAPOptionSetting::OnClickButton_Key()
{
    Switcher_Option->SetActiveWidget(SmartKeySettingUI);
    Text_Graphic->SetColorAndOpacity(OffColor);
    Text_Audio->SetColorAndOpacity(OffColor);
    Text_SmartKey->SetColorAndOpacity(OnColor);
    Text_Screen->SetColorAndOpacity(OffColor);

    auto ButtonStyle = Button_Key->GetStyle(); 
    ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Button_Key->SetStyle(ButtonStyle); 
    ButtonStyle.Normal.SetResourceObject(TabImage_OFF);
    Button_Graphics->SetStyle(ButtonStyle); 
    Button_Audio->SetStyle(ButtonStyle); 
    Button_Screen->SetStyle(ButtonStyle); 
}

void UAPOptionSetting::OnClickButton_Screen()
{
    Switcher_Option->SetActiveWidget(ScreenSettingUI);
    Text_Graphic->SetColorAndOpacity(OffColor);
    Text_Audio->SetColorAndOpacity(OffColor);
    Text_SmartKey->SetColorAndOpacity(OffColor);
    Text_Screen->SetColorAndOpacity(OnColor);

    auto ButtonStyle = Button_Key->GetStyle(); 
    ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Button_Screen->SetStyle(ButtonStyle); 
    ButtonStyle.Normal.SetResourceObject(TabImage_OFF);
    Button_Graphics->SetStyle(ButtonStyle); 
    Button_Audio->SetStyle(ButtonStyle); 
    Button_Key->SetStyle(ButtonStyle); 
}

void UAPOptionSetting::OnClickButton_Back()
{
    RemoveFromParent();
}

