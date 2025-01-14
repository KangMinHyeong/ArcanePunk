// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillController.h"

#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_TwoCircle.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Character/SkillRange/APSkillRange_Circle.h"
#include "Character/SkillRange/APSkillRange_Arrow.h"
#include "Skill/SkillDataManager.h"
#include "GameInstance/APGameInstance.h"
#include "Components/Character/APMovementComponent.h"

ASkillController::ASkillController()
{
}

void ASkillController::InitializeSkills(FName skillName, AArcanePunkCharacter* OwnerCharacter)
{
	SkillActor = GetWorld()->SpawnActor<ASkillActor>(ASkillActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	SkillActor->InitSkill(skillName, OwnerCharacter, this);

	OwnerPlayer = OwnerCharacter;

	// 스킬 타겟 범위 액터 스폰
	if(SkillActor->GetSkillTargetRangeClass())
	{
		SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(SkillActor->GetSkillTargetRangeClass(), OwnerPlayer->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	
		// 스킬 타겟 범위 액터 초기화 및 비활성화
		SkillRange_Target->AttachToComponent(OwnerPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);	
		SkillRange_Target->SetActorHiddenInGame(true);
		SkillRange_Target->SetActive(false);
		SkillRange_Target->SetOwner(OwnerPlayer.Get());
	}
	
	if(SkillActor->GetSkillGroundRangeClass())
	{
		// 스킬 사거리 범위 액터 초기화 및 비활성화
		SkillRange_Ground = GetWorld()->SpawnActor<AAPSkillRange>(SkillActor->GetSkillGroundRangeClass(), OwnerPlayer->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		
		// 스킬 사거리 범위 액터 초기화 및 비활성화
		SkillRange_Ground->AttachToComponent(OwnerPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		SkillRange_Ground->SetActorHiddenInGame(true);
		SkillRange_Ground->SetActive(false);
		SkillRange_Ground->SetOwner(OwnerPlayer.Get());
	}
}

void ASkillController::UseSkill(ESkillKey SkillKey)
{
	// 이미 스킬 범위를 활성화 했다면 원상태로
	if(bReady) {Restore(); return;}
	if(!CheckSkillCondition(SkillKey)) return;

	CurrentSkillKey = SkillKey;
	auto SkillDataRow = USkillDataManager::GetInstance()->GetSkillData(SkillActor->GetSkillName());
	auto SettingGI = Cast<UAPSettingSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSettingSubsystem::StaticClass()));

	// 버프형, 즉발형 판별 (현재는 Projectile형만 있어서 체크 X)
	if(SkillDataRow.SkillType == ESkillType::Immediate) // 즉발형이면 바로 스킬 재생
	{
		SkillActor->PlaySkillAction(CurrentSkillKey);
		return;
	}

	// 스마트키 체크
	if(SettingGI->GetSmartKey()[(uint8)SkillKey]) 
	{
		ActivateSkill();
		return;
	}

	// 스킬 범위 액터 활성화 및 크기 조절
	auto X = SkillDataRow.CollisionSize.X;
	auto Y = SkillDataRow.CollisionSize.Y;
	auto Range = SkillDataRow.Range * SkillActor->RangeCoefficient * 0.5f;

	if(SkillRange_Target.IsValid())
	{
		SkillRange_Target->SetScale_Target(SkillDataRow.SkillType, X, Y, Range);
		SkillRange_Target->SetMaxDist(Range);
		SkillRange_Target->SetActive(true);
	}
	if(SkillRange_Ground.IsValid())
	{
		SkillRange_Ground->SetScale(Range);
		SkillRange_Ground->SetActive(true);
	}

	// 커서 비활성화
	auto PC = Cast<APlayerController>(OwnerPlayer->GetController()); if(!PC) return;
	PC->bShowMouseCursor = false;
	USkillDataManager::CursorImmediately();

	if(SkillDataRow.SkillType == ESkillType::Charging)
	{
		SkillActor->PlaySkillAction(CurrentSkillKey);
		return;
	}
	// 좌클릭으로 캐스팅할 수 있도록 델리게이트에 연결
	OwnerPlayer->OnLeftMouseClick.AddDynamic(this, &ASkillController::ActivateSkill);

	bReady = true;
}

void ASkillController::ActivateSkill()
{
	RotatePlayer();

	// 스킬 몽타주 재생 및 노티파이 델리게이트 바인딩
	SkillActor->PlaySkillAction(CurrentSkillKey);
}

void ASkillController::Restore()
{
	// 델리게이트 연결 삭제
	OwnerPlayer->OnLeftMouseClick.RemoveDynamic(this, &ASkillController::ActivateSkill);

	// 커서 활성화
	auto PC = Cast<APlayerController>(OwnerPlayer->GetController()); if(!PC) return;
	PC->bShowMouseCursor = true;
	USkillDataManager::CursorImmediately();

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
	if(!OwnerPlayer.IsValid()) Check = false;
	if(OwnerPlayer->GetDoing()) Check = false;
	if(OwnerPlayer->GetPlayerStatus().StatusData.MP < SkillActor->GetMPConsumption()) Check = false;
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
        return OwnerPlayer->GetbCanSkill_Q();
        break;
    
        case ESkillKey::E:
        return OwnerPlayer->GetbCanSkill_E();
        break;

        case ESkillKey::R:
        return OwnerPlayer->GetbCanSkill_R();
        break;
	}

    return true;
}

void ASkillController::RotatePlayer()
{
	Restore();

	// 스킬 범위 회전값으로 캐릭터 회전
	FVector Loc = TargetLocation - OwnerPlayer->GetMesh()->GetComponentLocation(); Loc.Z = 0.0f; 
	FRotator Rotation = FRotationMatrix::MakeFromX(Loc).Rotator();
	OwnerPlayer->GetAPMoveComponent()->SetAttackRotation(Rotation, 500.0f);
}