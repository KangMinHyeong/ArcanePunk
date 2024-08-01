
#include "UserInterface/Common/APCheckUI.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameInstance/APGameInstance.h"
#include "Interfaces/InteractionInterface.h"

void UAPCheckUI::SetCheckType(ECheckType UpdateCheckType, UUserWidget* Parent)
{
    ParentWidget = Parent; CurrentCheckType = UpdateCheckType;

    Button_Validation->OnClicked.AddDynamic(this, &UAPCheckUI::Validate);
    Button_Cancel->OnClicked.AddDynamic(this, &UAPCheckUI::Cancel);

    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  

    const UEnum* CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ECheckType"), true); if(!CheckEnum) return;
    FName Name = FName(*(CheckEnum->GetDisplayNameTextByValue((uint8)UpdateCheckType)).ToString());
    auto DataTable = APGI->GetContentTextData()->FindRow<FContentTextDataTable>(Name, Name.ToString()); 

    Text_Message->SetText(FText::FromString(DataTable->Content));
}

void UAPCheckUI::Validate()
{
    auto Interface = Cast<ICheckingWidgetInterface>(ParentWidget.Get()); if(!Interface) return;
    Interface->OnValidating(CurrentCheckType);

    RemoveFromParent();
}

void UAPCheckUI::Cancel()
{
    RemoveFromParent();
}
