
#include "GameElements/Trap/APTrapBase.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"

AAPTrapBase::AAPTrapBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
	TrapTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("TrapTrigger"));

	SetRootComponent(TrapMesh);
	TrapTrigger->SetupAttachment(TrapMesh);
}

void AAPTrapBase::BeginPlay()
{
	Super::BeginPlay();
	
	TrapMesh->OnComponentBeginOverlap.AddDynamic(this, &AAPTrapBase::OnOverlap);
}

void AAPTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPTrapBase::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TrapOverlapEffect, GetActorLocation(), GetActorRotation());
	Destroy();
}
