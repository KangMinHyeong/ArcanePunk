
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
	RotateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateMesh"));

	SetRootComponent(TrapMesh);
	TrapTrigger->SetupAttachment(TrapMesh);
	RotateMesh->SetupAttachment(TrapMesh);
}

void AAPTrapBase::BeginPlay()
{
	Super::BeginPlay();
	
	TrapTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPTrapBase::OnOverlap);
}

void AAPTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPTrapBase::AutoRotating()
{
    FVector ToTarget = Player->GetActorLocation() - RotateMesh->GetComponentLocation();
	FRotator TargetRotation = FRotator(0, ToTarget.Rotation().Yaw + RotatePlus, 0);

	RotateMesh->SetWorldRotation(FMath::RInterpTo(RotateMesh->GetComponentRotation(), TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), RotateSpeed));
}

void AAPTrapBase::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	// UGameplayStatics::ApplyDamage(Character, TrapDamage, nullptr, this, UDamageType::StaticClass());

	// auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TrapOverlapEffect, GetActorLocation(), GetActorRotation());
	// Destroy();
}
