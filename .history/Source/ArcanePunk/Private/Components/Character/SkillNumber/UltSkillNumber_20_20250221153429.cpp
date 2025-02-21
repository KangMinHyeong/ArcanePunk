
#include "Components/Character/SkillNumber/UltSkillNumber_20.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/Imitator.h"
#include "GameElements/Trigger/BattleSection/APBattleSectionBase.h"
#include "EngineUtils.h"

UUltSkillNumber_20::UUltSkillNumber_20()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_20");

	SkillDuration = 25.0f;
	SpawnNum = 6;
}

void UUltSkillNumber_20::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_20::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	
	if(Skilling)
	{
        if(!bActivate) {return;}
		else {Activate_Skill();}
	}
	else
	{
		if(!CheckSkillCondition()) return;
		OwnerCharacter->SetDoing(true);
		Skilling = true;
        OnSkill();
	}
}

void UUltSkillNumber_20::OnSkill()
{
	if(!OwnerCharacter.IsValid()) return; 
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	// OwnerCharacter->GetAPHUD()->StartImitatorSlot();
    // OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);


	OwnerCharacter->SetDoing(false);
	RemainSpawn = SpawnNum;
	SpawnTime = SpawnTotalTime / SpawnNum;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UUltSkillNumber_20::Activate_UltSkill_20, SpawnTime, true);
	GetWorld()->GetTimerManager().SetTimer(UltSkill20_TimerHandle, this, &UUltSkillNumber_20::SkillEnd, SkillDuration, false);
}

void UUltSkillNumber_20::Activate_UltSkill_20()
{
	if(!OwnerCharacter.IsValid() || RemainSpawn == 0) {GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle); return;}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SetImitatorLocation();
	auto Imitator = GetWorld()->SpawnActor<AImitator>(OwnerCharacter->GetAPSkillHubComponent()->GetImitator(), SpawnLocation, OwnerCharacter->GetActorRotation(), SpawnParams);
	if(!Imitator) return;
	Imitator->SetOwner(OwnerCharacter.Get());
	Imitator->SetSkill(SkillAbilityNestingData, this);	
	Imitators.Add(Imitator);
	
    RemainSpawn--;
}

void UUltSkillNumber_20::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return; if(!CopySkill.IsValid()) return;
	
	OwnerCharacter->GetAPSkillHubComponent()->LastSkill = CopySkill->GetSkillKey();
	CopySkill->PlaySkill();
}

void UUltSkillNumber_20::SkillEnd()
{	
	Skilling = false;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
	// OwnerCharacter->GetAPHUD()->EndImitatorSlot();

	CopySkillEnded(SkillKey, CurrentSkillNumber);

	for(auto Imitator : Imitators) {if(Imitator) Imitator->Destroy();}
}

void UUltSkillNumber_20::SetImitatorLocation()
{
	// TArray<AAPBattleSectionBase*> BattleSections;
	// for(auto BattleSection : TActorRange<AAPBattleSectionBase>(GetWorld()))
	// {
	// 	BattleSections.Add(BattleSection);
	// }

	// if(BattleSections.IsEmpty()) return;
	// if(BattleSections.Num() >= 2)
	// {
	// 	BattleSections.Sort([this](AActor& A, AActor& B) 
	// 	{
	// 		return (A.GetActorLocation() - GetOwner()->GetActorLocation()).Size() > (B.GetActorLocation() - GetOwner()->GetActorLocation()).Size();
	// 	});
	// }
	// FVector Spawn_1 = BattleSections.Top()->GetSpawnRoot()->GetComponentLocation();
	// FVector Spawn_2 = BattleSections.Top()->GetSpawnRoot2()->GetComponentLocation();
	
	// SpawnLocation = FVector(FMath::RandRange(Spawn_1.X, Spawn_2.X), FMath::RandRange(Spawn_1.Y, Spawn_2.Y), OwnerCharacter->GetMesh()->GetComponentLocation().Z);

}

void UUltSkillNumber_20::SetCopySkill(ESkillNumber CopySkillNum)
{
	bInit = true;
	CopySkillEnded(SkillKey, CurrentSkillNumber);
	bInit = false;

	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->SetSpareSkillNumber(OwnerCharacter->GetAPSkillHubComponent()->CreateSkillNumber(CopySkillNum));

	if(OwnerCharacter->GetSpareSkillNumber())
	{
		CopySkill = OwnerCharacter->GetSpareSkillNumber();
		CopySkill->RegisterAllComponentTickFunctions(true);
		CopySkill->RegisterComponent();
		CopySkill->SetSkillKey(ESkillKey::None);
		CopySkill->SetMPConsumption(0);
		CopySkill->SetCurrentSkillNumber(CopySkillNum);
		

		CurrentSkillNumber = CopySkillNum;
		bActivate = true;

		OwnerCharacter->OnSkillEndWithSkillKey.AddDynamic(this, &UUltSkillNumber_20::CopySkillEnded);
		// OwnerCharacter->GetAPHUD()->UpdateImitatorSlot((uint8)CopySkillNum);
	}
}

void UUltSkillNumber_20::CopySkillEnded(ESkillKey UpdateSkillKey, ESkillNumber CopySkillNum)
{
	if(UpdateSkillKey == ESkillKey::Q || UpdateSkillKey == ESkillKey::E) return;
	if(CurrentSkillNumber != CopySkillNum) return;

	bActivate = false;
	if(CopySkill.IsValid()) CopySkill->Remove_Skill();
	CopySkill = nullptr;

	OwnerCharacter->OnSkillEndWithSkillKey.RemoveDynamic(this, &UUltSkillNumber_20::CopySkillEnded);
	// if(!bInit) OwnerCharacter->GetAPHUD()->UpdateImitatorSlot();
}

void UUltSkillNumber_20::UpdateSkillData()
{

}

