
#include "Enemy/Enemy_TutorialDummy.h"

AEnemy_TutorialDummy::AEnemy_TutorialDummy()
{
}

void AEnemy_TutorialDummy::BeginPlay()
{
    Super::BeginPlay();

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
}

float AEnemy_TutorialDummy::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
    return 0.0f;
}