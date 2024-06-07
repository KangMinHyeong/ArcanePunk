
#include "UserInterface/Setting/APOptionSetting.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Setting/APAudioSetting.h"
#include "UserInterface/Setting/APSmartKeySetting.h"
#include "UserInterface/Setting/APGraphicsSetting.h"

void UAPOptionSetting::NativeConstruct()
{
    Super::NativeConstruct();

    SetIsFocusable(true);
    SetKeyboardFocus();
    BindButton();
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

    Button_Back->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Back);
}

void UAPOptionSetting::OnClickButton_Graphics()
{
    GraphicsSetting();
}

void UAPOptionSetting::OnClickButton_Audio()
{
    AudioSetting();
}

void UAPOptionSetting::OnClickButton_Key()
{
    SmartKeySetting();
}

void UAPOptionSetting::OnClickButton_Back()
{
    RemoveFromParent();
}

// void UAPOptionSetting::InitGraphicsSetting()
// {
//     GraphicsSettingUI = Cast<UAPGraphicsSetting>(CreateWidget(this, GraphicsSettingClass)); if(!GraphicsSettingUI.IsValid()) return;

//     GraphicsSettingUI->InitGraphicsSetting();
//     GraphicsSettingUI->InitWindowSetting();
//     GraphicsSettingUI->InitBindSetting();
// }

// void UAPOptionSetting::InitAudioSetting()
// {
//     AudioSettingUI = Cast<UAPAudioSetting>(CreateWidget(this, AudioSettingClass)); if(!AudioSettingUI.IsValid()) return;
// }

void UAPOptionSetting::GraphicsSetting()
{
    Switcher_Option->SetActiveWidget(GraphicsSettingUI);
    Text_Graphic->SetColorAndOpacity(OnColor);
    Text_Audio->SetColorAndOpacity(OffColor);
    Text_SmartKey->SetColorAndOpacity(OffColor);
    // if(!GraphicsSettingUI.IsValid()) GraphicsSettingUI = Cast<UAPGraphicsSetting>(CreateWidget(this, GraphicsSettingClass)); 
    // if(!GraphicsSettingUI.IsValid()) return;
    
    // GraphicsSettingUI->AddToViewport();
}

void UAPOptionSetting::AudioSetting()
{
    Switcher_Option->SetActiveWidget(AudioSettingUI);
    Text_Graphic->SetColorAndOpacity(OffColor);
    Text_Audio->SetColorAndOpacity(OnColor);
    Text_SmartKey->SetColorAndOpacity(OffColor);
    // if(!AudioSettingUI.IsValid()) return; AudioSettingUI = Cast<UAPAudioSetting>(CreateWidget(this, AudioSettingClass)); if(!AudioSettingUI.IsValid()) return;
    
    // AudioSettingUI->AddToViewport();
}

void UAPOptionSetting::SmartKeySetting()
{
    Switcher_Option->SetActiveWidget(SmartKeySettingUI);
    Text_Graphic->SetColorAndOpacity(OffColor);
    Text_Audio->SetColorAndOpacity(OffColor);
    Text_SmartKey->SetColorAndOpacity(OnColor);
    // auto SmartKeySettingUI = Cast<UAPSmartKeySetting>(CreateWidget(this, SmartKeySettingClass)); if(!SmartKeySettingUI) return;
    
    // SmartKeySettingUI->AddToViewport();
}