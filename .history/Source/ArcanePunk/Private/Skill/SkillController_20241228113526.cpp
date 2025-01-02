// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillController.h"

ASkillController::ASkillController()
{
}

void ASkillController::InitializeSkills(FName skillName, AArcanePunkCharacter* OwnerCharacter)
{
	SkillActor = GetWorld()->SpawnActor<ASkillActor>(ASkillActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	SkillActor->InitSkill(skillName, OwnerCharacter);
}

void ASkillController::UseSkill()
{
	SkillActor->UseSkill();
}

void ASkillController::ShowSkillRange(float Range, FVector Location)
{
	// 스킬 범위를 시각적으로 표시
	DrawDebugSphere(GetWorld(), Location, Range, 32, FColor::Green, false, 1.0f, 0, 1.0f);
}
