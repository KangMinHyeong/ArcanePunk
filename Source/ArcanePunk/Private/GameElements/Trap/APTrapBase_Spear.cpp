
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
	
    TrapTrigger->OnComponentBeginOverlap.RemoveDynamic(this, &AAPTrapBase::OnOverlap);
	RotateMesh->OnComponentBeginOverlap.AddDynamic(this, &AAPTrapBase_Spear::OnOverlap);

    InitLocation = RotateMesh->GetComponentLocation();
    Destination = InitLocation; Destination.Z += SpearHeight;
    GetWorldTimerManager().SetTimer(TriggerTimerHandle, this, &AAPTrapBase_Spear::OnOperating, TriggerTime, false);
}

void AAPTrapBase_Spear::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if(bOperation)
    {
        auto Current = RotateMesh->GetComponentLocation();
        Current = FMath::VInterpConstantTo(Current, Destination, DeltaTime, RotateSpeed);
        RotateMesh->SetWorldLocation(Current);

        if((Current - Destination).Size() <= Margin)
        {
            bOperation = false;
            RotateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetWorldTimerManager().SetTimer(TriggerTimerHandle, this, &AAPTrapBase_Spear::OnOperatingEnd, HoldTime, false);
        }
    }
    if(bEnd)
    {
        auto Current = RotateMesh->GetComponentLocation();
        Current = FMath::VInterpConstantTo(Current, InitLocation, DeltaTime, RotateSpeed);
        RotateMesh->SetWorldLocation(Current);

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
    RotateMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAPTrapBase_Spear::OnOperatingEnd()
{
    GetWorldTimerManager().ClearTimer(TriggerTimerHandle);
    bEnd = true;
}

void AAPTrapBase_Spear::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	UGameplayStatics::ApplyDamage(Character, TrapDamage, nullptr, this, UDamageType::StaticClass());

	// auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TrapOverlapEffect, GetActorLocation(), GetActorRotation());
}

