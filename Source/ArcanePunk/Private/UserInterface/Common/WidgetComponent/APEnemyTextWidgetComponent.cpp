
#include "UserInterface/Common/WidgetComponent/APEnemyTextWidgetComponent.h"

#include "Components/TextBlock.h"

void UAPEnemyTextWidgetComponent::NativeConstruct()
{
    Super::NativeConstruct();
}

void UAPEnemyTextWidgetComponent::SetDamageText(float AppliedDamage, bool bCritical)
{
    if(bCritical) {Text_Content->SetColorAndOpacity(Critical_Color);}
    else {Text_Content->SetColorAndOpacity(Basic_Color);}

    Text_Content->SetText(FText::FromString(FString::FromInt(round(AppliedDamage))));
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    OnDamageAction();
}

void UAPEnemyTextWidgetComponent::SetDetectText()
{
    Text_Content->SetColorAndOpacity(Detect_Color);
    Text_Content->SetText(FText::FromString(DetectText));
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    OnDetectAction();
}
