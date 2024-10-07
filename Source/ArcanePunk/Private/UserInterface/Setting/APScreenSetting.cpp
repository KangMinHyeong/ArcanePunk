
#include "UserInterface/Setting/APScreenSetting.h"

#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameInstance/APGameInstance.h"

void UAPScreenSetting::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    BindButtonAndSlider();
}

void UAPScreenSetting::NativeConstruct() 
{   
    Super::NativeConstruct();

    SettingGI = Cast<UAPSettingSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSettingSubsystem::StaticClass())); if(!SettingGI.IsValid()) return;

    InitSlider();
    InitUIMaterials();

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Saturation, EStringRowName::Grading_Saturation);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Contrast, EStringRowName::Grading_Contrast);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Gamma, EStringRowName::Grading_Gamma);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Gain, EStringRowName::Grading_Gain);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Offset, EStringRowName::Grading_Offset);

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Init, EStringRowName::Init);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Apply, EStringRowName::Apply);
}

void UAPScreenSetting::BindButtonAndSlider()
{
    Apply_Button->OnClicked.AddDynamic(this, &UAPScreenSetting::OnApply);
    RollBack_Button->OnClicked.AddDynamic(this, &UAPScreenSetting::OnCancel);

    Apply_Button->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    RollBack_Button->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    
    GammaSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnGammaSlide);
    SaturationSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnSaturationSlide);
    ContrastSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnContrastSlide);
    GainSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnGainSlide);
    OffsetSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnOffsetSlide);
}

void UAPScreenSetting::InitSlider()
{
    Saturation = SettingGI->GetSaturation(); 
    Contrast = SettingGI->GetContrast(); 
    Gamma = SettingGI->GetGamma(); 
    Gain = SettingGI->GetGain(); 
    Offset = SettingGI->GetOffset(); 

    Text_SaturationNum->SetText(FText::FromString(FString::FromInt(round(FMath::Lerp(0.0f, 100.0f, Saturation)))));
    SaturationSlider->SetValue(Saturation);

    Text_ContrastNum->SetText(FText::FromString(FString::FromInt(round(FMath::Lerp(0.0f, 100.0f, Contrast)))));
    ContrastSlider->SetValue(Contrast);

    Text_GammaNum->SetText(FText::FromString(FString::FromInt(round(FMath::Lerp(0.0f, 100.0f, Gamma)))));
    GammaSlider->SetValue(Gamma);

    Text_GainNum->SetText(FText::FromString(FString::FromInt(round(FMath::Lerp(0.0f, 100.0f, Gain)))));
    GainSlider->SetValue(Gain);

    Text_OffsetNum->SetText(FText::FromString(FString::FromInt(round(FMath::Lerp(0.0f, 100.0f, Offset)))));
    OffsetSlider->SetValue(Offset);
}

void UAPScreenSetting::InitUIMaterials()
{
    UIMaterials.Empty();
    UIMaterials.Emplace(Image_WhiteBG->GetDynamicMaterial());
    UIMaterials.Emplace(Image_WhiteSymbol->GetDynamicMaterial());
    UIMaterials.Emplace(Image_BlackBG->GetDynamicMaterial());
    UIMaterials.Emplace(Image_BlackSymbol->GetDynamicMaterial());
    UIMaterials.Emplace(Image_ColorBG->GetDynamicMaterial());
    UIMaterials.Emplace(Image_ColorSymbol->GetDynamicMaterial());

    SetSaturationUIMaterials(FMath::Lerp(-1.0f, 1.0f, SettingGI->GetSaturation()));
    SetContrastUIMaterials(FMath::Lerp(-1.0f, 1.0f, SettingGI->GetContrast()));
    
}

void UAPScreenSetting::SetSaturationUIMaterials(float Value)
{
    for(auto Mat : UIMaterials) Mat->SetScalarParameterValue(TEXT("Saturation"), Value);
}

void UAPScreenSetting::SetContrastUIMaterials(float Value)
{
    for(auto Mat : UIMaterials) Mat->SetScalarParameterValue(TEXT("Contrast"), Value);
}

void UAPScreenSetting::OnApply()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    if(!SettingGI.IsValid()) return;
    SettingGI->SetSaturation(Saturation);
    SettingGI->SetContrast(Contrast);
    SettingGI->SetGamma(Gamma);
    SettingGI->SetGain(Gain);
    SettingGI->SetOffset(Offset);
 
    float SaturationValue = FMath::Lerp(0.0f, 2.0f, Saturation); 
    for (auto PPV : TActorRange<APostProcessVolume>(GetWorld())) {
        PPV->Settings.bOverride_ColorSaturation = true;
        PPV->Settings.ColorSaturation.X = SaturationValue; PPV->Settings.ColorSaturation.Y = SaturationValue; PPV->Settings.ColorSaturation.Z = SaturationValue;
    }
    float ContrastValue = FMath::Lerp(0.0f, 2.0f, Contrast); 
    for (auto PPV : TActorRange<APostProcessVolume>(GetWorld())) {
        PPV->Settings.bOverride_ColorContrast = true;
        PPV->Settings.ColorContrast.X = ContrastValue; PPV->Settings.ColorContrast.Y = ContrastValue; PPV->Settings.ColorContrast.Z = ContrastValue;
    }
    float GainValue = FMath::Lerp(0.0f, 2.0f, Gain); 
    for (auto PPV : TActorRange<APostProcessVolume>(GetWorld())) {
        PPV->Settings.bOverride_ColorGain = true;
        PPV->Settings.ColorGain.X = GainValue; PPV->Settings.ColorGain.Y = GainValue; PPV->Settings.ColorGain.Z = GainValue;
    }
    float OffsetValue = FMath::Lerp(-1.0f, 1.0f, Offset); 
    for (auto PPV : TActorRange<APostProcessVolume>(GetWorld())) {
        PPV->Settings.bOverride_ColorOffset = true;
        PPV->Settings.ColorOffset.X = OffsetValue; PPV->Settings.ColorOffset.Y = OffsetValue; PPV->Settings.ColorOffset.Z = OffsetValue;
    }
}

void UAPScreenSetting::OnCancel()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    OnGammaSlide(0.5f);
    OnSaturationSlide(0.5f);
    OnContrastSlide(0.5f);
    OnGainSlide(0.5f);
    OnOffsetSlide(0.5f);

    SaturationSlider->SetValue(0.5f);
    ContrastSlider->SetValue(0.5f);
    GammaSlider->SetValue(0.5f);
    GainSlider->SetValue(0.5f);
    OffsetSlider->SetValue(0.5f);
}

void UAPScreenSetting::OnGammaSlide(float Value)
{
    Gamma = Value;
    FString Gamma_String = TEXT("Gamma "); Gamma_String.Append(FString::Printf(TEXT("%.2f"), FMath::Lerp(1.2f, 3.2f, Gamma)));
    Text_GammaNum->SetText(FText::FromString(FString::FromInt(round(FMath::Lerp(0.0f, 100.0f, Gamma)))));
    
    for (auto PPV : TActorRange<APostProcessVolume>(GetWorld())) {

        UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), Gamma_String);
        // PPV->Settings.bOverride_ColorGamma = true;
        // PPV->Settings.ColorGamma.X = GammaValue;
        // PPV->Settings.ColorGamma.Y = GammaValue;
        // PPV->Settings.ColorGamma.Z = GammaValue;
    }
}

void UAPScreenSetting::OnSaturationSlide(float Value)
{
    Saturation = Value;
    float Num = FMath::Lerp(0.0f, 100.0f, Saturation); 
    Text_SaturationNum->SetText(FText::FromString(FString::FromInt(round(Num))));
    SetSaturationUIMaterials(FMath::Lerp(-1.0f, 1.0f, Saturation));

}

void UAPScreenSetting::OnContrastSlide(float Value)
{
    Contrast = Value; 
    float Num = FMath::Lerp(0.0f, 100.0f, Contrast); 
    Text_ContrastNum->SetText(FText::FromString(FString::FromInt(round(Num))));
    SetContrastUIMaterials(FMath::Lerp(-1.0f, 1.0f, Contrast));
}

void UAPScreenSetting::OnGainSlide(float Value)
{
    Gain = Value;
    float Num = FMath::Lerp(0.0f, 100.0f, Gain); 
    Text_GainNum->SetText(FText::FromString(FString::FromInt(round(Num))));

    
}

void UAPScreenSetting::OnOffsetSlide(float Value)
{
    Offset = Value;
    float Num = FMath::Lerp(0.0f, 100.0f, Offset); 
    Text_OffsetNum->SetText(FText::FromString(FString::FromInt(round(Num))));    
}

