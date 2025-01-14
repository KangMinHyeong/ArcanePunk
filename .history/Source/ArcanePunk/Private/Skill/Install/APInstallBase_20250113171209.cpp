
#include "Skill/Install/APInstallBase.h"

AAPInstallBase::AAPInstallBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TurretCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TurretCapsule"));
    TurretRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretRoot"));
    RotateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateMesh"));
    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
     
    SetRootComponent(TurretCapsule);
    TurretRoot->SetupAttachment(TurretCapsule);
    RotateMesh->SetupAttachment(TurretRoot);
    ProjectileSpawnPoint->SetupAttachment(RotateMesh);
}

// Called when the game starts or when spawned
void AAPInstallBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
}

// Called every frame
void AAPInstallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

