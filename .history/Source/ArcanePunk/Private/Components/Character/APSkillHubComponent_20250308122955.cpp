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

void UAPSkillHubComponent::PressQ()
{
	if(!OwnerCharacter.IsValid()) return;

	SkillState = ESkillKey::Q;
	PlayBasicSkill();
}
void UAPSkillHubComponent::PressE()
{
	if(!OwnerCharacter.IsValid()) return;

	SkillState = ESkillKey::E;
	PlayBasicSkill();
}
void UAPSkillHubComponent::PressR() // R
{
	if(!OwnerCharacter.IsValid()) return;

	SkillState = ESkillKey::R;
	PlayBasicSkill();
}

void UAPSkillHubComponent::PlayBasicSkill()
{
	if(!OwnerCharacter.IsValid()) return;

	// switch(SkillState)
	// {
	// case ESkillKey::Q:
	// 	if(OwnerCharacter->GetQSkillNumber()) OwnerCharacter->GetQSkillNumber()->PlaySkill();
	// 	break;

	// case ESkillKey::E:
	// 	if(OwnerCharacter->GetESkillNumber()) OwnerCharacter->GetESkillNumber()->PlaySkill();
	// 	break;

	// case ESkillKey::R:
	// 	if(OwnerCharacter->GetRSkillNumber()) OwnerCharacter->GetRSkillNumber()->PlaySkill();
	// 	break;

	// }
	// LastSkill = SkillState;
	// SkillState = ESkillKey::None;
}


void UAPSkillHubComponent::UpdatingSkill_Q()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter.IsValid()) return;

	// switch (OwnerCharacter->GetQSkill())
	// {
	// 	case ESkillNumber::Skill_1:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber1>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_2:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber2>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_3:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber3>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_4:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber4>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_5:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber5>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_6:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber6>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_7:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber7>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_8:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber8>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_9:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber9>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_10:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber10>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_11:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber11>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_12:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber12>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_13:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber13>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_14:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber14>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_15:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber15>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_16:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber16>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_17:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber17>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_18:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber18>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_19:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber19>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_20:
	// 	OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber20>(GetOwner()));
	// 	break;
	// }
	// if(!OwnerCharacter->GetQSkillNumber()) return;
	// OwnerCharacter->GetQSkillNumber()->RegisterAllComponentTickFunctions(true);
	// OwnerCharacter->GetQSkillNumber()->RegisterComponent();
	// OwnerCharacter->GetQSkillNumber()->SetSkillKey(ESkillKey::Q);
	// OwnerCharacter->GetQSkillNumber()->SetCurrentSkillNumber(OwnerCharacter->GetQSkill());

	// OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateSkillSlot(ESkillKey::Q, (uint8)OwnerCharacter->GetQSkill());
}

void UAPSkillHubComponent::UpdatingSkill_E()
{
	// OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	// if(!OwnerCharacter.IsValid()) return;

	// switch (OwnerCharacter->GetESkill())
	// {
	// 	case ESkillNumber::Skill_1:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber1>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_2:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber2>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_3:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber3>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_4:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber4>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_5:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber5>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_6:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber6>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_7:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber7>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_8:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber8>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_9:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber9>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_10:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber10>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_11:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber11>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_12:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber12>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_13:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber13>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_14:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber14>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_15:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber15>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_16:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber16>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_17:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber17>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_18:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber18>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_19:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber19>(GetOwner()));
	// 	break;

	// 	case ESkillNumber::Skill_20:
	// 	OwnerCharacter->SetESkillNumber(NewObject<USkillNumber20>(GetOwner()));
	// 	break;
	// }
	// if(!OwnerCharacter->GetESkillNumber()) return;
	// OwnerCharacter->GetESkillNumber()->RegisterAllComponentTickFunctions(true);
	// OwnerCharacter->GetESkillNumber()->RegisterComponent();
	// OwnerCharacter->GetESkillNumber()->SetSkillKey(ESkillKey::E);
	// OwnerCharacter->GetESkillNumber()->SetCurrentSkillNumber(OwnerCharacter->GetESkill());
	
	// OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateSkillSlot(ESkillKey::E, (uint8)OwnerCharacter->GetESkill());
}

void UAPSkillHubComponent::UpdatingSkill_R()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter.IsValid()) return;

	switch (OwnerCharacter->GetRSkill())
	{
		case EUltSkillNumber::UltSkill_1:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_1>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_2:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_2>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_3:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_3>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_4:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_4>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_5:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_5>(GetOwner()));
		break;
		
		case EUltSkillNumber::UltSkill_6:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_6>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_7:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_7>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_8:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_8>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_9:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_9>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_10:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_10>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_11:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_11>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_12:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_12>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_13:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_13>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_14:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_14>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_15:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_15>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_16:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_16>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_17:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_17>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_18:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_18>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_19:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_19>(GetOwner()));
		break;

		case EUltSkillNumber::UltSkill_20:
		OwnerCharacter->SetRSkillNumber(NewObject<UUltSkillNumber_20>(GetOwner()));
		break;
	}
	if(!OwnerCharacter->GetRSkillNumber()) return;
	OwnerCharacter->GetRSkillNumber()->RegisterAllComponentTickFunctions(true);
	OwnerCharacter->GetRSkillNumber()->RegisterComponent();
	OwnerCharacter->GetRSkillNumber()->SetSkillKey(ESkillKey::R);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateSkillSlot(ESkillKey::R, (uint8)OwnerCharacter->GetRSkill());
}

USkillNumberBase* UAPSkillHubComponent::CreateSkillNumber(ESkillNumber CreatedNumber)
{
	switch (CreatedNumber)
	{
		case ESkillNumber::Skill_1:
		return NewObject<USkillNumber1>(GetOwner());
		break;

		case ESkillNumber::Skill_2:
		return NewObject<USkillNumber2>(GetOwner());
		break;

		case ESkillNumber::Skill_3:
		return NewObject<USkillNumber3>(GetOwner());
		break;

		case ESkillNumber::Skill_4:
		return NewObject<USkillNumber4>(GetOwner());
		break;

		case ESkillNumber::Skill_5:
		return NewObject<USkillNumber5>(GetOwner());
		break;

		case ESkillNumber::Skill_6:
		return NewObject<USkillNumber6>(GetOwner());
		break;

		case ESkillNumber::Skill_7:
		return NewObject<USkillNumber7>(GetOwner());
		break;

		case ESkillNumber::Skill_8:
		return NewObject<USkillNumber8>(GetOwner());
		break;

		case ESkillNumber::Skill_9:
		return NewObject<USkillNumber9>(GetOwner());
		break;

		case ESkillNumber::Skill_10:
		return NewObject<USkillNumber10>(GetOwner());
		break;

		case ESkillNumber::Skill_11:
		return NewObject<USkillNumber11>(GetOwner());
		break;

		case ESkillNumber::Skill_12:
		return NewObject<USkillNumber12>(GetOwner());
		break;

		case ESkillNumber::Skill_13:
		return NewObject<USkillNumber13>(GetOwner());
		break;

		case ESkillNumber::Skill_14:
		return NewObject<USkillNumber14>(GetOwner());
		break;

		case ESkillNumber::Skill_15:
		return NewObject<USkillNumber15>(GetOwner());
		break;

		case ESkillNumber::Skill_16:
		return NewObject<USkillNumber16>(GetOwner());
		break;

		case ESkillNumber::Skill_17:
		return NewObject<USkillNumber17>(GetOwner());
		break;

		case ESkillNumber::Skill_18:
		return NewObject<USkillNumber18>(GetOwner());
		break;

		case ESkillNumber::Skill_19:
		return NewObject<USkillNumber19>(GetOwner());
		break;

		case ESkillNumber::Skill_20:
		return NewObject<USkillNumber20>(GetOwner());
		break;
	}
	return nullptr;
	
	// OwnerCharacter->GetAPHUD()->OnUpdateSkillSlot.Broadcast(ESkillKey::E, (uint8)OwnerCharacter->GetESkill());
}

USkillNumberBase* UAPSkillHubComponent::GetSKillNumberComp(ESkillNumber SkillNumber)
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter.IsValid()) return nullptr;

	if(SkillNumber == OwnerCharacter->GetQSkill())
	{
		return OwnerCharacter->GetQSkillNumber();
	}
	else if(SkillNumber == OwnerCharacter->GetESkill())
	{
		return OwnerCharacter->GetESkillNumber();
	}
	return nullptr;
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

