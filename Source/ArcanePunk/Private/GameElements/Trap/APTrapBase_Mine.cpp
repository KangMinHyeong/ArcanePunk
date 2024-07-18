
#include "GameElements/Trap/APTrapBase_Mine.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

AAPTrapBase_Mine::AAPTrapBase_Mine()
{
    PrimaryActorTick.bCanEverTick = true;

	MineTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineTopMesh"));
    MineRangeDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("MineRangeDecal"));

	MineTopMesh->SetupAttachment(TrapMesh);
    MineRangeDecal->SetupAttachment(TrapMesh);
}

void AAPTrapBase_Mine::BeginPlay()
{
    Super::BeginPlay();

    MineRangeDecal->SetHiddenInGame(true);
    MineRangeDecal->DecalSize = FVector(10.0f, TrapTrigger->GetUnscaledSphereRadius(), TrapTrigger->GetUnscaledSphereRadius());

    Player = Cast<AArcanePunkCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
    OperationLocation = MineTopMesh->GetComponentLocation() - FVector(0.0f, 0.0f, 40.0f);
}

void AAPTrapBase_Mine::AutoRotating()
{
    FVector ToTarget = Player->GetActorLocation() - MineTopMesh->GetComponentLocation();
	FRotator TargetRotation = FRotator(0, ToTarget.Rotation().Yaw - 90.0f, 0);

	MineTopMesh->SetWorldRotation(FMath::RInterpTo(MineTopMesh->GetComponentRotation(), TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), RotateSpeed));
}

void AAPTrapBase_Mine::OperateExplosion(float DeltaTime)
{
    FVector Current = MineTopMesh->GetComponentLocation();
    Current = FMath::VInterpConstantTo(Current, OperationLocation, DeltaTime, OperateSpeed);
    MineTopMesh->SetWorldLocation(Current);

    if(abs(OperationLocation.Z - Current.Z) <= KINDA_SMALL_NUMBER)
    {
        SetActorTickEnabled(false);
        bOperation = false;
        Explosion();
    }
}

void AAPTrapBase_Mine::Explosion()
{
    UE_LOG(LogTemp, Display, TEXT("Your message"));
}

void AAPTrapBase_Mine::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Player.IsValid() && bRotating) AutoRotating();
    if(Player.IsValid() && bOperation) OperateExplosion(DeltaTime);
}

void AAPTrapBase_Mine::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

    bRotating = false;
    bOperation = true;
    MineRangeDecal->SetHiddenInGame(false);
}