
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

void UAPOptionSetting::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    BindButton();

    ButtonStyle_On = Button_Graphics->GetStyle(); 
    ButtonStyle_Off = Button_Graphics->GetStyle(); 
    ButtonStyle_On.Normal.SetResourceObject(TabImage_ON);
    ButtonStyle_Off.Normal.SetResourceObject(TabImage_OFF);
}

void UAPOptionSetting::NativeConstruct()
{
    Super::NativeConstruct();

    SetIsFocusable(true);
    SetKeyboardFocus();
    
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_SettingTitle, EStringRowName::GameSetting);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Graphic, EStringRowName::Setting_Graphic);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Audio, EStringRowName::Setting_Audio);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_SmartKey, EStringRowName::Setting_SmartKey);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Screen, EStringRowName::Setting_Screen);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_OtherSettings, EStringRowName::Setting_OtherSettings);

    UAPSoundSubsystem::PlayUIOpenSound(UAPGameInstance::GetSoundGI(GetWorld()));

    OnClickButton_Graphics();
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
        OnClickButton_Back();
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
    Button_OtherSettings->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_OtherSettings);

    Button_Back->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Back);

    Button_Graphics->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Audio->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Key->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Screen->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_OtherSettings->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Back->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    
}

void UAPOptionSetting::OnClickButton_Graphics()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    Switcher_Option->SetActiveWidget(GraphicsSettingUI);
    Text_Graphic->SetColorAndOpacity(OnColor);
    Text_Audio->SetColorAndOpacity(OffColor);
    Text_SmartKey->SetColorAndOpacity(OffColor);
    Text_Screen->SetColorAndOpacity(OffColor);
    Text_OtherSettings->SetColorAndOpacity(OffColor);

    Button_Graphics->SetStyle(ButtonStyle_On); 
    Button_Audio->SetStyle(ButtonStyle_Off); 
    Button_Key->SetStyle(ButtonStyle_Off); 
    Button_Screen->SetStyle(ButtonStyle_Off); 
    Button_OtherSettings->SetStyle(ButtonStyle_Off); 
}

void UAPOptionSetting::OnClickButton_Audio()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    Switcher_Option->SetActiveWidget(AudioSettingUI);
    Text_Graphic->SetColorAndOpacity(OffColor);
    Text_Audio->SetColorAndOpacity(OnColor);
    Text_SmartKey->SetColorAndOpacity(OffColor);
    Text_Screen->SetColorAndOpacity(OffColor);
    Text_OtherSettings->SetColorAndOpacity(OffColor);

    Button_Audio->SetStyle(ButtonStyle_On); 
    Button_Graphics->SetStyle(ButtonStyle_Off); 
    Button_Key->SetStyle(ButtonStyle_Off); 
    Button_Screen->SetStyle(ButtonStyle_Off); 
    Button_OtherSettings->SetStyle(ButtonStyle_Off); 
}

void UAPOptionSetting::OnClickButton_Key()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    Switcher_Option->SetActiveWidget(SmartKeySettingUI);
    Text_Graphic->SetColorAndOpacity(OffColor);
    Text_Audio->SetColorAndOpacity(OffColor);
    Text_SmartKey->SetColorAndOpacity(OnColor);
    Text_Screen->SetColorAndOpacity(OffColor);
    Text_OtherSettings->SetColorAndOpacity(OffColor);

    Button_Key->SetStyle(ButtonStyle_On); 
    Button_Graphics->SetStyle(ButtonStyle_Off); 
    Button_Audio->SetStyle(ButtonStyle_Off); 
    Button_Screen->SetStyle(ButtonStyle_Off); 
    Button_OtherSettings->SetStyle(ButtonStyle_Off); 
}

void UAPOptionSetting::OnClickButton_Screen()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    Switcher_Option->SetActiveWidget(ScreenSettingUI);
    Text_Graphic->SetColorAndOpacity(OffColor);
    Text_Audio->SetColorAndOpacity(OffColor);
    Text_SmartKey->SetColorAndOpacity(OffColor);
    Text_Screen->SetColorAndOpacity(OnColor);
    Text_OtherSettings->SetColorAndOpacity(OffColor);

    Button_Screen->SetStyle(ButtonStyle_On); 
    Button_Graphics->SetStyle(ButtonStyle_Off); 
    Button_Audio->SetStyle(ButtonStyle_Off); 
    Button_Key->SetStyle(ButtonStyle_Off); 
    Button_OtherSettings->SetStyle(ButtonStyle_Off); 
}

void UAPOptionSetting::OnClickButton_OtherSettings()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    Switcher_Option->SetActiveWidget(OtherSettingUI);
    Text_Graphic->SetColorAndOpacity(OffColor);
    Text_Audio->SetColorAndOpacity(OffColor);
    Text_SmartKey->SetColorAndOpacity(OffColor);
    Text_Screen->SetColorAndOpacity(OffColor);
    Text_OtherSettings->SetColorAndOpacity(OnColor);

    Button_OtherSettings->SetStyle(ButtonStyle_On); 
    Button_Graphics->SetStyle(ButtonStyle_Off); 
    Button_Audio->SetStyle(ButtonStyle_Off); 
    Button_Key->SetStyle(ButtonStyle_Off); 
    Button_Screen->SetStyle(ButtonStyle_Off); 
}

void UAPOptionSetting::OnClickButton_Back()
{
    UAPSoundSubsystem::PlayUICloseSound(UAPGameInstance::GetSoundGI(GetWorld()));

    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer());
    if(PC) PC->SetPause(false);
}
