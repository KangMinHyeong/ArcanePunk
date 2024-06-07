
#include "Skill/ArcaneArea.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "Skill/Sub/AreaBeam.h"

AArcaneArea::AArcaneArea()
{
    AreaRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AreaRoot"));
    SetRootComponent(AreaRoot);
}

void AArcaneArea::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AArcaneArea::DestroySKill()
{
    DeActivate_Ult();
    Super::DestroySKill();
}

void AArcaneArea::SpwanAreaBeam()
{
    if(!OwnerCharacter.IsValid()) return;
    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    float Degree = 360.0f / AreaBeamNumber; Degree = FMath::DegreesToRadians(Degree);
    float Length = AreaDist * FMath::Sin(Degree * 0.5f) * 2.0f;

    TArray<AAreaBeam*> AreaBeams;
    for(int32 i = 0; i<AreaBeamNumber; i++)
    {
        float AddLocation_X = FMath::Cos(Degree * i); float AddLocation_Y =  FMath::Sin(Degree * i);
        FVector AddLocation = FVector(AddLocation_X, AddLocation_Y, 0.0f) * AreaDist;

        auto AreaBeam = GetWorld()->SpawnActor<AAreaBeam>(AreaBeamClass, GetActorLocation() + AddLocation, GetActorRotation(), SpawnParams);
        if(AreaBeam) 
        {
            AreaBeam->SetOwner(GetOwner()); 
            AreaBeams.Add(AreaBeam);
        }
    }

    for(int32 i = 0; i<AreaBeams.Num(); i++)
    {
        if(i == 0) {AreaBeams[i]->SetBeamRotation(AreaBeams[AreaBeams.Num() - 1]->GetActorLocation() );}
        else {AreaBeams[i]->SetBeamRotation(AreaBeams[i-1]->GetActorLocation() );} 

        AreaBeams[i]->SetAreaLength(Length);
        AreaBeams[i]->SetAreaWidth(AreaWidth);
        AreaBeams[i]->SetDestroyTime(DestroyTime);
        AreaBeams[i]->SetSkill(SkillAbilityData, SkillComp.Get());
    }
}

void AArcaneArea::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;

    SpwanAreaBeam();
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneArea::DestroySKill, DestroyTime, false);
}

