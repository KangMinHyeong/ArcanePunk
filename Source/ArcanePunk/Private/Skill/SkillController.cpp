// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillController.h"

#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_TwoCircle.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Character/SkillRange/APSkillRange_Circle.h"
#include "Character/SkillRange/APSkillRange_Arrow.h"
#include "GameInstance/APGameInstance.h"
#include "Components/Character/APMovementComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "UserInterface/HUD/APHUD.h"

ASkillController::ASkillController()
{
}

void ASkillController::InitializeSkills(ESkillKey SkillKey, FName SkillId, TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacterPtr)
{
	if(!SkillActor)
	{
		SkillActor = TSharedPtr<ASkillActor>(GetWorld()->SpawnActor<ASkillActor>(ASkillActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator));
	}

	// TODO: ScillControllerData에 대한 매니저 클래스 구현 후 받아오는 코드 수정
	// CurSkillData = USkillActorDataManager::GetInstance()->GetSkillData(SkillId);
	
	SkillActor->InitSkill(SkillId, this->OwnerCharacter, this);

	this->OwnerCharacter = OwnerCharacterPtr;

	// // 스킬 타겟 범위 액터 스폰
	// if(!SkillRange_Target.IsValid() && SkillActor->GetSkillTargetRangeClass())
	// {
	// 	SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(SkillActor->GetSkillTargetRangeClass(), OwnerPlayer->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	//
	// 	// 스킬 타겟 범위 액터 초기화 및 비활성화
	// 	SkillRange_Target->AttachToComponent(OwnerPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);	
	// 	SkillRange_Target->SetActorHiddenInGame(true);
	// 	SkillRange_Target->SetActive(false);
	// 	SkillRange_Target->SetOwner(OwnerPlayer.Get());
	// }
	//
	// if(!SkillRange_Ground.IsValid() && SkillActor->GetSkillGroundRangeClass())
	// {
	// 	// 스킬 사거리 범위 액터 초기화 및 비활성화
	// 	SkillRange_Ground = GetWorld()->SpawnActor<AAPSkillRange>(SkillActor->GetSkillGroundRangeClass(), OwnerPlayer->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	// 	
	// 	// 스킬 사거리 범위 액터 초기화 및 비활성화
	// 	SkillRange_Ground->AttachToComponent(OwnerPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	// 	SkillRange_Ground->SetActorHiddenInGame(true);
	// 	SkillRange_Ground->SetActive(false);
	// 	SkillRange_Ground->SetOwner(OwnerPlayer.Get());
	// }
}

void ASkillController::UseSkill(ESkillKey SkillKey)
{
	// 이미 스킬 범위를 활성화 했다면 원상태로
	if(bReady) {Restore(); return;}
	if(!CheckSkillCondition(SkillKey)) return;

	CurrentSkillKey = SkillKey;
	auto SettingGI = Cast<UAPSettingSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSettingSubsystem::StaticClass()));

	// 버프형, 즉발형 판별 (현재는 Projectile형만 있어서 체크 X)
	if(CurSkillData.SkillType == ESkillType::Immediate) // 즉발형이면 바로 스킬 재생
	{
		SkillActor->PlaySkillAction();
		return;
	}

	// 스마트키 체크
	if(SettingGI->GetSmartKey()[(uint8)SkillKey] && CurSkillData.SkillType != ESkillType::Charging) 
	{
		ActivateSkill();
		return;
	}

	// 스킬 범위 액터 활성화 및 크기 조절
	// auto X = SkillDataRow.CollisionSize.X;
	// auto Y = SkillDataRow.CollisionSize.Y;
	// auto Range = SkillDataRow.Range * SkillActor->RangeCoefficient * 0.5f;
	//
	// if(SkillRange_Target.IsValid())
	// {
	// 	SkillRange_Target->SetScale_Target(SkillDataRow.SkillType, X, Y, Range);
	// 	SkillRange_Target->SetMaxDist(Range);
	// 	SkillRange_Target->SetActive(true);
	//
	// }
	// if(SkillRange_Ground.IsValid())
	// {
	// 	SkillRange_Ground->SetScale(Range);
	// 	SkillRange_Ground->SetActive(true);
	// }

	// 커서 비활성화
	auto PC = Cast<APlayerController>(OwnerCharacter->GetController()); if(!PC) return;
	PC->bShowMouseCursor = false;
	// USkillActorDataManager::CursorImmediately();

	if(CurSkillData.SkillType == ESkillType::Charging)
	{
		OwnerCharacter->PlaySkillAction(SkillAction.Get());
		SkillActor->PlaySkillAction();
		return;
	}
	// 좌클릭으로 캐스팅할 수 있도록 델리게이트에 연결
	OwnerCharacter->OnLeftMouseClick.AddDynamic(this, &ASkillController::ActivateSkill);

	bReady = true;
}

void ASkillController::ActivateSkill()
{
	RotatePlayer();

	// 스킬 몽타주 재생 및 노티파이 델리게이트 바인딩
	SkillActor->PlaySkillAction();
}

void ASkillController::Restore()
{
	// 델리게이트 연결 삭제
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &ASkillController::ActivateSkill);

	// 커서 활성화
	auto PC = Cast<APlayerController>(OwnerCharacter->GetController()); if(!PC) return;
	PC->bShowMouseCursor = true;
	// USkillActorDataManager::CursorImmediately();

	FHitResult HitResult; PC->GetHitResultUnderCursor(ECC_GameTraceChannel3, false, HitResult);
	TargetLocation = HitResult.ImpactPoint;

	// 스킬 범위 액터 비활성화
	if(SkillRange_Target.IsValid()) SkillRange_Target->SetActive(false);
	if(SkillRange_Ground.IsValid()) SkillRange_Ground->SetActive(false);
	
	bReady = false;
}

void ASkillController::ShowSkillRange(float Range, FVector Location)
{
	// 스킬 범위를 시각적으로 표시
	DrawDebugSphere(GetWorld(), Location, Range, 32, FColor::Green, false, 1.0f, 0, 1.0f);
}

bool ASkillController::CheckSkillCondition(ESkillKey SkillKey)
{
	bool Check = true;
	if(!OwnerCharacter.IsValid()) Check = false;
	if(OwnerCharacter->GetDoing()) Check = false;
	// TODO: MP 체크는 SkillController쪽에서 해야할듯?
	// if(OwnerPlayer->GetPlayerStatus().StatusData.MP < SkillActor->GetMPConsumption()) Check = false;
	// if(SkillActor->IsSkilling()) Check = false;
	if(!CheckSkillCool(SkillKey)) Check = false;

	if(!Check)
	{
		UAPDataTableSubsystem::DisplaySystemMesseage(UAPGameInstance::GetDataTableGI(GetWorld()), EStringRowName::CannotSkill, true, true); 
		UAPSoundSubsystem::PlayRejectSound(UAPGameInstance::GetSoundGI(GetWorld()));
	}
	
    return Check;
}

bool ASkillController::CheckSkillCool(ESkillKey SkillKey)
{
	switch (SkillKey)
	{
		case ESkillKey::Q:
        return OwnerCharacter->GetbCanSkill_Q();
        break;
    
        case ESkillKey::E:
        return OwnerCharacter->GetbCanSkill_E();
        break;

        case ESkillKey::R:
        return OwnerCharacter->GetbCanSkill_R();
        break;
	}

    return true;
}

void ASkillController::RotatePlayer()
{
	Restore();

	// 스킬 범위 회전값으로 캐릭터 회전
	FVector Loc = TargetLocation - OwnerCharacter->GetMesh()->GetComponentLocation(); Loc.Z = 0.0f; 
	FRotator Rotation = FRotationMatrix::MakeFromX(Loc).Rotator();
	OwnerCharacter->GetAPMoveComponent()->SetAttackRotation(Rotation, 500.0f);
}

void ASkillController::UpdateSkillSlotImage()
{
	if(SkillActor)
	SkillActor->UpdateSkillSlotImage();
}

void ASkillController::StartCoolDown()
{
	// OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(CurrentSkillKey, CurSkillData.CoolDownTime_msec);
}
