// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/USkillActorFactorySubsystem.h"
#include "Skill/SkillActors/ProjectileSkillActor.h"

void USkillActorFactorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RegisterAll(); // 자동으로 한 번만 호출됨
}

TSharedPtr<ASkillActor> USkillActorFactorySubsystem::GetSkillActorClass(FName SkillId) const
{
	// TODO: SkillControllerData에서 가져오는 형태로 변경
	FSkillControllerData SkillControllerData = GetGameInstance()->GetSubsystem<USkillControllerDataManager>()->GetSkillData(SkillId);

	if (SkillClassMap.Contains(SkillControllerData.SkillType))
	{
		return TSharedPtr<ASkillActor>(GetWorld()->SpawnActor<ASkillActor>(SkillClassMap[SkillControllerData.SkillType], FTransform::Identity));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SkillClassMap에 스킬 타입 %d이(가) 없습니다."), SkillControllerData.SkillType);
		return nullptr;
	}
}

// 신규 스킬 타입 추가 시 여기에 스킬 추가
void USkillActorFactorySubsystem::RegisterAll()
{
	SkillClassMap.Add(ESkillType::Projectile, AProjectileSkillActor::StaticClass());
}