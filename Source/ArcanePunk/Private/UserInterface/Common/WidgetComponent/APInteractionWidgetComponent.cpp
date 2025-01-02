
#include "UserInterface/Common/WidgetComponent/APInteractionWidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/InteractionInterface.h"
#include "GameInstance/APGameInstance.h"

void UAPInteractionWidgetComponent::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UAPInteractionWidgetComponent::NativeConstruct()
{
	Super::NativeConstruct();

}

void UAPInteractionWidgetComponent::UpdateWidget(const FInteractData & InteractData)
{
    const UEnum* CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EInteractionType"), true); if(!CheckEnum) return;
    FName Name = FName(*(CheckEnum->GetDisplayNameTextByValue((uint8)InteractData.InteractionType)).ToString());
    
	KeyPressText->SetText(FText::FromString(InteractData.InteractionKeyName));
	UAPDataTableSubsystem::SetTextBlock_Name(UAPGameInstance::GetDataTableGI(GetWorld()), NameText, Name);
	FSlateFontInfo SlateFontInfo = NameText->GetFont(); SlateFontInfo.Size = InteractData.TypeNameSize;
	NameText->SetFont(SlateFontInfo);
}

float UAPInteractionWidgetComponent::UpdateInteractionProgress()
{
	return 0.0f;
}
