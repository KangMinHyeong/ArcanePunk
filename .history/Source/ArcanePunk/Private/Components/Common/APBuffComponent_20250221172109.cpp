
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

void UAPBuffComponent::BindBuffComp()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	
	OwnerCharacter->OnUpdateOriginStatus.RemoveDynamic(this, &UAPBuffComponent::UpdateStatus);
	OwnerCharacter->OnUpdateOriginStatus.AddDynamic(this, &UAPBuffComponent::UpdateStatus);
}

void UAPBuffComponent::UpdateStatus()
{
	if(!OwnerCharacter.IsValid()) return;
	auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 

	OwnerCharacter->SetDefaultATKSpeed(PD.StatusData.ATKSpeed * CurrentATKSpeedCoefficient); 
	OwnerCharacter->SetDefaultATK(PD.StatusData.ATK * CurrentATKCoefficient); 
	OwnerCharacter->SetDefaultMaxHP(PD.StatusData.MaxHP * CurrentMaxHPCoefficient); 
	OwnerCharacter->SetDefaultHP(OwnerCharacter->GetDefaultHP() * CurrentMaxHPCoefficient); 
	OwnerCharacter->SetDefaultCriticalPercent(PD.StatusData.CriticalPercent * CurrentCriticalPercentCoefficient); 
	OwnerCharacter->SetDefaultCriticalDamage(PD.StatusData.CriticalDamageCoefficient * CurrentCriticalDamageCoefficient); 
	OwnerCharacter->SetDefaultDEF(PD.StatusData.DEF * CurrentDEFCoefficient); 
	OwnerCharacter->UpdateStatus();
}

void UAPBuffComponent::ATKSpeedUp(float ATKSpeedCoefficient, bool Start)
{
	if(!OwnerCharacter.IsValid()) return;

	auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 

	if(Start) {CurrentATKSpeedCoefficient = CurrentATKSpeedCoefficient + ATKSpeedCoefficient;}
	else {CurrentATKSpeedCoefficient = CurrentATKSpeedCoefficient - ATKSpeedCoefficient;}

	PD.StatusData.ATKSpeed = PD.StatusData.ATKSpeed * CurrentATKSpeedCoefficient;

	OwnerCharacter->SetDefaultATKSpeed(PD.StatusData.ATKSpeed); 
}

void UAPBuffComponent::ATKSpeedUp(float ATKSpeedCoefficient, float Time)
{
	if(!OwnerCharacter.IsValid()) return;

	auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 

	CurrentATKSpeedCoefficient = CurrentATKSpeedCoefficient + ATKSpeedCoefficient;
	PD.StatusData.ATKSpeed = PD.StatusData.ATKSpeed * CurrentATKSpeedCoefficient;

	OwnerCharacter->SetDefaultATKSpeed(PD.StatusData.ATKSpeed); 
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPBuffComponent::ATKSpeedUpEnd, &TimerHandle, ATKSpeedCoefficient);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Time, false);
}

void UAPBuffComponent::ATKSpeedUpEnd(FTimerHandle *TimerHandle, float ATKSpeedCoefficient)
{
	if(!OwnerCharacter.IsValid()) return;

	auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 

	CurrentATKSpeedCoefficient = CurrentATKSpeedCoefficient - ATKSpeedCoefficient;
	PD.StatusData.ATKSpeed = PD.StatusData.ATKSpeed * CurrentATKSpeedCoefficient;
	OwnerCharacter->SetDefaultATKSpeed(PD.StatusData.ATKSpeed); 

	GetWorld()->GetTimerManager().ClearTimer(*TimerHandle);
}

void UAPBuffComponent::ATKUp(float ATKCoefficient, bool Start)
{
	if(!OwnerCharacter.IsValid()) return;

	auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 

	if(Start) {CurrentATKCoefficient = CurrentATKCoefficient + ATKCoefficient;}
	else {CurrentATKCoefficient = CurrentATKCoefficient - ATKCoefficient;}
	
	PD.StatusData.ATK = PD.StatusData.ATK * CurrentATKCoefficient;

	OwnerCharacter->SetDefaultATK(PD.StatusData.ATK); 
}

void UAPBuffComponent::ATKUp(float ATKCoefficient, float Time)
{
	if(!OwnerCharacter.IsValid()) return;

	auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 

	CurrentATKCoefficient = CurrentATKCoefficient + ATKCoefficient;
	PD.StatusData.ATK = PD.StatusData.ATK * CurrentATKCoefficient;

	OwnerCharacter->SetDefaultATK(PD.StatusData.ATK); 
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPBuffComponent::ATKUpEnd, &TimerHandle, ATKCoefficient);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Time, false);
}

void UAPBuffComponent::ATKUpEnd(FTimerHandle *TimerHandle, float ATKCoefficient)
{
	if(!OwnerCharacter.IsValid()) return;

	auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 

	CurrentATKCoefficient = CurrentATKCoefficient - ATKCoefficient;
	PD.StatusData.ATK = PD.StatusData.ATK * CurrentATKCoefficient;
	
	OwnerCharacter->SetDefaultATK(PD.StatusData.ATK); 

	GetWorld()->GetTimerManager().ClearTimer(*TimerHandle);
}

void UAPBuffComponent::MaxHPUp(float MaxHPCoefficient, bool Start)
{
}
