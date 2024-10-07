
#include "UserInterface/Setting/APAudioSetting.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Setting/APOptionSetting.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/APGameInstance.h"

void UAPAudioSetting::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    BindButton();
}

void UAPAudioSetting::NativeConstruct()
{
    Super::NativeConstruct();
    
    InitSliders();

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Master, EStringRowName::Volume_Master);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_BGM, EStringRowName::Volume_BGM);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Effect, EStringRowName::Volume_Effect);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_UI, EStringRowName::Volume_UI);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Init, EStringRowName::Init);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Apply, EStringRowName::Apply);
}

void UAPAudioSetting::NativeDestruct()
{
    Super::NativeDestruct();

    SoundGI->OnChangingSoundVolume.Broadcast(SoundGI->GetGameSoundVolume().MasterVolume, SoundGI->GetGameSoundVolume().BGMVolume, SoundGI->GetGameSoundVolume().EffectVolume, SoundGI->GetGameSoundVolume().UIVolume);
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
    SoundGI = Cast<UAPSoundSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSoundSubsystem::StaticClass())); if(!SoundGI.IsValid()) return;
    Master = SoundGI->GetGameSoundVolume().MasterVolume;
    BGM = SoundGI->GetGameSoundVolume().BGMVolume;
    Effect = SoundGI->GetGameSoundVolume().EffectVolume;
    UI = SoundGI->GetGameSoundVolume().UIVolume;

    Slider_Master->SetValue(Master);
    Slider_BGM->SetValue(BGM);
    Slider_Effect->SetValue(Effect);
    Slider_UI->SetValue(UI);
}

void UAPAudioSetting::BindButton()
{
    Button_Back->OnClicked.AddDynamic(this, &UAPAudioSetting::OnClickBack);
    Button_Apply->OnClicked.AddDynamic(this, &UAPAudioSetting::OnClickApply);

    Button_Back->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Apply->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);

    Slider_Master->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_Master);
    Slider_BGM->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_BGM);
    Slider_Effect->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_Effect);
    Slider_UI->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_UI);
}

void UAPAudioSetting::OnClickBack()
{
    Master = 0.5f;
    BGM = 0.5f;
    Effect = 0.5f;
    UI = 0.5f;

    Slider_Master->SetValue(Master);
    Slider_BGM->SetValue(BGM);
    Slider_Effect->SetValue(Effect);
    Slider_UI->SetValue(UI);
    
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
}

void UAPAudioSetting::OnClickApply()
{
    if(!SoundGI.IsValid()) return;
    SoundGI->SetGameMasterVolume(Master);
    SoundGI->SetGameBGMVolume(BGM);
    SoundGI->SetGameEffectVolume(Effect);
    SoundGI->SetGameUIVolume(UI);
    
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
}

void UAPAudioSetting::OnSlide_Master(float Value)
{
    Master = Value;

    if(SoundGI.IsValid()) SoundGI->OnChangingSoundVolume.Broadcast(Master, BGM, Effect, UI);
}

void UAPAudioSetting::OnSlide_BGM(float Value)
{
    BGM = Value;

    if(SoundGI.IsValid()) SoundGI->OnChangingSoundVolume.Broadcast(Master, BGM, Effect, UI);
}

void UAPAudioSetting::OnSlide_Effect(float Value)
{
    Effect = Value;

    if(SoundGI.IsValid()) SoundGI->OnChangingSoundVolume.Broadcast(Master, BGM, Effect, UI);
}

void UAPAudioSetting::OnSlide_UI(float Value)
{
    UI = Value;

    if(SoundGI.IsValid()) SoundGI->OnChangingSoundVolume.Broadcast(Master, BGM, Effect, UI);
}
