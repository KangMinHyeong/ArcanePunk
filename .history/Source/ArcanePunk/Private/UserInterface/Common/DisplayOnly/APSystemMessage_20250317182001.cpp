
#include "UserInterface/Common/DisplayOnly/APSystemMessage.h"

#include "GameInstance/APGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UAPSystemMessage::NativeOnInitialized()
{
    Super::NativeOnInitialized();

}

void UAPSystemMessage::DisplaySystemMesseage(EStringRowName StringRowName, bool bLowMessage, bool bWarning)
{
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()),Text_Low, StringRowName);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()),Text_High, StringRowName);

    if(bLowMessage)
    {
        Content_Low->SetVisibility(ESlateVisibility::Visible);
        Content_High->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        Content_Low->SetVisibility(ESlateVisibility::Collapsed);
        Content_High->SetVisibility(ESlateVisibility::Visible);
    }

    if(bWarning)
    {
        Text_Low->SetColorAndOpacity(Warning_Color);
        Text_High->SetColorAndOpacity(Warning_Color);
        // red
    }
    else
    {
        Text_Low->SetColorAndOpacity(Notice_Color);
        Text_High->SetColorAndOpacity(Notice_Color);
        // green
    }
}

void UAPSystemMessage::DisplaySystemMesseage(const FName & Message, bool bLowMessage, bool bWarning)
{
    Text_Low->SetText(FText::FromName(Message));
    Text_Low->SetText(FText::FromName(Message));

    if(bLowMessage)
    {
        Content_Low->SetVisibility(ESlateVisibility::Visible);
        Content_High->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        Content_Low->SetVisibility(ESlateVisibility::Collapsed);
        Content_High->SetVisibility(ESlateVisibility::Visible);
    }

    if(bWarning)
    {
        Text_Low->SetColorAndOpacity(Warning_Color);
        Text_High->SetColorAndOpacity(Warning_Color);
        // red
    }
    else
    {
        Text_Low->SetColorAndOpacity(Notice_Color);
        Text_High->SetColorAndOpacity(Notice_Color);
        // green
    }
}