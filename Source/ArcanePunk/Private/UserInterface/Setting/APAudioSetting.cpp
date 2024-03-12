
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

    Slider_Master->SetValue(GI->GameSoundVolume.MasterVolume);
    Slider_BGM->SetValue(GI->GameSoundVolume.BGMVolume);
    Slider_Effect->SetValue(GI->GameSoundVolume.EffectVolume);
    
}

void UAPAudioSetting::BindButton()
{
    Button_Back->OnClicked.AddDynamic(this, &UAPAudioSetting::OnClickBack);

    Slider_Master->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_Master);
    Slider_BGM->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_BGM);
    Slider_Effect->OnValueChanged.AddDynamic(this, &UAPAudioSetting::OnSlide_Effect);
}

void UAPAudioSetting::OnClickBack()
{
    RemoveFromParent();
    
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) return;
    OwnerPC->OptionSetting();
}

void UAPAudioSetting::OnSlide_Master(float Value)
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;
    GI->GameSoundVolume.MasterVolume = Value;
}

void UAPAudioSetting::OnSlide_BGM(float Value)
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;
    GI->GameSoundVolume.BGMVolume = Value;
}

void UAPAudioSetting::OnSlide_Effect(float Value)
{
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;
    GI->GameSoundVolume.EffectVolume = Value;
}
