
#include "Components/SkillActor/APSkillType.h"

#include "NiagaraComponent.h"

UAPSkillType::UAPSkillType()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPSkillType::BeginPlay()
{
	Super::BeginPlay();
}

void UAPSkillType::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAPSkillType::SetSkillType(ESkillTypeState UpdateSkillType, ESkillCategory UpdateSkillCategory)
{
	SkillType = UpdateSkillType;
	SkillCategory = UpdateSkillCategory;

	switch (SkillCategory)
	{
		case ESkillCategory::Projecitle:
		Type_Projecitle();
		break;

		case ESkillCategory::Throw:
		Type_Throw();
		break;

		case ESkillCategory::Laser:
		Type_Laser();
		break;

		case ESkillCategory::InvisibleCollider:
		Type_InvisibleCollider();
		break;

		case ESkillCategory::Buff:
		Type_Buff();
		break;
	
	}


}

// void UAPSkillType::Type_Projecitle()
// {
// 	auto Skill = Cast<AAPSkillActorBase>(GetOwner()); if(!Skill) return;

// 	auto Mesh = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
// 	if(Mesh && Skill->IsSkillTypeMaterial(SkillType))Mesh->SetMaterial(0,Skill->GetSkillTypeMaterial(SkillType));
// 	if(Skill->IsEffectColor(SkillType)) Skill->SetEffectColor(Skill->GetEffectColor(SkillType));

// 	switch (SkillType)
// 	{
// 		case ESkillTypeState::Type_Q:
		
// 		break;

// 		case ESkillTypeState::Type_E:
		
// 		break;

// 		case ESkillTypeState::Type_R:
		
// 		break;
	
// 	}

// }

// void UAPSkillType::Type_Throw()
// {
// 	auto Skill = Cast<AAPSkillActorBase>(GetOwner()); if(!Skill) return;

// 	auto Mesh = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
// 	if(Mesh && Skill->IsSkillTypeMaterial(SkillType))Mesh->SetMaterial(0,Skill->GetSkillTypeMaterial(SkillType));

// 	switch (SkillType)
// 	{
// 		case ESkillTypeState::Type_Q:

// 		break;

// 		case ESkillTypeState::Type_E:

// 		break;

// 		case ESkillTypeState::Type_R:

// 		break;
	
// 	}
// }

// void UAPSkillType::Type_Laser()
// {
// 	auto Skill = Cast<AAPSkillActorBase>(GetOwner()); if(!Skill) return;
// 	if(Skill->IsEffectColor(SkillType)) Skill->SetEffectColor(Skill->GetEffectColor(SkillType));

// 	switch (SkillType)
// 	{
// 		case ESkillTypeState::Type_Q:

// 		break;

// 		case ESkillTypeState::Type_E:

// 		break;

// 		case ESkillTypeState::Type_R:

// 		break;
	
// 	}
// }

// void UAPSkillType::Type_InvisibleCollider()
// {
// 	auto Skill = Cast<AAPSkillActorBase>(GetOwner()); if(!Skill) return;
// 	if(Skill->IsEffectColor(SkillType)) Skill->SetEffectColor(Skill->GetEffectColor(SkillType));

// 	switch (SkillType)
// 	{
// 		case ESkillTypeState::Type_Q:

// 		break;

// 		case ESkillTypeState::Type_E:

// 		break;

// 		case ESkillTypeState::Type_R:

// 		break;
	
// 	}
// }

// void UAPSkillType::Type_Buff()
// {
// 	switch (SkillType)
// 	{
// 		case ESkillTypeState::Type_Q:

// 		break;

// 		case ESkillTypeState::Type_E:

// 		break;

// 		case ESkillTypeState::Type_R:

// 		break;
	
// 	}
// }