#include "Components/Character/APSkillHubComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "UserInterface/HUD/APHUD.h"
#include "PlayerController/ArcanePunkPlayerController.h"

UAPSkillHubComponent::UAPSkillHubComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPSkillHubComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->OnAutoRecoveryMPDelegate.AddUObject(this, &UAPSkillHubComponent::AutoRecoveryMP);
	OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerCharacterPC.IsValid()) return;
}

void UAPSkillHubComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSkillHubComponent::AutoRecoveryMP()
{
	if(Proceeding) {return;}
	else {Proceeding = true; }
	GetWorld()->GetTimerManager().SetTimer(RecoveryMPTimerHandle, this, &UAPSkillHubComponent::RecoveryMP, RecoveryTime_MP, true);	
}

void UAPSkillHubComponent::RecoveryMP()
{
	if(!OwnerCharacter.IsValid()) return; const auto PD = OwnerCharacter->GetPlayerStatus();
	
	if(PD.StatusData.MaxMP == PD.StatusData.MP)
	{
		GetWorld()->GetTimerManager().ClearTimer(RecoveryMPTimerHandle);
		Proceeding = false;
	}
	else
	{
		OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(1, false);
		if(OwnerCharacter->GetPlayerStatus().StatusData.MaxMP == OwnerCharacter->GetPlayerStatus().StatusData.MP)
		{
			GetWorld()->GetTimerManager().ClearTimer(RecoveryMPTimerHandle);
			Proceeding = false;
		}
	}
}

