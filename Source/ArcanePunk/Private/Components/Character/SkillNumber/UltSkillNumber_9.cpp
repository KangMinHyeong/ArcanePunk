
#include "Components/Character/SkillNumber/UltSkillNumber_9.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/CarpetBoom.h"

UUltSkillNumber_9::UUltSkillNumber_9()
{
	OriginCoolTime = 7.0f;

    UltSkill9_LimitDistance = 1800.0f;
	UltSkill9_Width = 500.0f;	
	UltSkill9_Length = 2400.0f;
}

void UUltSkillNumber_9::BeginPlay()
{
	Super::BeginPlay();

}


void UUltSkillNumber_9::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_Skill();
	}
	else
	{
		if(OwnerCharacter->GetPlayerStatus().PlayerDynamicData.MP <= 0 || !CheckSkillCool(SkillKey)) {OwnerCharacterPC->DisplayNotEnoughMPUI(); return;}
		OwnerCharacter->SetDoing(true);
        Skilling = true;
        Spawn_UltSkill9();
	}
}

void UUltSkillNumber_9::Spawn_UltSkill9()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

    if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick_Ult();}
	
	ActivateSkillRange_Target(UltSkill9_Width, UltSkill9_Length*0.5f, ESkillRangeType::Square); // 최대 길이 0.5배 해줘야지 같은 거리로 뒤로 밀림
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(UltSkill9_LimitDistance*0.25f); // 최대 사거리 0.25배 해줘야지 같은 거리로 뒤로 밀림
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData);	

	if(CheckSmartKey(SkillKey))
	{
		OwnerCharacterPC->bShowMouseCursor = true;
		CursorImmediately();
		SkillRange_Target->SetActorHiddenInGame(true);
	}

	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void UUltSkillNumber_9::OnSkill()
{
	Super::OnSkill();
    SetComponentTickEnabled(false);
	if(!OwnerCharacter.IsValid()) return; OwnerCharacter->SetDoing(true);
    
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	OwnerAnim->PlayUltSkill_9_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);

    SetBoomLocation();
    CharacterRotation();
}

void UUltSkillNumber_9::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	CarpetBoom = GetWorld()->SpawnActor<ACarpetBoom>(OwnerCharacter->GetAPSkillHubComponent()->GetCarpetBoom(), SpawnLocation , OwnerCharacter->GetActorRotation());
	if(!CarpetBoom.IsValid()) return; 
    CarpetBoom->SetOwner(OwnerCharacter.Get());
    CarpetBoom->SetCarpetBoomWidth(UltSkill9_Width);
	CarpetBoom->SetCarpetBoomLength(UltSkill9_Length); 
    CarpetBoom->SetBoomTargetLocation(TargetLocation);
	CarpetBoom->SetSkill(SkillAbilityNestingData);	

	Remove_Skill();
}

void UUltSkillNumber_9::SkillEnd()
{
}

void UUltSkillNumber_9::UpdateSkillData()
{
}

void UUltSkillNumber_9::SetBoomLocation()
{
    if(!OwnerCharacter.IsValid()) return;

    SpawnLocation = OwnerCharacter->GetMesh()->GetComponentLocation() - SkillRange_Target->GetDecalComponent()->GetComponentLocation();
    FVector Unit = SpawnLocation/SpawnLocation.Size(); 
    
    SpawnLocation = Unit * (UltSkill9_Length - UltSkill9_LimitDistance) + OwnerCharacter->GetMesh()->GetComponentLocation();
    TargetLocation = SpawnLocation - Unit * UltSkill9_Length;    
}