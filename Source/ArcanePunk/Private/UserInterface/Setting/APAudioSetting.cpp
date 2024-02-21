
#include "UserInterface/Setting/APAudioSetting.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Setting/APOptionSetting.h"

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
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) return;

    if(OwnerPC->GetOptionSettingUI())
    {
        Slider_Master->SetValue(OwnerPC->GetOptionSettingUI()->GetMasterVolume());
        Slider_BGM->SetValue(OwnerPC->GetOptionSettingUI()->GetBGMVolume());
        Slider_Effect->SetValue(OwnerPC->GetOptionSettingUI()->GetEffectVolume());
    }
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
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) return;
    UE_LOG(LogTemp, Display, TEXT("Your %f"), Value);
}

void UAPAudioSetting::OnSlide_BGM(float Value)
{
}

void UAPAudioSetting::OnSlide_Effect(float Value)
{
}
