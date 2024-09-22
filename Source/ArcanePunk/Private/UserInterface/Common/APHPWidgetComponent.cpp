
#include "UserInterface/Common/APHPWidgetComponent.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Character/APCharacterBase.h"

void UAPHPWidgetComponent::NativeConstruct()
{
    Super::NativeConstruct();

    // auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwningPlayerPawn());
    // if(OwnerCharacter)
    // {
    //     OwnerCharacter->OnMaxHPAndHPChanged.AddDynamic(this, &UAPHPWidgetComponent::UpdatingMaxHPAndHP);
    // }
}

void UAPHPWidgetComponent::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    CurrentHP = FMath::FInterpConstantTo(CurrentHP, TargetHP, InDeltaTime, HPBarSpeed);
    HPPercent->SetPercent(CurrentHP / MaxHP);
    // HP_Text->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CurrentHP))));
}

void UAPHPWidgetComponent::UpdatingMaxHPAndHP(float UpdateMaxHP, float UpdateHP)
{
    MaxHP = UpdateMaxHP; TargetHP = UpdateHP;
    // MaxHP_Text->SetText(FText::FromString(FString::FromInt(static_cast<int32>(UpdateMaxHP))));
}

void UAPHPWidgetComponent::InitMaxHPAndHP(float UpdateMaxHP, float UpdateHP)
{
    MaxHP = UpdateMaxHP; CurrentHP = UpdateHP; TargetHP = UpdateHP;
    // MaxHP_Text->SetText(FText::FromString(FString::FromInt(static_cast<int32>(UpdateMaxHP))));
    // HP_Text->SetText(FText::FromString(FString::FromInt(static_cast<int32>(CurrentHP))));
    HPPercent->SetPercent(CurrentHP / MaxHP);
}
