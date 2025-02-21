
#include "Components/Character/SkillNumber/UltSkillNumber_10.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Skill/Terminator.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Components/Character/APSkillHubComponent.h"

UUltSkillNumber_10::UUltSkillNumber_10()
{
	SkillAbilityNestingData.SkillName = TEXT("UltSkill_10");
}

void UUltSkillNumber_10::BeginPlay()
{
	Super::BeginPlay();
	auto GM = Cast<AAPGameModeBattleStage>(UGameplayStatics::GetGameMode(GetWorld()));
    // if(GM) GM->OnMonsterKilled.AddUObject(this, &UUltSkillNumber_10::Activate_Skill);
}

void UUltSkillNumber_10::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(bActivate) return;

	if(!CheckSkillCondition()) return;
	bActivate = true;
    OwnerCharacter->SetDoing(true);
	Skilling = true;
	OnSkill();
}

void UUltSkillNumber_10::OnSkill()
{
    if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
	
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_10_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UUltSkillNumber_10::Activate_Skill()
{
    if(!OwnerCharacter.IsValid()) return;
	if(!bActivate) return;
	
	if(Terminator.IsValid()) 
	{
		Terminator->ResettTerminatorMode();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Terminator = GetWorld()->SpawnActor<ATerminator>(OwnerCharacter->GetAPSkillHubComponent()->GetTerminator(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
    if(!Terminator.IsValid()) return;
	Terminator->SetOwner(OwnerCharacter.Get());
	Terminator->SetSkill(SkillAbilityNestingData, this);	
    Terminator->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules ::KeepWorldTransform);

	Skilling = false;
}

void UUltSkillNumber_10::SkillEnd()
{
	bActivate = false; 
	
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
}

void UUltSkillNumber_10::UpdateSkillData()
{

}