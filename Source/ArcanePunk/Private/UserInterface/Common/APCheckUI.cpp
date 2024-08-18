
#include "UserInterface/Common/APCheckUI.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameInstance/APGameInstance.h"

void UAPCheckUI::SetCheckType(ECheckType UpdateCheckType, UUserWidget* Parent)
{
    ParentWidget = Parent; CurrentCheckType = UpdateCheckType;

    Button_Validation->OnClicked.AddDynamic(this, &UAPCheckUI::OnValidate);
    Button_Cancel->OnClicked.AddDynamic(this, &UAPCheckUI::OnCancel);

    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  

    const UEnum* CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ECheckType"), true); if(!CheckEnum) return;
    FName Name = FName(*(CheckEnum->GetDisplayNameTextByValue((uint8)UpdateCheckType)).ToString());
    
    APGI->SetTextBlock_Name(Text_Message, Name);
    APGI->SetTextBlock(Text_Validation, EStringRowName::Validation);
    APGI->SetTextBlock(Text_Cancel, EStringRowName::Cancel);
}

void UAPCheckUI::OnValidate()
{
    auto Interface = Cast<ICheckingWidgetInterface>(ParentWidget.Get()); if(!Interface) return;
    Interface->OnValidating(CurrentCheckType);

    RemoveFromParent();
}

void UAPCheckUI::OnCancel()
{
    RemoveFromParent();
}
