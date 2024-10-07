
#include "UserInterface/Common/Check/APCheckUI.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameInstance/APGameInstance.h"

void UAPCheckUI::SetCheckType(ECheckType UpdateCheckType, UUserWidget* Parent)
{
    ParentWidget = Parent; CurrentCheckType = UpdateCheckType;

    Button_Validation->OnClicked.AddDynamic(this, &UAPCheckUI::OnClick_Validate);
    Button_Cancel->OnClicked.AddDynamic(this, &UAPCheckUI::OnClick_Cancel);
    Button_Validation->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Cancel->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);

    const UEnum* CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ECheckType"), true); if(!CheckEnum) return;
    FName Name = FName(*(CheckEnum->GetDisplayNameTextByValue((uint8)UpdateCheckType)).ToString());
    
    UAPDataTableSubsystem::SetTextBlock_Name(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Message, Name);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()),Text_Validation, EStringRowName::Validation);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()),Text_Cancel, EStringRowName::Cancel);
}

void UAPCheckUI::OnClick_Validate()
{
    auto Interface = Cast<ICheckingWidgetInterface>(ParentWidget.Get()); if(!Interface) return;
    Interface->OnValidating(CurrentCheckType);
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    RemoveFromParent();
}

void UAPCheckUI::OnClick_Cancel()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    RemoveFromParent();
}
