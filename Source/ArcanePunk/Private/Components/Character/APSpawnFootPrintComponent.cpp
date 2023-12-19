#include "Components/Character/APSpawnFootPrintComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"

UAPSpawnFootPrintComponent::UAPSpawnFootPrintComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPSpawnFootPrintComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAPSpawnFootPrintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSpawnFootPrintComponent::SpawnFootPrint(bool LeftFoot)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	FHitResult HitResult;
	if(LeftFoot)
	{
		if(OwnerCharacter->PMCheck(HitResult, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() - OwnerCharacter->GetActorUpVector()*100.0f))
		{
			if(EPhysicalSurface::SurfaceType2 == UGameplayStatics::GetSurfaceType(HitResult))
			{
				auto FootPrint = GetWorld()->SpawnActor<AActor>(OwnerCharacter->GetFootClass(true), OwnerCharacter->GetFootTransform(true));
			}	
		}
	}
	else
	{
		if(OwnerCharacter->PMCheck(HitResult, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() - OwnerCharacter->GetActorUpVector()*100.0f))
		{
			if(EPhysicalSurface::SurfaceType2 == UGameplayStatics::GetSurfaceType(HitResult))
			{
				auto FootPrint = GetWorld()->SpawnActor<AActor>(OwnerCharacter->GetFootClass(false), OwnerCharacter->GetFootTransform(false));
			}
		}
	}
}