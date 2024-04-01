
#include "Skill/SwordClutch.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/Sub/ClutchTrigger.h"
#include "Components/SphereComponent.h"

ASwordClutch::ASwordClutch()
{
    ClutchRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ClutchRoot"));

    SetRootComponent(ClutchRoot);

    SkillCategory = ESkillCategory::Spawn_Dynamic;
}

void ASwordClutch::BeginPlay()
{
    Super::BeginPlay();  

}

void ASwordClutch::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(OwnerCharacter.IsValid())
    {
        SetActorLocation(OwnerCharacter->GetActorLocation());
        SetActorRotation(FMath::RInterpTo(GetActorRotation(),FRotator(0, GetActorRotation().Yaw + 90.0f, 0), DeltaTime, ClutchSpeed));
    }
}

void ASwordClutch::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData); // Wide 곱해주기 추가

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
    SpawnClutchTrigger();

    SetActorTickEnabled(true);
}

void ASwordClutch::DestroySKill()
{
    DeActivate(ESkillNumber::Skill_14);
    Super::DestroySKill();

}

void ASwordClutch::SpawnClutchTrigger()
{
    if(!OwnerCharacter.IsValid()) return;
    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    TArray<float> AddLocation_X; TArray<float> AddLocation_Y; AddLocation_X.SetNum(4); AddLocation_Y.SetNum(4);
    AddLocation_X[0] = 1.0f; AddLocation_Y[0] = 0.0f; AddLocation_X[1] = 0.0f; AddLocation_Y[1] = -1.0f; 
    AddLocation_X[2] = -1.0f; AddLocation_Y[2] = 0.0f; AddLocation_X[3] = 0.0f; AddLocation_Y[3] = 1.0f; 

    for(int32 i = 0; i<4; i++)
    {
        FVector AddLocation = FVector(AddLocation_X[i], AddLocation_Y[i], 0.0f) * Distance;

        auto ClutchTrigger = GetWorld()->SpawnActor<AClutchTrigger>(ClutchTriggerClass, GetActorLocation() + AddLocation, GetActorRotation(), SpawnParams);
        if(ClutchTrigger) 
        {
            ClutchTrigger->SetOwner(OwnerCharacter.Get()); 
            ClutchTrigger->AttachToComponent( ClutchRoot, FAttachmentTransformRules ::KeepWorldTransform);
            ClutchTrigger->SetClutchSphere(Wide/2.0f);
            ClutchTrigger->SetDestroy(DestroyTime);
        }
    }
    
}
