// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Setting/APOtherSettings.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/HUD/APHUD.h"

void UAPOtherSettings::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    BindButton();

    On_ButtonStyle = GuideBar_On_Button->GetStyle(); 
    Off_ButtonStyle = GuideBar_Off_Button->GetStyle(); 
    On_ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Off_ButtonStyle.Normal.SetResourceObject(TabImage_OFF);
}

void UAPOtherSettings::NativeConstruct()
{
    Super::NativeConstruct();

    auto SettingGI = Cast<UAPSettingSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSettingSubsystem::StaticClass())); if(!SettingGI) return;
    SettingGI->GetOtherSettings().bGuideBar ?  OnGuideBarOn() : OnGuideBarOff();

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Init, EStringRowName::Init);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Apply, EStringRowName::Apply);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), GuideBarSetting_Text, EStringRowName::GuideBarSetting);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), GuideBar_On_Text, EStringRowName::Button_On);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), GuideBar_Off_Text, EStringRowName::Button_Off);   

    InitOtherSetting();
}

FReply UAPOtherSettings::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    return Reply.Unhandled();
}

void UAPOtherSettings::BindButton()
{
    Apply_Button->OnClicked.AddDynamic(this, &UAPOtherSettings::OnApply);
    RollBack_Button->OnClicked.AddDynamic(this, &UAPOtherSettings::OnRollBack);

    GuideBar_On_Button->OnClicked.AddDynamic(this, &UAPOtherSettings::OnGuideBarOn);
    GuideBar_Off_Button->OnClicked.AddDynamic(this, &UAPOtherSettings::OnGuideBarOff);

    Apply_Button->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    RollBack_Button->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    GuideBar_On_Button->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    GuideBar_Off_Button->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
}

void UAPOtherSettings::InitOtherSetting()
{
    Player = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!Player.IsValid()) return;
}

void UAPOtherSettings::OnApply()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(!Player.IsValid()) return;
    Player->GetAPHUD()->GetStatusWidget()->SetHiddenGuideBar(bGuideBar);

    auto SettingGI = Cast<UAPSettingSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSettingSubsystem::StaticClass())); if(!SettingGI) return;
    SettingGI->SetOtherSettings_GuideBar(bGuideBar);
}

void UAPOtherSettings::OnRollBack()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    OnGuideBarOn();
    OnApply();
}

void UAPOtherSettings::OnGuideBarOn()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    bGuideBar = true;

    GuideBar_On_Button->SetStyle(On_ButtonStyle); 
    GuideBar_Off_Button->SetStyle(Off_ButtonStyle); 

    GuideBar_On_Text->SetColorAndOpacity(TextOnColor);
    GuideBar_Off_Text->SetColorAndOpacity(TextOffColor);
}

void UAPOtherSettings::OnGuideBarOff()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    bGuideBar = false;

    GuideBar_On_Button->SetStyle(Off_ButtonStyle); 
    GuideBar_Off_Button->SetStyle(On_ButtonStyle); 

    GuideBar_On_Text->SetColorAndOpacity(TextOffColor);
    GuideBar_Off_Text->SetColorAndOpacity(TextOnColor);
}
