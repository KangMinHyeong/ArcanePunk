#include "Components/Character/APHitPointComponent.h"

#include "Enemy/Enemy_CharacterBase.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

UAPHitPointComponent::UAPHitPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPHitPointComponent::DistinctHitPoint(const FVector & ImpactPoint, AActor *HitActor)
{
	FVector HitPoint = ImpactPoint - HitActor->GetActorLocation();
	FVector HitActorForwardVec = HitActor->GetActorForwardVector(); 
	FVector HitActorRightVec = HitActor->GetActorRightVector(); 

	float Forward = (HitActorForwardVec.X * HitPoint.X) + (HitActorForwardVec.Y * HitPoint.Y); // 앞 뒤 Hit 판별
	float Right = (HitActorRightVec.X * HitPoint.X) + (HitActorRightVec.Y * HitPoint.Y); // 좌 우 Hit 판별

	auto Enemy = Cast<AEnemy_CharacterBase>(HitActor);
	if(Enemy) {Enemy->SetHitPoint(Forward, Right); Enemy->SetHitEffect(ImpactPoint);}
}

void UAPHitPointComponent::DistinctHitPoint(UNiagaraSystem* HitEffect, const FVector & ImpactPoint, AActor *HitActor)
{
	FVector HitPoint = ImpactPoint - HitActor->GetActorLocation();
	FVector HitActorForwardVec = HitActor->GetActorForwardVector(); 
	FVector HitActorRightVec = HitActor->GetActorRightVector(); 

	float Forward = (HitActorForwardVec.X * HitPoint.X) + (HitActorForwardVec.Y * HitPoint.Y); // 앞 뒤 Hit 판별
	float Right = (HitActorRightVec.X * HitPoint.X) + (HitActorRightVec.Y * HitPoint.Y); // 좌 우 Hit 판별

	auto Enemy = Cast<AEnemy_CharacterBase>(HitActor);
	if(Enemy) {Enemy->SetHitPoint(Forward, Right);}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, ImpactPoint, HitActor->GetActorRotation());
}

void UAPHitPointComponent::SetCrowdControl(AActor *HitActor, ECharacterState UpdateState, float StateTime)
{
	auto Enemy = Cast<AEnemy_CharacterBase>(HitActor);
	if(!Enemy) return;

	switch (UpdateState)
	{
		case ECharacterState::KnockBack:
		Enemy->GetCrowdControlComp()->KnockBackState(KnockBackVec, KnockBackDist, StateTime);
		break;
	
		case ECharacterState::Stun:
		Enemy->GetCrowdControlComp()->StunState(StateTime);
		break;

		case ECharacterState::Sleep:

		break;

		case ECharacterState::Slow:
		Enemy->GetCrowdControlComp()->SlowState(SlowPercent, StateTime);
		break;
	}
}
