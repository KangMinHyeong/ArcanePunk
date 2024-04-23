
#include "Skill/SuperiorMode.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Skill/Sub/SuperiorAttack.h"

ASuperiorMode::ASuperiorMode()
{
    SuperiorModeRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SuperiorModeRoot"));

    SetRootComponent(SuperiorModeRoot);
}

void ASuperiorMode::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void ASuperiorMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ASuperiorMode::DestroySKill()
{
    OwnerCharacter->GetAttackComponent()->SetSuperiorMode(false);
    DeActivate_Ult();
    Super::DestroySKill();
}

void ASuperiorMode::SpawnAddtionalAttack(float Dist, float Width)
{
    if(!OwnerCharacter.IsValid()) return;
    FVector Target = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * Dist * MaxRange;

    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    auto SuperiorAttack = GetWorld()->SpawnActor<ASuperiorAttack>(SuperiorAttackClass, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
    if(SuperiorAttack)
    {
        SuperiorAttack->SetOwner(GetOwner());
        SuperiorAttack->SetTargetAndWidth(Target, Width * 2.0f);
    } 
}

void ASuperiorMode::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);
    if(!OwnerCharacter.IsValid()) return;
    
    auto NC = UNiagaraFunctionLibrary::SpawnSystemAttached(SuperiorModeEffect, OwnerCharacter->GetMesh(), TEXT("WindRushEffect"), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), FVector(1,1,1), EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::None, true);
    NC->SetNiagaraVariableFloat(TEXT("Time"), DestroyTime);

    OwnerCharacter->GetBuffComp()->ATKSpeedUp(ATKSpeedCoefficient, DestroyTime); 

    OwnerCharacter->GetAttackComponent()->SetSuperiorMode(true);
    OwnerCharacter->GetAttackComponent()->OnSuperiorAttack.AddUObject(this, &ASuperiorMode::SpawnAddtionalAttack);
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASuperiorMode::DestroySKill, DestroyTime, false);
}