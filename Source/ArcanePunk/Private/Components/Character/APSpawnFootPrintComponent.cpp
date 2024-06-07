#include "Components/Character/APSpawnFootPrintComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameInstance/APGameInstance.h"

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

void UAPSpawnFootPrintComponent::SpawnFootPrint(bool LeftFoot, bool Step)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	FHitResult HitResult;
	if(LeftFoot)
	{
		if(OwnerCharacter->PMCheck(HitResult, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() - OwnerCharacter->GetActorUpVector()*100.0f))
		{
			if(Step && EPhysicalSurface::SurfaceType2 == UGameplayStatics::GetSurfaceType(HitResult))
			{
				auto FootPrint = GetWorld()->SpawnActor<AActor>(GetFootClass(true), OwnerCharacter->GetFootTransform(true));
			} 
			else if(!Step)
			{
				if(OwnerCharacter->GetVelocity().Size() >= 0.1f)
				{
					if(Num != (uint8)UGameplayStatics::GetSurfaceType(HitResult))
					{
						Num = (uint8)UGameplayStatics::GetSurfaceType(HitResult);
						if(StepEffect.Num() <= Num) return; if(StepSound.Num() <= Num) return;

						if(RunEffect.IsValid()) RunEffect->DeactivateImmediate();
						if(StepSound[Num]) RunEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(StepEffect[Num], OwnerCharacter->GetMesh(), TEXT("StepEffect"), OwnerCharacter->GetMesh()->GetComponentLocation() + OwnerCharacter->GetActorUpVector()*UpCoeff - OwnerCharacter->GetActorForwardVector()*BackCoeff, OwnerCharacter->GetMesh()->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
					}
					if(StepSound[Num]) SpawnSound(StepSound[Num], OwnerCharacter->GetMesh()->GetComponentLocation());
				}
				else
				{
					if(RunEffect.IsValid()) RunEffect->DeactivateImmediate();
					Num = -1;
				}
			}
		}
	}
	else
	{
		if(OwnerCharacter->PMCheck(HitResult, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() - OwnerCharacter->GetActorUpVector()*100.0f))
		{
			if(Step && EPhysicalSurface::SurfaceType2 == UGameplayStatics::GetSurfaceType(HitResult))
			{
				auto FootPrint = GetWorld()->SpawnActor<AActor>(GetFootClass(false), OwnerCharacter->GetFootTransform(false));
			}
			else if(!Step)
			{
				if(OwnerCharacter->GetVelocity().Size() >= 0.1f)
				{
					if(Num != (uint8)UGameplayStatics::GetSurfaceType(HitResult))
					{
						Num = (uint8)UGameplayStatics::GetSurfaceType(HitResult);
						if(StepEffect.Num() <= Num) return; if(StepSound.Num() <= Num) return;

						if(RunEffect.IsValid()) RunEffect->DeactivateImmediate();
						if(StepSound[Num]) RunEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(StepEffect[Num], OwnerCharacter->GetMesh(), TEXT("StepEffect"), OwnerCharacter->GetMesh()->GetComponentLocation() + OwnerCharacter->GetActorUpVector()*UpCoeff - OwnerCharacter->GetActorForwardVector()*BackCoeff, OwnerCharacter->GetMesh()->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
					}
					if(StepSound[Num]) SpawnSound(StepSound[Num], OwnerCharacter->GetMesh()->GetComponentLocation());
				}
				else
				{
					if(RunEffect.IsValid()) RunEffect->DeactivateImmediate();
					Num = -1;
				}
			}
		}
	}
}

void UAPSpawnFootPrintComponent::SpawnSound(USoundBase* Sound, FVector Location)
{
	auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

	float SoundVolume = GI->GetGameSoundVolume().MasterVolume * GI->GetGameSoundVolume().EffectVolume;
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, Location, FRotator::ZeroRotator, SoundVolume * SoundCoefficient);
}

TSubclassOf<AActor> UAPSpawnFootPrintComponent::GetFootClass(bool Left) 
{	
	if(Left) return LeftFootClass;
    return RightFootClass;
}
