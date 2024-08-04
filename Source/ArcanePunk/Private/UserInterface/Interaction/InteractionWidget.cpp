
#include "UserInterface/Interaction/InteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/InteractionInterface.h"
#include "GameInstance/APGameInstance.h"

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
	auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  

    const UEnum* CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EInteractionType"), true); if(!CheckEnum) return;
    FName Name = FName(*(CheckEnum->GetDisplayNameTextByValue((uint8)InteractData.InteractionType)).ToString());
    
	KeyPressText->SetText(FText::FromString(InteractData.InteractionKeyName));
	APGI->SetTextBlock_Name(NameText, Name);
	FSlateFontInfo SlateFontInfo = NameText->GetFont(); SlateFontInfo.Size = InteractData.TypeNameSize;
	NameText->SetFont(SlateFontInfo);
	
}

float UInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}
