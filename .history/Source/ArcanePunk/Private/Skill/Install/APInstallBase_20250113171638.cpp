
#include "Skill/Install/APInstallBase.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Enemy/Enemy_CharacterBase.h"

AAPInstallBase::AAPInstallBase()
{
	PrimaryActorTick.bCanEverTick = true;

	InstallCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InstallCapsule"));
    InstallRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InstallRoot"));
     
    SetRootComponent(InstallCapsule);
    InstallRoot->SetupAttachment(InstallCapsule);
}

// Called when the game starts or when spawned
void AAPInstallBase::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    CurrentDamage = OwnerCharacter->GetCurrentATK() * DamageCoefficient; 

    OriginAmmoNum = SpawnAmmoNum;
    Enemy = FindEnemy();

	SetActorTickEnabled(false);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAPInstallBase::DestroySKill, DestroyTime, false);
}

// Called every frame
void AAPInstallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPInstallBase::DestroySKill()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AAPInstallBase::InitInstall(AActor *SkillActor)
{
}