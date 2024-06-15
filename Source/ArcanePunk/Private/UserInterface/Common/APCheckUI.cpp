
#include "UserInterface/Common/APCheckUI.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UAPCheckUI::SetCheckType(ECheckType UpdateCheckType, UUserWidget* Parent)
{
    ParentWidget = Parent; CurrentCheckType = UpdateCheckType;
    FString Message = TEXT(" ");
    switch (UpdateCheckType)
    {
    case ECheckType::Save:
        Message = TEXT("저장하시겠습니까?");
        break;
    
    case ECheckType::Load:
        Message = TEXT("이 파일로 시작하시겠습니까?");
        break;

    case ECheckType::Purchase:
        Message = TEXT("구매하시겠습니까?");
        break;

    case ECheckType::Select:
        Message = TEXT("선택하시겠습니까?");
        break;

    case ECheckType::Delete:
        Message = TEXT("삭제하시겠습니까?");
        break;
    }
    Text_Message->SetText(FText::FromString(Message));

    Button_Validation->OnClicked.AddDynamic(this, &UAPCheckUI::Validate);
    Button_Cancel->OnClicked.AddDynamic(this, &UAPCheckUI::Cancel);
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
