#include "Components/Character/APSkillHubComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "UserInterface/APHUD.h"
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
	if(OwnerCharacter->GetDoing()) return;

	SkillState = ESkillKey::Q;
	PlayBasicSkill();
	// if(SkillState == ESkillKey::None)
	// {
	// 	SkillState = ESkillKey::Q;
	// 	if(ActivationCheck(OwnerCharacter->GetQSkill())) {PlayBasicSkill();}
	// 	else {GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);}
	// }
	// else
	// {
	// 	ESkillKey Second = ESkillKey::Q;
	// 	GetWorld()->GetTimerManager().ClearTimer(SkillCancleTimerHandle);
	// 	AddSkillState(SkillState);		
	// 	SkillDetermine(SkillState, Second);
	// }
}

void UAPSkillHubComponent::PressE()
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->GetDoing()) return;

	SkillState = ESkillKey::E;
	PlayBasicSkill();
	// if(SkillState == ESkillKey::None)
	// {
	// 	SkillState = ESkillKey::E;
	// 	if(ActivationCheck(OwnerCharacter->GetESkill())) {PlayBasicSkill();}
	// 	else {GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);}
	// }
	// else
	// {
	// 	ESkillKey Second = ESkillKey::E;
	// 	GetWorld()->GetTimerManager().ClearTimer(SkillCancleTimerHandle);
	// 	AddSkillState(SkillState);		
	// 	SkillDetermine(SkillState, Second);
	// }
}

void UAPSkillHubComponent::PressSpace() // R
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->GetDoing()) return;

	SkillState = ESkillKey::R;
	PlayBasicSkill();
	// if(SkillState == ESkillKey::None)
	// {
	// 	SkillState = ESkillKey::R;
	// 	// R도 후에 만들어주기
	// 	GetWorld()->GetTimerManager().SetTimer(SkillCancleTimerHandle, this, &UAPSkillHubComponent::PlayBasicSkill, OwnerCharacter->GetSkillCancelTime(), false);
	// }
	// else
	// {
	// 	ESkillKey Second = ESkillKey::R;
	// 	GetWorld()->GetTimerManager().ClearTimer(SkillCancleTimerHandle);
	// 	AddSkillState(SkillState);		
	// 	SkillDetermine(SkillState, Second);
	// }
}

void UAPSkillHubComponent::PressShift()
{

}

// void UAPSkillHubComponent::SetSkillState(ESkillKey NewValue)
// {
// 	if(SkillStateArr.IsEmpty())
// 	{
// 		SkillState = NewValue;
// 	}
// 	else
// 	{
// 		SkillState = SkillStateArr.Top();
// 	}
// }

// void UAPSkillHubComponent::AddSkillState(ESkillKey NewValue)
// {
// 	SkillStateArr.Add(NewValue);
// }

// void UAPSkillHubComponent::RemoveSkillState(ESkillKey NewValue)
// {
// 	if(!SkillStateArr.IsEmpty())
// 	{
// 		SkillStateArr.Pop();
// 		SetSkillState(NewValue);
// 	}
// }

ESkillKey UAPSkillHubComponent::GetSkillState()
{
    return SkillState;
}

// void UAPSkillHubComponent::SkillDetermine(ESkillKey First, ESkillKey Second)
// {
// 	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
// 	if(!OwnerCharacter) return;

// 	LastSkill = First;
	
// 	switch(First)
// 	{
// 		case ESkillKey::Q:
// 		QComp->SkillBase_Q(Second);
// 		break;

// 		case ESkillKey::E:
// 		EComp->SkillBase_E(Second);
// 		break;

// 		case ESkillKey::R: // R
// 		SpaceComp->SkillBase_Space(Second);
// 		break;
// 	}
// }

void UAPSkillHubComponent::PlayBasicSkill()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	OwnerCharacterPC->RemoveOtherClick();
	switch(SkillState)
	{
		case ESkillKey::Q:
		if(OwnerCharacter->GetQSkillNumber()) OwnerCharacter->GetQSkillNumber()->PlaySkill();
		// CastSkillNumber(OwnerCharacter->GetQSkill(), ESkillKey::Q);
		break;

		case ESkillKey::E:
		if(OwnerCharacter->GetESkillNumber()) OwnerCharacter->GetESkillNumber()->PlaySkill();
		// CastSkillNumber(OwnerCharacter->GetESkill(), ESkillKey::E);
		break;

		case ESkillKey::R:
		if(OwnerCharacter->GetRSkillNumber()) OwnerCharacter->GetRSkillNumber()->PlaySkill();
		// CastUltSkillNumber(OwnerCharacter->GetRSkill(), ESkillKey::R);
		break;

		case 4:
		// Shift 공석
		break;
	}
	LastSkill = SkillState;
	SkillState = ESkillKey::None;
}

// void UAPSkillHubComponent::CastSkillNumber(ESkillNumber SkillNumber, ESkillKey WhichKey)
// {
// 	switch (SkillNumber)
// 	{
// 		case ESkillNumber::Skill_1:
// 		SkillNumComp->GetSkillNumber1()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_2:
// 		SkillNumComp->GetSkillNumber2()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_3:
// 		SkillNumComp->GetSkillNumber3()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_4:
// 		SkillNumComp->GetSkillNumber4()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_5:
// 		SkillNumComp->GetSkillNumber5()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_6:
// 		SkillNumComp->GetSkillNumber6()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_7:
// 		SkillNumComp->GetSkillNumber7()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_8:
// 		SkillNumComp->GetSkillNumber8()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_9:
// 		SkillNumComp->GetSkillNumber9()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_10:
// 		SkillNumComp->GetSkillNumber10()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_11:
// 		SkillNumComp->GetSkillNumber11()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_12:
// 		SkillNumComp->GetSkillNumber12()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_13:
// 		SkillNumComp->GetSkillNumber13()->PlaySkill(WhichKey);
// 		break;

// 		case ESkillNumber::Skill_14:
// 		SkillNumComp->GetSkillNumber14()->PlaySkill(WhichKey);
// 		break;
// 	}
// }

// void UAPSkillHubComponent::CastUltSkillNumber(EUltSkillNumber UltSkillNumber, ESkillKey WhichKey)
// {
// 	switch (UltSkillNumber)
// 	{
// 		case EUltSkillNumber::UltSkill_1:
// 		SkillNumComp->GetUltSkillNumber1()->PlaySkill(WhichKey);
// 		break;

// 	}	
// }

// bool UAPSkillHubComponent::ActivationCheck(ESkillNumber SkillNumber)
// {
// 	switch (SkillNumber)
// 	{
// 		case ESkillNumber::Skill_1:
// 		return SkillNumComp->GetSkillNumber1()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_2:
// 		return SkillNumComp->GetSkillNumber2()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_3:
// 		return SkillNumComp->GetSkillNumber3()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_4:
// 		return SkillNumComp->GetSkillNumber4()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_5:
// 		return SkillNumComp->GetSkillNumber5()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_6:
// 		return SkillNumComp->GetSkillNumber6()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_7:
// 		return SkillNumComp->GetSkillNumber7()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_8:
// 		return SkillNumComp->GetSkillNumber8()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_9:
// 		return SkillNumComp->GetSkillNumber9()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_10:
// 		return SkillNumComp->GetSkillNumber10()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_11:
// 		return SkillNumComp->GetSkillNumber11()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_12:
// 		return SkillNumComp->GetSkillNumber12()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_13:
// 		return SkillNumComp->GetSkillNumber13()->bActivate;
// 		break;

// 		case ESkillNumber::Skill_14:
// 		return SkillNumComp->GetSkillNumber14()->bActivate;
// 		break;
// 	}
// 	return false;
// }


void UAPSkillHubComponent::UpdateSkill_Q()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter.IsValid()) return;

	switch (OwnerCharacter->GetQSkill())
	{
		case ESkillNumber::Skill_1:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber1>(GetOwner()));
		break;

		case ESkillNumber::Skill_2:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber2>(GetOwner()));
		break;

		case ESkillNumber::Skill_3:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber3>(GetOwner()));
		break;

		case ESkillNumber::Skill_4:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber4>(GetOwner()));
		break;

		case ESkillNumber::Skill_5:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber5>(GetOwner()));
		break;

		case ESkillNumber::Skill_6:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber6>(GetOwner()));
		break;

		case ESkillNumber::Skill_7:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber7>(GetOwner()));
		break;

		case ESkillNumber::Skill_8:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber8>(GetOwner()));
		break;

		case ESkillNumber::Skill_9:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber9>(GetOwner()));
		break;

		case ESkillNumber::Skill_10:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber10>(GetOwner()));
		break;

		case ESkillNumber::Skill_11:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber11>(GetOwner()));
		break;

		case ESkillNumber::Skill_12:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber12>(GetOwner()));
		break;

		case ESkillNumber::Skill_13:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber13>(GetOwner()));
		break;

		case ESkillNumber::Skill_14:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber14>(GetOwner()));
		break;

		case ESkillNumber::Skill_15:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber15>(GetOwner()));
		break;

		case ESkillNumber::Skill_16:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber16>(GetOwner()));
		break;

		case ESkillNumber::Skill_17:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber17>(GetOwner()));
		break;

		case ESkillNumber::Skill_18:
		OwnerCharacter->SetQSkillNumber(NewObject<USkillNumber18>(GetOwner()));
		break;
	}
	if(!OwnerCharacter->GetQSkillNumber()) return;
	OwnerCharacter->GetQSkillNumber()->RegisterAllComponentTickFunctions(true);
	OwnerCharacter->GetQSkillNumber()->RegisterComponent();
	OwnerCharacter->GetQSkillNumber()->SkillKey = ESkillKey::Q;
	OwnerCharacter->GetAPHUD()->OnUpdateSkillSlot.Broadcast(ESkillKey::Q, (uint8)OwnerCharacter->GetQSkill());
}

void UAPSkillHubComponent::UpdateSkill_E()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter.IsValid()) return;

	switch (OwnerCharacter->GetESkill())
	{
		case ESkillNumber::Skill_1:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber1>(GetOwner()));
		break;

		case ESkillNumber::Skill_2:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber2>(GetOwner()));
		break;

		case ESkillNumber::Skill_3:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber3>(GetOwner()));
		break;

		case ESkillNumber::Skill_4:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber4>(GetOwner()));
		break;

		case ESkillNumber::Skill_5:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber5>(GetOwner()));
		break;

		case ESkillNumber::Skill_6:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber6>(GetOwner()));
		break;

		case ESkillNumber::Skill_7:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber7>(GetOwner()));
		break;

		case ESkillNumber::Skill_8:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber8>(GetOwner()));
		break;

		case ESkillNumber::Skill_9:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber9>(GetOwner()));
		break;

		case ESkillNumber::Skill_10:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber10>(GetOwner()));
		break;

		case ESkillNumber::Skill_11:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber11>(GetOwner()));
		break;

		case ESkillNumber::Skill_12:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber12>(GetOwner()));
		break;

		case ESkillNumber::Skill_13:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber13>(GetOwner()));
		break;

		case ESkillNumber::Skill_14:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber14>(GetOwner()));
		break;

		case ESkillNumber::Skill_15:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber15>(GetOwner()));
		break;

		case ESkillNumber::Skill_16:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber16>(GetOwner()));
		break;

		case ESkillNumber::Skill_17:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber17>(GetOwner()));
		break;

		case ESkillNumber::Skill_18:
		OwnerCharacter->SetESkillNumber(NewObject<USkillNumber18>(GetOwner()));
		break;
	}
	if(!OwnerCharacter->GetESkillNumber()) return;
	OwnerCharacter->GetESkillNumber()->RegisterAllComponentTickFunctions(true);
	OwnerCharacter->GetESkillNumber()->RegisterComponent();
	OwnerCharacter->GetESkillNumber()->SkillKey = ESkillKey::E;
	OwnerCharacter->GetAPHUD()->OnUpdateSkillSlot.Broadcast(ESkillKey::E, (uint8)OwnerCharacter->GetESkill());
}

void UAPSkillHubComponent::UpdateSkill_R()
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
	}
	if(!OwnerCharacter->GetRSkillNumber()) return;
	OwnerCharacter->GetRSkillNumber()->RegisterAllComponentTickFunctions(true);
	OwnerCharacter->GetRSkillNumber()->RegisterComponent();
	OwnerCharacter->GetRSkillNumber()->SkillKey = ESkillKey::R;
	OwnerCharacter->GetAPHUD()->OnUpdateSkillSlot.Broadcast(ESkillKey::R, (uint8)OwnerCharacter->GetRSkill());
}


USkillNumberBase* UAPSkillHubComponent::GetSKillNumberComponent(ESkillNumber SkillNumber)
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
	
	if(PD.PlayerDynamicData.MaxMP == PD.PlayerDynamicData.MP)
	{
		GetWorld()->GetTimerManager().ClearTimer(RecoveryMPTimerHandle);
		Proceeding = false;
	}
	else
	{
		OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(1, false);
		if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MaxMP == OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP)
		{
			GetWorld()->GetTimerManager().ClearTimer(RecoveryMPTimerHandle);
			Proceeding = false;
		}
	}
}