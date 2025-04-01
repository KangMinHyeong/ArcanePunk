
#include "Components/Character/APCharacterAuraComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerController/ArcanePunkPlayerController.h"

UAPCharacterAuraComponent::UAPCharacterAuraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAPCharacterAuraComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	OwnerPC = GetWorld()->GetFirstPlayerController(); if(!OwnerPC.IsValid()) return;
	
	// AuraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AuraEffect, OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation());
	AuraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(AuraEffect, OwnerCharacter->GetMesh(), TEXT("AuraEffect"), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation());
}


void UAPCharacterAuraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(OwnerPC.IsValid() && AuraComp)
	{
		FHitResult HitResult;
		OwnerPC->GetHitResultUnderCursor(ECC_GameTraceChannel3, false, HitResult);
		FVector HitPoint;
		if(HitResult.bBlockingHit)
		{
			HitPoint = FVector(HitResult.Location.X, HitResult.Location.Y, AuraComp->GetComponentLocation().Z);
		}
		else {return;}

		HitPoint = HitPoint - AuraComp->GetComponentLocation(); HitPoint = HitPoint/HitPoint.Size();  

		AuraComp->SetRelativeRotation(FRotationMatrix::MakeFromX(HitPoint).Rotator());
	}
}

void UAPCharacterAuraComponent::SetAuraActive(bool NewBool)
{
	if(!AuraComp) return;

	if(NewBool)
	{
		AuraComp->Activate();
	}
	else
	{
		AuraComp->DeactivateImmediate();
	}
}