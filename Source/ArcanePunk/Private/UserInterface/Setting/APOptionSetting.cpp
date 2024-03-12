
#include "UserInterface/Setting/APOptionSetting.h"

#include "Components/Button.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Setting/APAudioSetting.h"
#include "UserInterface/Setting/APSmartKeySetting.h"
#include "UserInterface/Setting/APGraphicsSetting.h"

void UAPOptionSetting::NativeConstruct()
{
    Super::NativeConstruct();

}

FReply UAPOptionSetting::NativeOnMouseWheel(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
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
    RemoveFromParent();
}

void UAPOptionSetting::OnClickButton_Audio()
{
    AudioSetting();
    RemoveFromParent();
}

void UAPOptionSetting::OnClickButton_Key()
{
    SmartKeySetting();
    RemoveFromParent();
}

void UAPOptionSetting::OnClickButton_Back()
{
    RemoveFromParent();
}

void UAPOptionSetting::InitGraphicsSetting()
{
    GraphicsSettingUI = Cast<UAPGraphicsSetting>(CreateWidget(this, GraphicsSettingClass)); if(!GraphicsSettingUI.IsValid()) return;

    GraphicsSettingUI->InitGraphicsSetting();
    GraphicsSettingUI->InitWindowSetting();
    GraphicsSettingUI->InitBindSetting();
}

void UAPOptionSetting::InitAudioSetting()
{
    AudioSettingUI = Cast<UAPAudioSetting>(CreateWidget(this, AudioSettingClass)); if(!AudioSettingUI.IsValid()) return;
}

void UAPOptionSetting::GraphicsSetting()
{
    if(!GraphicsSettingUI.IsValid()) GraphicsSettingUI = Cast<UAPGraphicsSetting>(CreateWidget(this, GraphicsSettingClass)); 
    if(!GraphicsSettingUI.IsValid()) return;
    
    GraphicsSettingUI->AddToViewport();
}

void UAPOptionSetting::AudioSetting()
{
    if(!AudioSettingUI.IsValid()) return; AudioSettingUI = Cast<UAPAudioSetting>(CreateWidget(this, AudioSettingClass)); if(!AudioSettingUI.IsValid()) return;
    
    AudioSettingUI->AddToViewport();
}

void UAPOptionSetting::SmartKeySetting()
{
    auto SmartKeySettingUI = Cast<UAPSmartKeySetting>(CreateWidget(this, SmartKeySettingClass)); if(!SmartKeySettingUI) return;
    
    SmartKeySettingUI->AddToViewport();
}