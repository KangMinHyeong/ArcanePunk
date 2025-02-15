
#include "UserInterface/Common/WidgetComponent/APTextWidgetComponent.h"

#include "Components/TextBlock.h"

void UAPTextWidgetComponent::NativeConstruct()
{
    Super::NativeConstruct();
}

void UAPTextWidgetComponent::SetDamageText(float AppliedDamage, bool bCritical)
{
    if(bCritical) {Content_Text->SetColorAndOpacity(Critical_Color);}
    else {Content_Text->SetColorAndOpacity(Basic_Color);}

    Content_Text->SetText(FText::FromString(FString::FromInt(round(AppliedDamage))));
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    OnDamageAction();
}

void UAPTextWidgetComponent::SetDetectText()
{
    Content_Text->SetColorAndOpacity(Detect_Color);
    Content_Text->SetText(FText::FromString(DetectText));
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    OnDetectAction();
}

void UAPTextWidgetComponent::SetDialogueText()
{
    FString DialogueText = "!";
    Content_Text->SetText(FText::FromString(DialogueText));
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
