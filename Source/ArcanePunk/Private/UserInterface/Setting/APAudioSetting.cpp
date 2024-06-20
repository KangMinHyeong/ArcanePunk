
#include "UserInterface/Setting/APAudioSetting.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Setting/APOptionSetting.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"

void UAPAudioSetting::NativeConstruct()
{
    Super::NativeConstruct();
    
    InitSliders();
    BindButton();
}

void UAPAudioSetting::NativeDestruct()
{
    Super::NativeDestruct();

    APGI->OnChangingSoundVolume.Broadcast(APGI->GetGameSoundVolume().MasterVolume, APGI->GetGameSoundVolume().BGMVolume, APGI->GetGameSoundVolume().EffectVolume);
}

FReply UAPAudioSetting::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    return Reply.Handled();
}

FReply UAPAudioSetting::NativeOnMouseWheel(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	return Reply.Handled();
}

void UAPAudioSetting::InitSliders()
{
    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;
    Master = APGI->GetGameSoundVolume().MasterVolume;
    BGM = APGI->GetGameSoundVolume().BGMVolume;
    Effect = APGI->GetGameSoundVolume().EffectVolume;

    Slider_Master->SetValue(Master);
    Slider_BGM->SetValue(BGM);
    Slider_Effect->SetValue(Effect);
}

void UAPAudioSetting::BindButton()
{
    Button_Back->OnClicked.AddDynamic(this, &UAPAudioSetting::OnClickBack);
    Button_Apply->OnClicked.AddDynamic(this, &UAPAudioSetting::OnClickApply);

    Slider_Master->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_Master);
    Slider_BGM->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_BGM);
    Slider_Effect->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_Effect);
}

void UAPAudioSetting::OnClickBack()
{
    Master = 1.0f;
    BGM = 1.0f;
    Effect = 1.0f;

    Slider_Master->SetValue(Master);
    Slider_BGM->SetValue(BGM);
    Slider_Effect->SetValue(Effect);
}

void UAPAudioSetting::OnClickApply()
{
    if(!APGI.IsValid()) return;
    APGI->SetGameMasterVolume(Master);
    APGI->SetGameBGMVolume(BGM);
    APGI->SetGameEffectVolume(Effect);
}

void UAPAudioSetting::OnSlide_Master(float Value)
{
    Master = Value;
    UE_LOG(LogTemp, Display, TEXT("Master %f"), Master);
    if(APGI.IsValid()) APGI->OnChangingSoundVolume.Broadcast(Master, BGM, Effect);
}

void UAPAudioSetting::OnSlide_BGM(float Value)
{
    BGM = Value;

    if(APGI.IsValid()) APGI->OnChangingSoundVolume.Broadcast(Master, BGM, Effect);
}

void UAPAudioSetting::OnSlide_Effect(float Value)
{
    Effect = Value;

    if(APGI.IsValid()) APGI->OnChangingSoundVolume.Broadcast(Master, BGM, Effect);
}
