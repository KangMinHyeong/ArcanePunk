
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

void UAPScreenSetting::NativeConstruct() {
    Super::NativeConstruct();

    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;
    BindButtonAndSlider();
    InitSlider();
    InitUIMaterials();
}

void UAPScreenSetting::BindButtonAndSlider()
{
    Apply_Button->OnClicked.AddDynamic(this, &UAPScreenSetting::OnApply);
    RollBack_Button->OnClicked.AddDynamic(this, &UAPScreenSetting::OnCancel);

    GammaSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnGammaSlide);
    SaturationSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnSaturationSlide);
    ContrastSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnContrastSlide);
    GainSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnGainSlide);
    OffsetSlider->OnValueChanged.AddDynamic(this, &UAPScreenSetting::OnOffsetSlide);
}

void UAPScreenSetting::InitSlider()
{
    Saturation = APGI->GetSaturation(); 
    Contrast = APGI->GetContrast(); 
    Gamma = APGI->GetGamma(); 
    Gain = APGI->GetGain(); 
    Offset = APGI->GetOffset(); 

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

    SetSaturationUIMaterials(FMath::Lerp(-1.0f, 1.0f, APGI->GetSaturation()));
    SetContrastUIMaterials(FMath::Lerp(-1.0f, 1.0f, APGI->GetContrast()));
    
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
    if(!APGI.IsValid()) return;
    APGI->SetSaturation(Saturation);
    APGI->SetContrast(Contrast);
    APGI->SetGamma(Gamma);
    APGI->SetGain(Gain);
    APGI->SetOffset(Offset);

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

    UE_LOG(LogTemp, Display, TEXT("Text_SaturationNum %f"), Saturation);
    
}

void UAPScreenSetting::OnContrastSlide(float Value)
{
    Contrast = Value; 
    float Num = FMath::Lerp(0.0f, 100.0f, Contrast); 
    Text_ContrastNum->SetText(FText::FromString(FString::FromInt(round(Num))));
    SetContrastUIMaterials(FMath::Lerp(-1.0f, 1.0f, Contrast));

    UE_LOG(LogTemp, Display, TEXT("Text_ContrastNum %f"), Saturation);
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
