
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
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;
    Master = GI->GetGameSoundVolume().MasterVolume;
    BGM = GI->GetGameSoundVolume().BGMVolume;
    Effect = GI->GetGameSoundVolume().EffectVolume;

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
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;
    GI->SetGameMasterVolume(Master);
    GI->SetGameBGMVolume(BGM);
    GI->SetGameEffectVolume(Effect);
}

void UAPAudioSetting::OnSlide_Master(float Value)
{
    Master = Value;
}

void UAPAudioSetting::OnSlide_BGM(float Value)
{
    BGM = Value;
}

void UAPAudioSetting::OnSlide_Effect(float Value)
{
    Effect = Value;
}
