
#include "Components/Character/APAuraComponent.h"

#include "PlayerController/ArcanePunkPlayerController.h"

UAPAuraComponent::UAPAuraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAPAuraComponent::BeginPlay()
{
	Super::BeginPlay();

	auto Owner = Cast<APawn>(GetOwner()); if(!Owner) return;
	OwnerPC = Cast<AArcanePunkPlayerController>(Owner->GetController()); if(!OwnerPC.IsValid()) return;
}

void UAPAuraComponent::TickComponent(float DeltaSeconds, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaSeconds, TickType, ThisTickFunction);

	if(OwnerPC.IsValid())
	{
		FHitResult HitResult;
		OwnerPC->GetHitResultUnderCursor(ECC_GameTraceChannel3, false, HitResult);
		FVector HitPoint;
		if(HitResult.bBlockingHit)
		{
			HitPoint = HitResult.Location; //FVector(HitResult.Location.X, HitResult.Location.Y, OwnerCharacter->GetActorLocation().Z);
		}
		else {return;}

		HitPoint = HitPoint - GetOwner()->GetActorLocation(); HitPoint = HitPoint/HitPoint.Size();  

		SetRelativeRotation(FRotationMatrix::MakeFromX(HitPoint).Rotator());
	}
}

