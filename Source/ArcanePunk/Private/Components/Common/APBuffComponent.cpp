
#include "Components/Common/APBuffComponent.h"

#include "Character/ArcanePunkCharacter.h"

UAPBuffComponent::UAPBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPBuffComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAPBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAPBuffComponent::ATKSpeedUp(float ATKSpeedCoefficient, bool Start)
{
	auto Owner = Cast<AArcanePunkCharacter>(GetOwner()); if(!Owner) return;

	auto PD = Owner->GetPlayerStatus_Origin(); 

	if(Start) {CurrentATKSpeedCoefficient = CurrentATKSpeedCoefficient + ATKSpeedCoefficient;}
	else {CurrentATKSpeedCoefficient = CurrentATKSpeedCoefficient - ATKSpeedCoefficient;}

	PD.PlayerDynamicData.ATKSpeed = PD.PlayerDynamicData.ATKSpeed * CurrentATKSpeedCoefficient;

	Owner->SetDefaultATKSpeed(PD.PlayerDynamicData.ATKSpeed); 
}

void UAPBuffComponent::ATKSpeedUp(float ATKSpeedCoefficient, float Time)
{
	auto Owner = Cast<AArcanePunkCharacter>(GetOwner()); if(!Owner) return;

	auto PD = Owner->GetPlayerStatus_Origin(); 

	CurrentATKSpeedCoefficient = CurrentATKSpeedCoefficient + ATKSpeedCoefficient;
	PD.PlayerDynamicData.ATKSpeed = PD.PlayerDynamicData.ATKSpeed * CurrentATKSpeedCoefficient;

	Owner->SetDefaultATKSpeed(PD.PlayerDynamicData.ATKSpeed); 
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPBuffComponent::ATKSpeedUpEnd, &TimerHandle, ATKSpeedCoefficient);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Time, false);
}

void UAPBuffComponent::ATKSpeedUpEnd(FTimerHandle *TimerHandle, float ATKSpeedCoefficient)
{
	auto Owner = Cast<AArcanePunkCharacter>(GetOwner()); if(!Owner) return;

	auto PD = Owner->GetPlayerStatus_Origin(); 

	CurrentATKSpeedCoefficient = CurrentATKSpeedCoefficient - ATKSpeedCoefficient;
	PD.PlayerDynamicData.ATKSpeed = PD.PlayerDynamicData.ATKSpeed * CurrentATKSpeedCoefficient;
	Owner->SetDefaultATKSpeed(PD.PlayerDynamicData.ATKSpeed); 

	GetWorld()->GetTimerManager().ClearTimer(*TimerHandle);
}

void UAPBuffComponent::ATKUp(float ATKCoefficient, bool Start)
{
	auto Owner = Cast<AArcanePunkCharacter>(GetOwner()); if(!Owner) return;

	auto PD = Owner->GetPlayerStatus_Origin(); 

	if(Start) {CurrentATKCoefficient = CurrentATKCoefficient + ATKCoefficient;}
	else {CurrentATKCoefficient = CurrentATKCoefficient - ATKCoefficient;}
	
	PD.PlayerDynamicData.ATK = PD.PlayerDynamicData.ATK * CurrentATKCoefficient;

	Owner->SetDefaultATK(PD.PlayerDynamicData.ATK); 
}

void UAPBuffComponent::ATKUp(float ATKCoefficient, float Time)
{
	auto Owner = Cast<AArcanePunkCharacter>(GetOwner()); if(!Owner) return;

	auto PD = Owner->GetPlayerStatus_Origin(); 

	CurrentATKCoefficient = CurrentATKCoefficient + ATKCoefficient;
	PD.PlayerDynamicData.ATK = PD.PlayerDynamicData.ATK * CurrentATKCoefficient;

	Owner->SetDefaultATK(PD.PlayerDynamicData.ATK); 
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPBuffComponent::ATKUpEnd, &TimerHandle, ATKCoefficient);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Time, false);
}

void UAPBuffComponent::ATKUpEnd(FTimerHandle *TimerHandle, float ATKCoefficient)
{
	auto Owner = Cast<AArcanePunkCharacter>(GetOwner()); if(!Owner) return;

	auto PD = Owner->GetPlayerStatus_Origin(); 

	CurrentATKCoefficient = CurrentATKCoefficient - ATKCoefficient;
	PD.PlayerDynamicData.ATK = PD.PlayerDynamicData.ATK * CurrentATKCoefficient;
	
	Owner->SetDefaultATK(PD.PlayerDynamicData.ATK); 

	GetWorld()->GetTimerManager().ClearTimer(*TimerHandle);
}
