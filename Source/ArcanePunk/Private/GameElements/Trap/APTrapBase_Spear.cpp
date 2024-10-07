
#include "GameElements/Trap/APTrapBase_Spear.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"

AAPTrapBase_Spear::AAPTrapBase_Spear()
{

}

void AAPTrapBase_Spear::BeginPlay()
{
    Super::BeginPlay();
	
    TrapCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AAPTrapBase::OnOverlap);
	TopMesh->OnComponentBeginOverlap.AddDynamic(this, &AAPTrapBase_Spear::OnOverlap);

    InitLocation = TopMesh->GetComponentLocation();
    Destination = InitLocation; Destination.Z += SpearHeight;
    GetWorldTimerManager().SetTimer(TriggerTimerHandle, this, &AAPTrapBase_Spear::OnOperating, TriggerTime, false);
}

void AAPTrapBase_Spear::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if(bOperation)
    {
        auto Current = TopMesh->GetComponentLocation();
        Current = FMath::VInterpConstantTo(Current, Destination, DeltaTime, RotateSpeed);
        TopMesh->SetWorldLocation(Current);

        if((Current - Destination).Size() <= Margin)
        {
            bOperation = false;
            TopMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetWorldTimerManager().SetTimer(TriggerTimerHandle, this, &AAPTrapBase_Spear::OnOperatingEnd, HoldTime, false);
        }
    }
    if(bEnd)
    {
        auto Current = TopMesh->GetComponentLocation();
        Current = FMath::VInterpConstantTo(Current, InitLocation, DeltaTime, RotateSpeed);
        TopMesh->SetWorldLocation(Current);

        if((Current - InitLocation).Size() <= Margin)
        {
            bEnd = false;
            GetWorldTimerManager().SetTimer(TriggerTimerHandle, this, &AAPTrapBase_Spear::OnOperating, TriggerTime, false);
        }
    }
}

void AAPTrapBase_Spear::OnOperating()
{
    GetWorldTimerManager().ClearTimer(TriggerTimerHandle);
    bOperation = true;
    TopMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    SpawnTrapOperationSound();
}

void AAPTrapBase_Spear::OnOperatingEnd()
{
    GetWorldTimerManager().ClearTimer(TriggerTimerHandle);
    bEnd = true;

    SpawnTrapEndSound();
}

void AAPTrapBase_Spear::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	TopMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
	FHitResult Hit;
	Hit.Location = GetActorLocation(); Hit.ImpactPoint = GetActorLocation();
	UGameplayStatics::ApplyPointDamage(Character, TrapDamage, Hit.ImpactPoint, Hit, GetInstigatorController(), this, UDamageType::StaticClass());
}

void AAPTrapBase_Spear::Deactivate()
{
    Super::Deactivate();
    TopMesh->OnComponentBeginOverlap.RemoveDynamic(this, &AAPTrapBase_Spear::OnOverlap);
}
