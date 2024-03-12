
#include "Enemy/Drop/Enemy_DropUnlock.h"

AEnemy_DropUnlock::AEnemy_DropUnlock()
{
    EnHanceTypeMaterial.SetNum(3);
    EnHanceTypePercent.SetNum(3);
}

void AEnemy_DropUnlock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Destroy();
}

void AEnemy_DropUnlock::BeginPlay()
{
    Super::BeginPlay();

    InitEnHanceType();
}

void AEnemy_DropUnlock::InitEnHanceType()
{
    float SilverPercent = EnHanceTypePercent[0];
    float GoldPercent = EnHanceTypePercent[0] + EnHanceTypePercent[1];
    float PlatinumPercent = EnHanceTypePercent[0] + EnHanceTypePercent[1] + EnHanceTypePercent[2];

    float CurrentEnHanceType = FMath::RandRange(0.0f, PlatinumPercent);

    if(CurrentEnHanceType >= 0.0f && CurrentEnHanceType <= SilverPercent)
    {
        DropMesh->SetMaterial(0,EnHanceTypeMaterial[0]);
        EnHanceType = EEnHanceType::Silver;
    }
    else if(CurrentEnHanceType > SilverPercent && CurrentEnHanceType <= GoldPercent)
    {
        DropMesh->SetMaterial(0,EnHanceTypeMaterial[1]);
        EnHanceType = EEnHanceType::Gold;
    }
    else if (CurrentEnHanceType > GoldPercent && CurrentEnHanceType <= PlatinumPercent)
    {
        DropMesh->SetMaterial(0,EnHanceTypeMaterial[2]);
        EnHanceType = EEnHanceType::Platinum;
    }
}
