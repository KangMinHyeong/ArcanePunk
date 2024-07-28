
#include "Enemy/Enemy_GunSoldier.h"

#include "Enemy/SkillActor/APEnemyAmmo.h"

AEnemy_GunSoldier:: AEnemy_GunSoldier()
{
    AmmoSpawnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("AmmoSpawnComponent"));

    AmmoSpawnComponent->SetupAttachment(GetMesh(), TEXT("AmmoSpawnLocation"));
}

void AEnemy_GunSoldier::NormalAttack()
{
    auto Ammo = GetWorld()->SpawnActor<AAPEnemyAmmo>(AmmoClass, AmmoSpawnComponent->GetComponentLocation(), GetActorRotation());
    Ammo->SetOwner(this);
    Ammo->SetOwnerEnemy(Monster_AttackRange_Plus, Monster_AttackRadius);
}