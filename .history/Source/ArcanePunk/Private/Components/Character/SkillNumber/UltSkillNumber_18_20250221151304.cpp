
#include "Components/Character/SkillNumber/UltSkillNumber_18.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/ArcaneBlast.h"

UUltSkillNumber_18::UUltSkillNumber_18()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_18");

	MaxChargeNum_Origin = 15;
	BlastMaxTime = 1.0f;
}

void UUltSkillNumber_18::BeginPlay()
{
	Super::BeginPlay();
}

void UUltSkillNumber_18::PlaySkill()
{	
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) {SkillEnd(); return;}

	if(!CheckSkillCondition()) return;
	bActivate = true;
    OwnerCharacter->SetDoing(true);
	Skilling = true;
	OnSkill();

    MinumumRate = 0.5f / OwnerCharacter->GetPlayerStatus().StatusData.ATKSpeed;
}

void UUltSkillNumber_18::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);

    // auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	// if(!OwnerAnim) return;

	// OwnerAnim->PlayUltSkill_17_Montage();
	// OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	OwnerCharacter->GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
    OwnerCharacter->OnLeftMouseClick.AddDynamic(this, &UUltSkillNumber_18::Activate_Skill);
	OwnerCharacter->SetCanJog(false);
	GetWorld()->GetTimerManager().SetTimer(BlastEndTimerHandle, this, &UUltSkillNumber_18::SkillEnd, BlastEndTime, false);
}

void UUltSkillNumber_18::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return;
    if(!Skilling) return;
    Skilling = false;
    CurrentChargeNum--;
	OwnerCharacter->GetAPHUD()->OnOnlyChargingNumberChange.Broadcast(SkillKey);
    GetWorld()->GetTimerManager().SetTimer(BlastTimerHandle, this, &UUltSkillNumber_18::ResetSkilling, MinumumRate, false);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto ArcaneBlast = GetWorld()->SpawnActor<AArcaneBlast>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneBlast(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
    if(!ArcaneBlast) return;
	ArcaneBlast->SetOwner(OwnerCharacter.Get());
	ArcaneBlast->SetMaxDestroyTime(BlastMaxTime);
	ArcaneBlast->SetSkill(SkillAbilityNestingData, this);	
    if(CurrentChargeNum == 0) SkillEnd();
}

void UUltSkillNumber_18::ResetSkilling()
{
    GetWorld()->GetTimerManager().ClearTimer(BlastTimerHandle);
    Skilling = true;
}

void UUltSkillNumber_18::SkillEnd()
{
    GetWorld()->GetTimerManager().ClearTimer(BlastTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BlastEndTimerHandle);

	OwnerCharacter->GetCharacterMovement()->RotationRate = DefaultRotationRate;
	CurrentChargeNum = MaxChargeNum;
	OwnerCharacter->SetCanJog(true);

    OwnerCharacter->SetDoing(false);
    Remove_Skill();
	bActivate = false; 
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
	
    OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &UUltSkillNumber_18::Activate_Skill);
}

void UUltSkillNumber_18::UpdateSkillData()
{

}

void UUltSkillNumber_18::OnCoolDown()
{
	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->OnOnlyChargingNumberChange.Broadcast(SkillKey);
}
