
#include "UserInterface/Setting/APSmartKeySetting.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "GameInstance/APGameInstance.h"

void UAPSmartKeySetting::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    BindSmartKeyButton();
}

void UAPSmartKeySetting::NativeConstruct()
{
    Super::NativeConstruct();

    InitSmartKey();
    
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Q, EStringRowName::SmartKey_Q);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_E, EStringRowName::SmartKey_E);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_R, EStringRowName::SmartKey_R);

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), SmartKeyQ_Text_On, EStringRowName::Button_On);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), SmartKeyE_Text_On, EStringRowName::Button_On);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), SmartKeyR_Text_On, EStringRowName::Button_On);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), SmartKeyQ_Text_Off, EStringRowName::Button_Off);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), SmartKeyE_Text_Off, EStringRowName::Button_Off);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), SmartKeyR_Text_Off, EStringRowName::Button_Off);

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Init, EStringRowName::Init);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Apply, EStringRowName::Apply);
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
    auto SettingGI = Cast<UAPSettingSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSettingSubsystem::StaticClass())); if(!SettingGI) return;
    CopySmartKeyArr = SettingGI->GetSmartKey();

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Off_ButtonStyle.Normal.SetResourceObject(TabImage_OFF);

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

    SmartKeyQ_Button_On->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    SmartKeyE_Button_On->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    SmartKeyR_Button_On->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    SmartKeyQ_Button_Off->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    SmartKeyE_Button_Off->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    SmartKeyR_Button_Off->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Apply_Button->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    RollBack_Button->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
}

void UAPSmartKeySetting::OnSmartKeyQ()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    CopySmartKeyArr[1] = true;

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Off_ButtonStyle.Normal.SetResourceObject(TabImage_OFF);

    SmartKeyQ_Button_On->SetStyle(On_ButtonStyle); 
    SmartKeyQ_Button_Off->SetStyle(Off_ButtonStyle); 
    SmartKeyQ_Text_On->SetColorAndOpacity(TextOnColor);
    SmartKeyQ_Text_Off->SetColorAndOpacity(TextOffColor);
}

void UAPSmartKeySetting::OnSmartKeyE()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    CopySmartKeyArr[2] = true;

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Off_ButtonStyle.Normal.SetResourceObject(TabImage_OFF);

    SmartKeyE_Button_On->SetStyle(On_ButtonStyle); 
    SmartKeyE_Button_Off->SetStyle(Off_ButtonStyle); 
    SmartKeyE_Text_On->SetColorAndOpacity(TextOnColor);
    SmartKeyE_Text_Off->SetColorAndOpacity(TextOffColor);
}

void UAPSmartKeySetting::OnSmartKeyR()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    CopySmartKeyArr[3] = true;

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Off_ButtonStyle.Normal.SetResourceObject(TabImage_OFF);

    SmartKeyR_Button_On->SetStyle(On_ButtonStyle); 
    SmartKeyR_Button_Off->SetStyle(Off_ButtonStyle); 
    SmartKeyR_Text_On->SetColorAndOpacity(TextOnColor);
    SmartKeyR_Text_Off->SetColorAndOpacity(TextOffColor);
}

void UAPSmartKeySetting::OnSmartKeyQ_Off()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    CopySmartKeyArr[1] = false;

    auto On_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyQ_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Off_ButtonStyle.Normal.SetResourceObject(TabImage_OFF);

    SmartKeyQ_Button_On->SetStyle(Off_ButtonStyle); 
    SmartKeyQ_Button_Off->SetStyle(On_ButtonStyle); 
    SmartKeyQ_Text_On->SetColorAndOpacity(TextOffColor);
    SmartKeyQ_Text_Off->SetColorAndOpacity(TextOnColor);
}

void UAPSmartKeySetting::OnSmartKeyE_Off()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    CopySmartKeyArr[2] = false;

    auto On_ButtonStyle = SmartKeyE_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyE_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Off_ButtonStyle.Normal.SetResourceObject(TabImage_OFF);

    SmartKeyE_Button_On->SetStyle(Off_ButtonStyle); 
    SmartKeyE_Button_Off->SetStyle(On_ButtonStyle); 
    SmartKeyE_Text_On->SetColorAndOpacity(TextOffColor);
    SmartKeyE_Text_Off->SetColorAndOpacity(TextOnColor);
}

void UAPSmartKeySetting::OnSmartKeyR_Off()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    
    CopySmartKeyArr[3] = false;

    auto On_ButtonStyle = SmartKeyR_Button_On->GetStyle(); 
    auto Off_ButtonStyle = SmartKeyR_Button_On->GetStyle(); 
    On_ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Off_ButtonStyle.Normal.SetResourceObject(TabImage_OFF);

    SmartKeyR_Button_On->SetStyle(Off_ButtonStyle); 
    SmartKeyR_Button_Off->SetStyle(On_ButtonStyle); 
    SmartKeyR_Text_On->SetColorAndOpacity(TextOffColor);
    SmartKeyR_Text_Off->SetColorAndOpacity(TextOnColor);
}

void UAPSmartKeySetting::OnApply()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    auto SettingGI = Cast<UAPSettingSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSettingSubsystem::StaticClass())); if(!SettingGI) return;
    SettingGI->SetSmartKey(CopySmartKeyArr);
}

void UAPSmartKeySetting::OnCancel()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    OnSmartKeyQ_Off();
    OnSmartKeyE_Off();
    OnSmartKeyR_Off();
}
