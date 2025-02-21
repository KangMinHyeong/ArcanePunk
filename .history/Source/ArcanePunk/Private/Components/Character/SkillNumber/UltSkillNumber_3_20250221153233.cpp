
#include "Components/Character/SkillNumber/UltSkillNumber_3.h"

#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Circle.h"
#include "Skill/ArcaneExecution.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Components/Character/APSkillHubComponent.h"
#include "GameInstance/APGameInstance.h"

UUltSkillNumber_3::UUltSkillNumber_3()
{
    SkillAbilityNestingData.SkillName = TEXT("UltSkill_3");
}

void UUltSkillNumber_3::BeginPlay()
{
    Super::BeginPlay();
}

void UUltSkillNumber_3::PlaySkill()
{
	Super::PlaySkill();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	if(Skilling)
	{
		Remove_SkillRange();
	}
	else
	{
        if(bActivate)
        {
            if(!CheckSkillCool(SkillKey)) {UAPDataTableSubsystem::DisplaySystemMesseage(UAPGameInstance::GetDataTableGI(GetWorld()), EStringRowName::CannotSkill, true, true);  return;}
            OwnerCharacter->SetDoing(true);
            Skilling = true;

            Spawn_SkillRange();
        }
        else
        {
            if(!CheckSkillCondition()) return;
            OwnerCharacter->SetDoing(true);
            Skilling = true;
            
            Spawn_SkillRange();
        }
	}
}

void UUltSkillNumber_3::Spawn_SkillRange()
{
    Super::Spawn_SkillRange();
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;

    if(!bActivate)
    {
        OwnerCharacterPC->CurrentMouseCursor = EMouseCursor::Crosshairs;
        USkillDataManager::CursorImmediately();

        // if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick_Ult(true);}
        
        ActivateSkillRange_Round(Ult3_CircleRange);
        if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetSkill(SkillAbilityNestingData, this);	

        if(CheckSmartKey(SkillKey))
        {
            OwnerCharacterPC->SetMouseCursor();
            USkillDataManager::CursorImmediately();
            if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);
        }

    }
    else
    {
        OwnerCharacterPC->bShowMouseCursor = false;
        USkillDataManager::CursorImmediately();

        // if(!CheckSmartKey(SkillKey)) {OwnerCharacterPC->PreventOtherClick_Ult();}
        
        ActivateSkillRange_Target(Ult3_TargetWidth, Ult3_TargetRange, ESkillRangeType::Arrow);
        if(SkillRange_Target.IsValid()) SkillRange_Target->SetMaxDist(Ult3_TargetRange);
        if(SkillRange_Target.IsValid()) SkillRange_Target->SetSkill(SkillAbilityNestingData, this);	

        if(CheckSmartKey(SkillKey))
        {
            OwnerCharacterPC->bShowMouseCursor = true;
            USkillDataManager::CursorImmediately();
            SkillRange_Target->SetActorHiddenInGame(true);
        }

    }

    OwnerCharacter->SetDoing(false);
    SetComponentTickEnabled(true);
}

void UUltSkillNumber_3::OnSkill()
{ 
    if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
    

    if(!bActivate)
    {
        // Check , 커서가 몬스터 위 && 커서가 Circle 안
        FHitResult Hit;
        if(OwnerCharacterPC->GetHitResultUnderCursor(ECC_GameTraceChannel6, false, Hit))
        {
            FVector Check = Hit.Location; Check.Z = OwnerCharacter->GetMesh()->GetComponentLocation().Z;
            if((Check - OwnerCharacter->GetMesh()->GetComponentLocation()).Size() < Ult3_CircleRange)
            {
                OwnerCharacterPC->SetMouseCursor(); USkillDataManager::CursorImmediately();
                if(SkillRange_Circle.IsValid()) SkillRange_Circle->SetActorHiddenInGame(true);
                bActivate = true; SetComponentTickEnabled(false);
                
                OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
                OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, true);
                OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);

                TargetEnemy = Hit.GetActor();
                CharacterRotation_Cursor(Hit);
                Activate_Skill();
                OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);
            }
        }
    }
    else
    {
        OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);
        SetComponentTickEnabled(false);
        if(!OwnerCharacter.IsValid()) return; 

        bActivate = false; 
        if(SkillRange_Target.IsValid()) SkillRange_Target->SetActorHiddenInGame(true);
        
        Activate_Skill();
    }
}

void UUltSkillNumber_3::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return;
    auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;
	// OwnerCharacter->GetAPHUD()->OnUsingSkill.Broadcast(SkillKey, false);
	// OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(SkillKey);
	// OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(SkillKey);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    if(bActivate)
    {
        ArcaneExecution = GetWorld()->SpawnActor<AArcaneExecution>(OwnerCharacter->GetAPSkillHubComponent()->GetArcaneExecutionClass(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());
	    if(!ArcaneExecution.IsValid()) return;
        ArcaneExecution->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform );
        ArcaneExecution->SetOwner(OwnerCharacter.Get());
        ArcaneExecution->SetTargetEnemy(Cast<ACharacter>(TargetEnemy));
        ArcaneExecution->SetExecutionType(1);
        ArcaneExecution->SetSkill(SkillAbilityNestingData, this);	
        OwnerAnim->PlayUltSkill_3_Montage();
    }
    else
    {
        if(!ArcaneExecution.IsValid()) return;
        OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	    OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	    AddSkillCoolTime = 0.0f;
        
        FVector Loc = SkillRange_Target->GetDecalComponent()->GetComponentLocation() - OwnerCharacter->GetMesh()->GetComponentLocation(); Loc = (Loc/Loc.Size()) * Ult3_TargetRange *2.0f;
        ArcaneExecution->SetTargetEnemyLocation(Loc);
        ArcaneExecution->SetExecutionRadius(Ult3_TargetWidth * 0.5f);
        ArcaneExecution->SetExecutionType(2);
        CharacterRotation();
        OwnerAnim->PlayUltSkill_3_Montage();
    }
}

void UUltSkillNumber_3::SkillEnd()
{
    auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;
    if(bActivate)
    {
        OwnerCharacter->GetAPHUD()->GetStatusWidget()->UsingSkillSlot(SkillKey, false);
        OwnerAnim->StopUltSkill_3_Montage();
        Remove_Skill();
    }
    else
    {
        OwnerAnim->StopUltSkill_3_Montage();
        Remove_Skill();
    }
}

void UUltSkillNumber_3::UpdateSkillData()
{
}
