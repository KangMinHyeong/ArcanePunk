
#include "GameElements/Trap/APTrapBase_Mine.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

AAPTrapBase_Mine::AAPTrapBase_Mine()
{
    PrimaryActorTick.bCanEverTick = true;

    TriggerDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("TriggerDecal"));

    TriggerDecal->SetupAttachment(TrapMesh);
}

void AAPTrapBase_Mine::BeginPlay()
{
    Super::BeginPlay();

    RangeDecal->DecalSize = FVector(10.0f, TrapTrigger->GetUnscaledSphereRadius(), TrapTrigger->GetUnscaledSphereRadius());
    TriggerDecal->DecalSize = RangeDecal->DecalSize;

    TriggerDecal->SetHiddenInGame(true);

    Player = Cast<AArcanePunkCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
    OperationLocation = RotateMesh->GetComponentLocation() - FVector(0.0f, 0.0f, 40.0f);
}

void AAPTrapBase_Mine::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Player.IsValid() && bRotating) AutoRotating();
    if(Player.IsValid() && bOperation) OperateExplosion(DeltaTime);
    if(Player.IsValid() && bCharging) OperateTrigger(DeltaTime);
}

void AAPTrapBase_Mine::OperateExplosion(float DeltaTime)
{
    FVector Current = RotateMesh->GetComponentLocation();
    Current = FMath::VInterpConstantTo(Current, OperationLocation, DeltaTime, OperateSpeed);
    RotateMesh->SetWorldLocation(Current);

    if(abs(OperationLocation.Z - Current.Z) <= KINDA_SMALL_NUMBER) Explosion_Ready();
}

void AAPTrapBase_Mine::Explosion_Ready()
{
    bOperation = false; bCharging = true;
    RangeDecal->SetHiddenInGame(false);
    TriggerDecal->SetHiddenInGame(false);
}

void AAPTrapBase_Mine::Explosion()
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors, AArcanePunkCharacter::StaticClass());

    for(AActor* Actor : Actors)
    {
        UGameplayStatics::ApplyDamage(Actor, TrapDamage, nullptr, this, UDamageType::StaticClass());
    }

    auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TrapOverlapEffect, GetActorLocation(), GetActorRotation());
    NC->SetVariableFloat(TEXT("Size"), 1.5f);
    Destroy();
}

void AAPTrapBase_Mine::OperateTrigger(float DeltaTime)
{
    float DecalSize = TriggerDecal->GetComponentScale().Y;

    DecalSize = FMath::FInterpConstantTo(DecalSize, 0.0f, DeltaTime, TriggerSpeed);
    TriggerDecal->SetWorldScale3D(FVector(TriggerDecal->GetComponentScale().X, DecalSize, DecalSize));

    if(DecalSize <= KINDA_SMALL_NUMBER)
    {
        bCharging = false; SetActorTickEnabled(false);
        GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPTrapBase_Mine::Explosion, TrapOperationTime, false);
    }
}

void AAPTrapBase_Mine::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

    bRotating = false;
    bOperation = true;
}