
#include "UserInterface/Interaction/InteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/InteractionInterface.h"

void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UInteractionWidget::UpdateWidget(FInteractData InteractData)
{
	KeyPressText->SetText(FText::FromString(InteractData.InteractionKeyName));
	NameText->SetText(FText::FromString(InteractData.InteractionTypeName));
	FSlateFontInfo SlateFontInfo = NameText->GetFont(); SlateFontInfo.Size = InteractData.TypeNameSize;
	NameText->SetFont(SlateFontInfo);
	
}

float UInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}
