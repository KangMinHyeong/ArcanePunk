
#include "UserInterface/Setting/APGraphicsSetting.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetStringLibrary.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameInstance/APGameInstance.h"
#include "Interfaces/InteractionInterface.h"

void UAPGraphicsSetting::InitGraphicsSetting()
{
   UGameUserSettings::GetGameUserSettings()->LoadSettings();

    SetFrameRate_Setting(); ChangeFrameRate(UGameUserSettings::GetGameUserSettings()->GetFrameRateLimit());
    PP_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetPostProcessingQuality()); Change_PP(UGameUserSettings::GetGameUserSettings()->GetPostProcessingQuality());
    TQ_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetTextureQuality()); Change_TQ(UGameUserSettings::GetGameUserSettings()->GetTextureQuality());
    AA_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetAntiAliasingQuality()); Change_AA(UGameUserSettings::GetGameUserSettings()->GetAntiAliasingQuality());
    SQ_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetShadowQuality()); Change_SQ(UGameUserSettings::GetGameUserSettings()->GetShadowQuality());
    EQ_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetVisualEffectQuality()); Change_EQ(UGameUserSettings::GetGameUserSettings()->GetVisualEffectQuality());
    FQ_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetFoliageQuality()); Change_FQ(UGameUserSettings::GetGameUserSettings()->GetFoliageQuality());
    Shading_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetShadingQuality()); Change_Shading(UGameUserSettings::GetGameUserSettings()->GetShadingQuality());
    RQ_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetReflectionQuality()); Change_RQ(UGameUserSettings::GetGameUserSettings()->GetReflectionQuality());
    VD_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetViewDistanceQuality()); Change_VD(UGameUserSettings::GetGameUserSettings()->GetViewDistanceQuality());
    GI_Setting = static_cast<EGraphicsSetting>(UGameUserSettings::GetGameUserSettings()->GetGlobalIlluminationQuality()); Change_GI(UGameUserSettings::GetGameUserSettings()->GetGlobalIlluminationQuality());
    SetRS_Setting(); Change_RS(UGameUserSettings::GetGameUserSettings()->GetResolutionScaleNormalized());

}

void UAPGraphicsSetting::InitWindowSetting()
{
    FIntPoint IntPoint = UGameUserSettings::GetGameUserSettings()->GetScreenResolution();
    FString Str; Str.Append(FString::FromInt(IntPoint.X)); Str.Append("x"); Str.Append(FString::FromInt(IntPoint.Y));
    ScreenResolution->SetSelectedOption(Str);

    EWindowMode::Type SelectWindowMode =  UGameUserSettings::GetGameUserSettings()->GetFullscreenMode();
    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;  
    if(SelectWindowMode == EWindowMode::Windowed)
    {
        WindowMode->SetSelectedOption(DataTableGI->GetStringContent(EStringRowName::ScreenMode_Window));
    }
    else // if(SelectWindowMode == EWindowMode::Fullscreen)
    {
        WindowMode->SetSelectedOption(DataTableGI->GetStringContent(EStringRowName::ScreenMode_Full));
    }  

    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

void UAPGraphicsSetting::InitBindSetting()
{
    BindComboBox();
    BindButton();
}


void UAPGraphicsSetting::SetRS_Setting()
{
    float InitRS = UGameUserSettings::GetGameUserSettings()->GetResolutionScaleNormalized(); InitRS = FMath::Clamp( InitRS, 0.0f, 1.0f );
    if( InitRS >= 0.0f && InitRS <= 0.25f) {RS_Setting = static_cast<EGraphicsSetting>(0);}
    else if(InitRS > 0.25f && InitRS <= 0.5f) {RS_Setting = static_cast<EGraphicsSetting>(1);}
    else if(InitRS > 0.5f && InitRS <= 0.75f) {RS_Setting = static_cast<EGraphicsSetting>(2);}
    else {RS_Setting = static_cast<EGraphicsSetting>(3);}
}

void UAPGraphicsSetting::SetFrameRate_Setting()
{
    float InitFrame = UGameUserSettings::GetGameUserSettings()->GetFrameRateLimit();
    if( InitFrame >= 0.0f && InitFrame <= 46.0f) {FrameRateSetting = static_cast<EGraphicsSetting>(0);}
    else if(InitFrame > 46.0f && InitFrame <= 61.0f) {FrameRateSetting = static_cast<EGraphicsSetting>(1);}
    else if(InitFrame > 61.0f && InitFrame <= 121.0f) {FrameRateSetting = static_cast<EGraphicsSetting>(2);}
    else {FrameRateSetting = static_cast<EGraphicsSetting>(3);}
}

void UAPGraphicsSetting::BindComboBox()
{
    //  Window Mode Select
    WindowMode->OnSelectionChanged.AddDynamic(this, &UAPGraphicsSetting::OnSelectChange_WindowMode);
    //  Screen Resolution Size
    ScreenResolution->OnSelectionChanged.AddDynamic(this, &UAPGraphicsSetting::OnSelectChange_ScreenResolution);
}

void UAPGraphicsSetting::BindButton()
{
    //  Frame Rate Limit
    Button_45fps->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickButton_45fps);
    Button_60fps->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickButton_60fps);
    Button_120fps->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickButton_120fps);
    Button_Unlimited->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickButton_Unlimited);

    //  OverAll Scalability
    OS_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickOS_Low); Btn_Low.Emplace(OS_Low);
    OS_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickOS_Middle); Btn_Middle.Emplace(OS_Middle);
    OS_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickOS_High); Btn_High.Emplace(OS_High);
    OS_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickOS_Epic); Btn_Epic.Emplace(OS_Epic);

    //  Post Process
    PP_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickPP_Low); Btn_Low.Emplace(PP_Low);
    PP_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickPP_Middle); Btn_Middle.Emplace(PP_Middle);
    PP_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickPP_High); Btn_High.Emplace(PP_High);
    PP_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickPP_Epic); Btn_Epic.Emplace(PP_Epic);

    //  Texture Quality
    TQ_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickTQ_Low); Btn_Low.Emplace(TQ_Low);
    TQ_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickTQ_Middle); Btn_Middle.Emplace(TQ_Middle);
    TQ_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickTQ_High); Btn_High.Emplace(TQ_High);
    TQ_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickTQ_Epic); Btn_Epic.Emplace(TQ_Epic);

    //  Anti Aliasing
    AA_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickAA_Low); Btn_Low.Emplace(AA_Low);
    AA_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickAA_Middle); Btn_Middle.Emplace(AA_Middle);
    AA_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickAA_High); Btn_High.Emplace(AA_High);
    AA_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickAA_Epic); Btn_Epic.Emplace(AA_Epic);

    //  Shadow Quality
    SQ_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickSQ_Low); Btn_Low.Emplace(SQ_Low);
    SQ_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickSQ_Middle); Btn_Middle.Emplace(SQ_Middle);
    SQ_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickSQ_High); Btn_High.Emplace(SQ_High);
    SQ_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickSQ_Epic); Btn_Epic.Emplace(SQ_Epic);

    //  Effect Quality
    EQ_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickEQ_Low); Btn_Low.Emplace(EQ_Low);
    EQ_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickEQ_Middle); Btn_Middle.Emplace(EQ_Middle);
    EQ_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickEQ_High); Btn_High.Emplace(EQ_High);
    EQ_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickEQ_Epic); Btn_Epic.Emplace(EQ_Epic);

    //  Foliage Quality
    FQ_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickFQ_Low); Btn_Low.Emplace(FQ_Low);
    FQ_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickFQ_Middle); Btn_Middle.Emplace(FQ_Middle);
    FQ_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickFQ_High); Btn_High.Emplace(FQ_High);
    FQ_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickFQ_Epic); Btn_Epic.Emplace(EQ_Epic);

    //  Shading Quality
    Shading_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickShading_Low); Btn_Low.Emplace(Shading_Low);
    Shading_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickShading_Middle); Btn_Middle.Emplace(Shading_Middle);
    Shading_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickShading_High); Btn_High.Emplace(Shading_High);
    Shading_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickShading_Epic); Btn_Epic.Emplace(Shading_Epic);

    //  Reflection Quality
    RQ_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickRQ_Low); Btn_Low.Emplace(RQ_Low);
    RQ_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickRQ_Middle); Btn_Middle.Emplace(RQ_Middle);
    RQ_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickRQ_High); Btn_High.Emplace(RQ_High);
    RQ_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickRQ_Epic); Btn_Epic.Emplace(RQ_Epic);
    
    //  View Distance
    VD_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickVD_Low); Btn_Low.Emplace(VD_Low);
    VD_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickVD_Middle); Btn_Middle.Emplace(VD_Middle);
    VD_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickVD_High); Btn_High.Emplace(VD_High);
    VD_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickVD_Epic); Btn_Epic.Emplace(VD_Epic);

    //  Global Illumination Quality
    GI_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickGI_Low); Btn_Low.Emplace(GI_Low);
    GI_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickGI_Middle); Btn_Middle.Emplace(GI_Middle);
    GI_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickGI_High); Btn_High.Emplace(GI_High);
    GI_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickGI_Epic); Btn_Epic.Emplace(GI_Epic);

    //  Resolution Scale 
    RS_Low->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickRS_Low); Btn_Low.Emplace(RS_Low);
    RS_Middle->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickRS_Middle); Btn_Middle.Emplace(RS_Middle);
    RS_High->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickRS_High); Btn_High.Emplace(RS_High);
    RS_Epic->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickRS_Epic); Btn_Epic.Emplace(RS_Epic);

    //  Back to the Option Setting
    Button_Back->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickButton_Back); 
    //  Save Setting
    // Button_Save->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickButton_Save);
    //  Optimal Setting
    Button_OptimalSetting->OnClicked.AddDynamic(this, &UAPGraphicsSetting::OnClickButton_OptimalSetting);
    
    for(auto btn : Btn_Low)
    {
        auto TextBlock = Cast<UTextBlock>(btn->GetContent());
        if(TextBlock) UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock, EStringRowName::Quality_Low);
    }
    for(auto btn : Btn_Middle)
    {
        auto TextBlock = Cast<UTextBlock>(btn->GetContent());
        if(TextBlock) UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock, EStringRowName::Quality_Middle);
    }
    for(auto btn : Btn_High)
    {
        auto TextBlock = Cast<UTextBlock>(btn->GetContent());
        if(TextBlock) UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock, EStringRowName::Quality_High);
    }
    for(auto btn : Btn_Epic)
    {
        auto TextBlock = Cast<UTextBlock>(btn->GetContent());
        if(TextBlock) UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock, EStringRowName::Quality_Ultra);
    }

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Init, EStringRowName::Init);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Optimization, EStringRowName::Optimization);
}

//  Window Mode Select
void UAPGraphicsSetting::OnSelectChange_WindowMode(FString Items, ESelectInfo::Type type)
{
    EWindowMode::Type SelectWindowMode;
    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;    

    if(Items == DataTableGI->GetStringContent(EStringRowName::ScreenMode_Window))
    {
        SelectWindowMode = EWindowMode::Windowed;
    }
    else
    {
        SelectWindowMode = EWindowMode::Fullscreen;
    }

    UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(SelectWindowMode);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
    UGameUserSettings::GetGameUserSettings()->SaveSettings();
}

//  Screen Resolution Size
void UAPGraphicsSetting::OnSelectChange_ScreenResolution(FString Items, ESelectInfo::Type type)
{
    FString LeftSize;
    FString RightSize;
    FString InS = TEXT("x");
    bool MyBool = Items.FString::Split(InS, &LeftSize, &RightSize, ESearchCase::IgnoreCase, ESearchDir::FromStart);
    FIntPoint IntPoint; IntPoint.X = UKismetStringLibrary::Conv_StringToInt(LeftSize); IntPoint.Y = UKismetStringLibrary::Conv_StringToInt(RightSize);
    UGameUserSettings::GetGameUserSettings()->SetScreenResolution(IntPoint);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
    UGameUserSettings::GetGameUserSettings()->SaveSettings();
}

//  Frame Rate Limit
void UAPGraphicsSetting::OnClickButton_45fps()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(FrameRateSetting == EGraphicsSetting::Low) {return;}
    else{FrameRateSetting = EGraphicsSetting::Low; ChangeFrameRate(45.0f);}
}
void UAPGraphicsSetting::OnClickButton_60fps()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(FrameRateSetting == EGraphicsSetting::Middle) {return;}
    else{FrameRateSetting = EGraphicsSetting::Middle; ChangeFrameRate(60.0f);}
}
void UAPGraphicsSetting::OnClickButton_120fps()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(FrameRateSetting == EGraphicsSetting::High) {return;}
    else{FrameRateSetting = EGraphicsSetting::High; ChangeFrameRate(120.0f);}
}
void UAPGraphicsSetting::OnClickButton_Unlimited()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(FrameRateSetting == EGraphicsSetting::Epic) {return;}
    else{FrameRateSetting = EGraphicsSetting::Epic; ChangeFrameRate(240.0f);}
}
void UAPGraphicsSetting::ChangeFrameRate(float fps)
{
    Button_45fps->SetBackgroundColor(DefaultButtonColor);
    Button_60fps->SetBackgroundColor(DefaultButtonColor);
    Button_120fps->SetBackgroundColor(DefaultButtonColor);
    Button_Unlimited->SetBackgroundColor(DefaultButtonColor);

    switch (FrameRateSetting)
    {
        case EGraphicsSetting::Low:
        Button_45fps->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        Button_60fps->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        Button_120fps->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        Button_Unlimited->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(fps);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  OverAll Scalability
void UAPGraphicsSetting::OnClickOS_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    UGameUserSettings::GetGameUserSettings()->SetOverallScalabilityLevel(0);
    OS_Setting = EGraphicsSetting::Low;
    Change_OS();
}
void UAPGraphicsSetting::OnClickOS_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    UGameUserSettings::GetGameUserSettings()->SetOverallScalabilityLevel(1);
    OS_Setting = EGraphicsSetting::Middle;
    Change_OS();
}
void UAPGraphicsSetting::OnClickOS_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    UGameUserSettings::GetGameUserSettings()->SetOverallScalabilityLevel(2);
    OS_Setting = EGraphicsSetting::High;
    Change_OS();
}
void UAPGraphicsSetting::OnClickOS_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    UGameUserSettings::GetGameUserSettings()->SetOverallScalabilityLevel(3);
    OS_Setting = EGraphicsSetting::Epic;
    Change_OS();
}

void UAPGraphicsSetting::Change_OS()
{
    PP_Setting = OS_Setting; Change_PP((uint8)PP_Setting);
    TQ_Setting = OS_Setting; Change_TQ((uint8)TQ_Setting);
    AA_Setting = OS_Setting; Change_AA((uint8)AA_Setting);
    SQ_Setting = OS_Setting; Change_SQ((uint8)SQ_Setting);
    EQ_Setting = OS_Setting; Change_EQ((uint8)EQ_Setting);
    FQ_Setting = OS_Setting; Change_FQ((uint8)FQ_Setting);
    Shading_Setting = OS_Setting; Change_Shading((uint8)Shading_Setting);
    RQ_Setting = OS_Setting; Change_RQ((uint8)RQ_Setting);
    VD_Setting = OS_Setting; Change_VD((uint8)VD_Setting);
    GI_Setting = OS_Setting; Change_GI((uint8)GI_Setting);
    RS_Setting = OS_Setting; Change_RS(UGameUserSettings::GetGameUserSettings()->GetResolutionScaleNormalized());

}

//  Post Process
void UAPGraphicsSetting::OnClickPP_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(PP_Setting == EGraphicsSetting::Low) {return;}
    else{PP_Setting = EGraphicsSetting::Low; Change_PP(0);}
}
void UAPGraphicsSetting::OnClickPP_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(PP_Setting == EGraphicsSetting::Middle) {return;}
    else{PP_Setting = EGraphicsSetting::Middle; Change_PP(1);}
}
void UAPGraphicsSetting::OnClickPP_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(PP_Setting == EGraphicsSetting::High) {return;}
    else{PP_Setting = EGraphicsSetting::High; Change_PP(2);}
}
void UAPGraphicsSetting::OnClickPP_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(PP_Setting == EGraphicsSetting::Epic) {return;}
    else{PP_Setting = EGraphicsSetting::Epic; Change_PP(3);}
}
void UAPGraphicsSetting::Change_PP(int32 PP)
{
    PP_Low->SetBackgroundColor(DefaultButtonColor);
    PP_Middle->SetBackgroundColor(DefaultButtonColor);
    PP_High->SetBackgroundColor(DefaultButtonColor);
    PP_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (PP_Setting)
    {
        case EGraphicsSetting::Low:
        PP_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        PP_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        PP_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        PP_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetPostProcessingQuality(PP);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  Texture Quality
void UAPGraphicsSetting::OnClickTQ_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(TQ_Setting == EGraphicsSetting::Low) {return;}
    else{TQ_Setting = EGraphicsSetting::Low; Change_TQ(0);}
}
void UAPGraphicsSetting::OnClickTQ_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(TQ_Setting == EGraphicsSetting::Middle) {return;}
    else{TQ_Setting = EGraphicsSetting::Middle; Change_TQ(1);}
}
void UAPGraphicsSetting::OnClickTQ_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(TQ_Setting == EGraphicsSetting::High) {return;}
    else{TQ_Setting = EGraphicsSetting::High; Change_TQ(2);}
}
void UAPGraphicsSetting::OnClickTQ_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(TQ_Setting == EGraphicsSetting::Epic) {return;}
    else{TQ_Setting = EGraphicsSetting::Epic; Change_TQ(3);}
}
void UAPGraphicsSetting::Change_TQ(int32 TQ)
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    TQ_Low->SetBackgroundColor(DefaultButtonColor);
    TQ_Middle->SetBackgroundColor(DefaultButtonColor);
    TQ_High->SetBackgroundColor(DefaultButtonColor);
    TQ_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (TQ_Setting)
    {
        case EGraphicsSetting::Low:
        TQ_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        TQ_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        TQ_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        TQ_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetTextureQuality(TQ);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  Anti Aliasing
void UAPGraphicsSetting::OnClickAA_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(AA_Setting == EGraphicsSetting::Low) {return;}
    else{AA_Setting = EGraphicsSetting::Low; Change_AA(0);}
}
void UAPGraphicsSetting::OnClickAA_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(AA_Setting == EGraphicsSetting::Middle) {return;}
    else{AA_Setting = EGraphicsSetting::Middle; Change_AA(1);}
}
void UAPGraphicsSetting::OnClickAA_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(AA_Setting == EGraphicsSetting::High) {return;}
    else{AA_Setting = EGraphicsSetting::High; Change_AA(2);}
}
void UAPGraphicsSetting::OnClickAA_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(AA_Setting == EGraphicsSetting::Epic) {return;}
    else{AA_Setting = EGraphicsSetting::Epic; Change_AA(3);}
}
void UAPGraphicsSetting::Change_AA(int32 AA)
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    AA_Low->SetBackgroundColor(DefaultButtonColor);
    AA_Middle->SetBackgroundColor(DefaultButtonColor);
    AA_High->SetBackgroundColor(DefaultButtonColor);
    AA_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (AA_Setting)
    {
        case EGraphicsSetting::Low:
        AA_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        AA_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        AA_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        AA_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(AA);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  Shadow Quality
void UAPGraphicsSetting::OnClickSQ_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(SQ_Setting == EGraphicsSetting::Low) {return;}
    else{SQ_Setting = EGraphicsSetting::Low; Change_SQ(0);}
}
void UAPGraphicsSetting::OnClickSQ_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(SQ_Setting == EGraphicsSetting::Middle) {return;}
    else{SQ_Setting = EGraphicsSetting::Middle; Change_SQ(1);}
}
void UAPGraphicsSetting::OnClickSQ_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(SQ_Setting == EGraphicsSetting::High) {return;}
    else{SQ_Setting = EGraphicsSetting::High; Change_SQ(2);}
}
void UAPGraphicsSetting::OnClickSQ_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(SQ_Setting == EGraphicsSetting::Epic) {return;}
    else{SQ_Setting = EGraphicsSetting::Epic; Change_SQ(3);}
}
void UAPGraphicsSetting::Change_SQ(int32 SQ)
{
    SQ_Low->SetBackgroundColor(DefaultButtonColor);
    SQ_Middle->SetBackgroundColor(DefaultButtonColor);
    SQ_High->SetBackgroundColor(DefaultButtonColor);
    SQ_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (SQ_Setting)
    {
        case EGraphicsSetting::Low:
        SQ_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        SQ_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        SQ_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        SQ_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }
    UGameUserSettings::GetGameUserSettings()->SetShadowQuality(SQ);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  Effect Quality
void UAPGraphicsSetting::OnClickEQ_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(EQ_Setting == EGraphicsSetting::Low) {return;}
    else{EQ_Setting = EGraphicsSetting::Low; Change_EQ(0);} 
}
void UAPGraphicsSetting::OnClickEQ_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(EQ_Setting == EGraphicsSetting::Middle) {return;}
    else{EQ_Setting = EGraphicsSetting::Middle; Change_EQ(1);}
}
void UAPGraphicsSetting::OnClickEQ_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(EQ_Setting == EGraphicsSetting::High) {return;}
    else{EQ_Setting = EGraphicsSetting::High; Change_EQ(2);}
}
void UAPGraphicsSetting::OnClickEQ_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(EQ_Setting == EGraphicsSetting::Epic) {return;}
    else{EQ_Setting = EGraphicsSetting::Epic; Change_EQ(3);}
}
void UAPGraphicsSetting::Change_EQ(int32 EQ)
{
    EQ_Low->SetBackgroundColor(DefaultButtonColor);
    EQ_Middle->SetBackgroundColor(DefaultButtonColor);
    EQ_High->SetBackgroundColor(DefaultButtonColor);
    EQ_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (EQ_Setting)
    {
        case EGraphicsSetting::Low:
        EQ_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        EQ_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        EQ_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        EQ_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetVisualEffectQuality(EQ);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  Foliage Quality
void UAPGraphicsSetting::OnClickFQ_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(FQ_Setting == EGraphicsSetting::Low) {return;}
    else{FQ_Setting = EGraphicsSetting::Low; Change_FQ(0);}
}
void UAPGraphicsSetting::OnClickFQ_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(FQ_Setting == EGraphicsSetting::Middle) {return;}
    else{FQ_Setting = EGraphicsSetting::Middle; Change_FQ(1);}
}
void UAPGraphicsSetting::OnClickFQ_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(FQ_Setting == EGraphicsSetting::High) {return;}
    else{FQ_Setting = EGraphicsSetting::High; Change_FQ(2);}
}
void UAPGraphicsSetting::OnClickFQ_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(FQ_Setting == EGraphicsSetting::Epic) {return;}
    else{FQ_Setting = EGraphicsSetting::Epic; Change_FQ(3);}
}
void UAPGraphicsSetting::Change_FQ(int32 FQ)
{
    FQ_Low->SetBackgroundColor(DefaultButtonColor);
    FQ_Middle->SetBackgroundColor(DefaultButtonColor);
    FQ_High->SetBackgroundColor(DefaultButtonColor);
    FQ_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (FQ_Setting)
    {
        case EGraphicsSetting::Low:
        FQ_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        FQ_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        FQ_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        FQ_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetFoliageQuality(FQ);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  Shading Quality
void UAPGraphicsSetting::OnClickShading_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(Shading_Setting == EGraphicsSetting::Low) {return;}
    else{Shading_Setting = EGraphicsSetting::Low; Change_Shading(0);}
}
void UAPGraphicsSetting::OnClickShading_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(Shading_Setting == EGraphicsSetting::Middle) {return;}
    else{Shading_Setting = EGraphicsSetting::Middle; Change_Shading(1);}
}
void UAPGraphicsSetting::OnClickShading_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(Shading_Setting == EGraphicsSetting::High) {return;}
    else{Shading_Setting = EGraphicsSetting::High; Change_Shading(2);}
}
void UAPGraphicsSetting::OnClickShading_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(Shading_Setting == EGraphicsSetting::Epic) {return;}
    else{Shading_Setting = EGraphicsSetting::Epic; Change_Shading(3);}
}
void UAPGraphicsSetting::Change_Shading(int32 Shading)
{
    Shading_Low->SetBackgroundColor(DefaultButtonColor);
    Shading_Middle->SetBackgroundColor(DefaultButtonColor);
    Shading_High->SetBackgroundColor(DefaultButtonColor);
    Shading_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (Shading_Setting)
    {
        case EGraphicsSetting::Low:
        Shading_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        Shading_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        Shading_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        Shading_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetShadingQuality(Shading);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//	Reflection Quality
void UAPGraphicsSetting::OnClickRQ_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(RQ_Setting == EGraphicsSetting::Low) {return;}
    else{RQ_Setting = EGraphicsSetting::Low; Change_RQ(0);}
}
void UAPGraphicsSetting::OnClickRQ_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(RQ_Setting == EGraphicsSetting::Middle) {return;}
    else{RQ_Setting = EGraphicsSetting::Middle; Change_RQ(1);}
}
void UAPGraphicsSetting::OnClickRQ_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(RQ_Setting == EGraphicsSetting::High) {return;}
    else{RQ_Setting = EGraphicsSetting::High; Change_RQ(2);}
}
void UAPGraphicsSetting::OnClickRQ_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(RQ_Setting == EGraphicsSetting::Epic) {return;}
    else{RQ_Setting = EGraphicsSetting::Epic; Change_RQ(3);}
}
void UAPGraphicsSetting::Change_RQ(int32 RQ)
{
    RQ_Low->SetBackgroundColor(DefaultButtonColor);
    RQ_Middle->SetBackgroundColor(DefaultButtonColor);
    RQ_High->SetBackgroundColor(DefaultButtonColor);
    RQ_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (RQ_Setting)
    {
        case EGraphicsSetting::Low:
        RQ_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        RQ_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        RQ_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        RQ_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetReflectionQuality(RQ);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  View Distance
void UAPGraphicsSetting::OnClickVD_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(VD_Setting == EGraphicsSetting::Low) {return;}
    else{VD_Setting = EGraphicsSetting::Low; Change_VD(0);}
}
void UAPGraphicsSetting::OnClickVD_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(VD_Setting == EGraphicsSetting::Middle) {return;}
    else{VD_Setting = EGraphicsSetting::Middle; Change_VD(1);}
}
void UAPGraphicsSetting::OnClickVD_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(VD_Setting == EGraphicsSetting::High) {return;}
    else{VD_Setting = EGraphicsSetting::High; Change_VD(2);}
}
void UAPGraphicsSetting::OnClickVD_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(VD_Setting == EGraphicsSetting::Epic) {return;}
    else{VD_Setting = EGraphicsSetting::Epic; Change_VD(3);}
}
void UAPGraphicsSetting::Change_VD(int32 VD)
{
    VD_Low->SetBackgroundColor(DefaultButtonColor);
    VD_Middle->SetBackgroundColor(DefaultButtonColor);
    VD_High->SetBackgroundColor(DefaultButtonColor);
    VD_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (VD_Setting)
    {
        case EGraphicsSetting::Low:
        VD_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        VD_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        VD_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        VD_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetViewDistanceQuality(VD);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  Global Illumination Quality
void UAPGraphicsSetting::OnClickGI_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(GI_Setting == EGraphicsSetting::Low) {return;}
    else{GI_Setting = EGraphicsSetting::Low; Change_GI(0);}
}
void UAPGraphicsSetting::OnClickGI_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(GI_Setting == EGraphicsSetting::Middle) {return;}
    else{GI_Setting = EGraphicsSetting::Middle; Change_GI(1);}
}
void UAPGraphicsSetting::OnClickGI_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(GI_Setting == EGraphicsSetting::High) {return;}
    else{GI_Setting = EGraphicsSetting::High; Change_GI(2);}
}
void UAPGraphicsSetting::OnClickGI_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(GI_Setting == EGraphicsSetting::Epic) {return;}
    else{GI_Setting = EGraphicsSetting::Epic; Change_GI(3);}
}
void UAPGraphicsSetting::Change_GI(int32 GI)
{
    GI_Low->SetBackgroundColor(DefaultButtonColor);
    GI_Middle->SetBackgroundColor(DefaultButtonColor);
    GI_High->SetBackgroundColor(DefaultButtonColor);
    GI_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (GI_Setting)
    {
        case EGraphicsSetting::Low:
        GI_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        GI_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        GI_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        GI_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetGlobalIlluminationQuality(GI);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  Resolution Scale 
void UAPGraphicsSetting::OnClickRS_Low()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(RS_Setting == EGraphicsSetting::Low) {return;}
    else{RS_Setting = EGraphicsSetting::Low; Change_RS(0.25f);}
}
void UAPGraphicsSetting::OnClickRS_Middle()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(RS_Setting == EGraphicsSetting::Middle) {return;}
    else{RS_Setting = EGraphicsSetting::Middle; Change_RS(0.5f);}
}
void UAPGraphicsSetting::OnClickRS_High()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(RS_Setting == EGraphicsSetting::High) {return;}
    else{RS_Setting = EGraphicsSetting::High; Change_RS(0.75f);}
}
void UAPGraphicsSetting::OnClickRS_Epic()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    if(RS_Setting == EGraphicsSetting::Epic) {return;}
    else{RS_Setting = EGraphicsSetting::Epic; Change_RS(1.0f);}
}
void UAPGraphicsSetting::Change_RS(float RS)
{
    RS_Low->SetBackgroundColor(DefaultButtonColor);
    RS_Middle->SetBackgroundColor(DefaultButtonColor);
    RS_High->SetBackgroundColor(DefaultButtonColor);
    RS_Epic->SetBackgroundColor(DefaultButtonColor);

    switch (RS_Setting)
    {
        case EGraphicsSetting::Low:
        RS_Low->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Middle:
        RS_Middle->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::High:
        RS_High->SetBackgroundColor(SelectedButtonColor);
        break;
        case EGraphicsSetting::Epic:
        RS_Epic->SetBackgroundColor(SelectedButtonColor);
        break;
    }

    UGameUserSettings::GetGameUserSettings()->SetResolutionScaleNormalized(RS);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
}

//  Back to the Option Setting
void UAPGraphicsSetting::OnClickButton_Back()
{
    // 초기화
}

//  Svae Graphics Setting
void UAPGraphicsSetting::OnClickButton_Save()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    UGameUserSettings::GetGameUserSettings()->SaveSettings();
}

void UAPGraphicsSetting::OnClickButton_OptimalSetting()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    UGameUserSettings::GetGameUserSettings()->RunHardwareBenchmark();
    UGameUserSettings::GetGameUserSettings()->ApplyHardwareBenchmarkResults();
    InitGraphicsSetting();
}

void UAPGraphicsSetting::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;    
    auto DataTableGI = Cast<UAPDataTableSubsystem>(APGI->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;    
    
    WindowMode->ClearOptions();
    
    WindowMode->AddOption(DataTableGI->GetStringContent(EStringRowName::ScreenMode_Full));
    WindowMode->AddOption(DataTableGI->GetStringContent(EStringRowName::ScreenMode_Window));  

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Window, EStringRowName::Select_WindowMode);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_AspectRatio, EStringRowName::Select_AspectRatio);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_MaxFPS, EStringRowName::Select_MaxFPS);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_OverAll, EStringRowName::Select_OverAll);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Texture, EStringRowName::Select_Texture);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_AntiAliasing, EStringRowName::Select_AntiAliasing);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_PostProcessing, EStringRowName::Select_PostProcessing);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Shadow, EStringRowName::Select_Shadow);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Effect, EStringRowName::Select_Effect);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Foliage, EStringRowName::Select_Foliage);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Shading, EStringRowName::Select_Shading);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Reflection, EStringRowName::Select_Reflection);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_ViewDistance, EStringRowName::Select_ViewDistance);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_GlobalIllumination, EStringRowName::Select_GlobalIllumination);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_ResolutionScale, EStringRowName::Select_ResolutionScale);

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_NoLimit, EStringRowName::NoLimit);

    InitGraphicsSetting();
    InitWindowSetting();
    InitBindSetting();
}

void UAPGraphicsSetting::NativeConstruct()
{
    Super::NativeConstruct();

}

FReply UAPGraphicsSetting::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    return Reply.Handled();
}
FReply UAPGraphicsSetting::NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
    FReply Reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	return Reply.Handled();
}

