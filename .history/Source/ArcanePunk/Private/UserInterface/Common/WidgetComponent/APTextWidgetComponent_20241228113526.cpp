
#include "UserInterface/Common/WidgetComponent/APTextWidgetComponent.h"

#include "Components/TextBlock.h"

void UAPTextWidgetComponent::NativeConstruct()
{
    Super::NativeConstruct();
}

void UAPTextWidgetComponent::SetDamageText(float AppliedDamage, bool bCritical)
{
    if(bCritical) {Text_Content->SetColorAndOpacity(Critical_Color);}
    else {Text_Content->SetColorAndOpacity(Basic_Color);}

    Text_Content->SetText(FText::FromString(FString::FromInt(round(AppliedDamage))));
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    OnDamageAction();
}

void UAPTextWidgetComponent::SetDetectText()
{
    Text_Content->SetColorAndOpacity(Detect_Color);
    Text_Content->SetText(FText::FromString(DetectText));
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    OnDetectAction();
}
