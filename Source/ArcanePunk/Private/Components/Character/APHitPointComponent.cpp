#include "Components/Character/APHitPointComponent.h"

#include "Enemy/Enemy_CharacterBase.h"

UAPHitPointComponent::UAPHitPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPHitPointComponent::DistinctHitPoint(FVector ImpactPoint, AActor *HitActor)
{
	FVector HitPoint = ImpactPoint - HitActor->GetActorLocation();
	FVector HitActorForwardVec = HitActor->GetActorForwardVector(); 
	FVector HitActorRightVec = HitActor->GetActorRightVector(); 

	float Forward = (HitActorForwardVec.X * HitPoint.X) + (HitActorForwardVec.Y * HitPoint.Y); // 앞 뒤 Hit 판별
	float Right = (HitActorRightVec.X * HitPoint.X) + (HitActorRightVec.Y * HitPoint.Y); // 좌 우 Hit 판별

	auto Enemy = Cast<AEnemy_CharacterBase>(HitActor);
	if(Enemy) Enemy->SetHitPoint(Forward, Right);	
}


