
#include "UserInterface/Setting/APOptionSetting.h"

#include "Components/Button.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void UAPOptionSetting::NativeConstruct()
{
    Super::NativeConstruct();

    BindButton();
}

FReply UAPOptionSetting::NativeOnMouseWheel(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	return Reply.Handled();
}

void UAPOptionSetting::BindButton()
{
    
    Button_Graphics->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Graphics);
    Button_Audio->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Audio);
    Button_Key->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Key);

    Button_Back->OnClicked.AddDynamic(this, &UAPOptionSetting::OnClickButton_Back);
}

void UAPOptionSetting::OnClickButton_Graphics()
{
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) return;
    OwnerPC->GraphicsSetting();
    RemoveFromParent();
}

void UAPOptionSetting::OnClickButton_Audio()
{
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) return;
    OwnerPC->AudioSetting();
    RemoveFromParent();
}

void UAPOptionSetting::OnClickButton_Key()
{
    auto OwnerPC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!OwnerPC) return;
    OwnerPC->SmartKeySetting();
    RemoveFromParent();
}




void UAPOptionSetting::OnClickButton_Back()
{
    RemoveFromParent();
}
