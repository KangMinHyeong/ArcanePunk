
#include "Enemy/Enemy_DroneSoldier.h"

#include "Enemy/Enemy_Drone.h"

void AEnemy_DroneSoldier::NormalAttack()
{
    if(!DroneClass) return;
    if(IsDead()) return;
    
    auto SpawnLoc = GetActorLocation() + GetActorForwardVector() * Monster_AttackRange_Plus + GetActorUpVector() * DroneHeight;
    auto Drone = GetWorld()->SpawnActor<AEnemy_Drone>(DroneClass, SpawnLoc, GetActorRotation()); if(!Drone) return;
	Drone->SetOwner(this);
}