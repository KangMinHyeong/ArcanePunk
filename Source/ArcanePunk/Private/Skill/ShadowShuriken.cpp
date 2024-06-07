// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/ShadowShuriken.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APAttackComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/Sub/ClutchTrigger.h"
#include "Components/SphereComponent.h"

AShadowShuriken::AShadowShuriken()
{
    ShurikenRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ShurikenRoot"));

    SetRootComponent(ShurikenRoot);

}

void AShadowShuriken::BeginPlay()
{
    Super::BeginPlay();  
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);  
}

void AShadowShuriken::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Shurikens.Num() != ShurikenTargetLocation.Num()) return;

    if(!bComplete) {SpreadShuriken(DeltaTime);}
    else {CollectShuriken(DeltaTime);}
    
}

void AShadowShuriken::DestroySKill()
{
    Super::DestroySKill();
}

void AShadowShuriken::SpreadShuriken(float DeltaTime)
{
    int32 CompleteNum = 0;
    for(int32 i = 0; i<Shurikens.Num(); i++)
    {
        FVector Current = Shurikens[i]->GetActorLocation();
        Current = FMath::VInterpConstantTo(Current, ShurikenTargetLocation[i], DeltaTime, ShurikenSpeed);
        Shurikens[i]->SetActorLocation(Current);

        if(abs(Current.X - ShurikenTargetLocation[i].X) < 0.5f && abs(Current.Y - ShurikenTargetLocation[i].Y) < 0.5f )
        {
            CompleteNum++;
        }
    }
    if(CompleteNum == Shurikens.Num())
    {
        for(auto Shuriken : Shurikens) Shuriken->GetClutchSphere()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
        SetActorTickEnabled(false);
        bComplete = true;
        GetWorldTimerManager().SetTimer(CollectTimerHandle, this, &AShadowShuriken::CollectShuriken, CollectTime, false);
    }
}

void AShadowShuriken::CollectShuriken(float DeltaTime)
{
    int32 CompleteNum = 0;
    for(int32 i = 0; i<Shurikens.Num(); i++)
    {
        FVector Current = Shurikens[i]->GetActorLocation(); 
        Current = FMath::VInterpConstantTo(Current, GetActorLocation(), DeltaTime, ShurikenCollectSpeed[i]);
        Shurikens[i]->SetActorLocation(Current);

        if(abs(Current.X - GetActorLocation().X) < 1.0f && abs(Current.Y - GetActorLocation().Y) < 1.0f )
        {
            CompleteNum++;
        }
    }
    if(CompleteNum == Shurikens.Num())
    {
        for(auto Shuriken : Shurikens) Shuriken->Destroy();
        DestroySKill();
    }
}

void AShadowShuriken::SpawnShuriken()
{
    if(!OwnerCharacter.IsValid()) return;
    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    TArray<float> AddLocation_X; TArray<float> AddLocation_Y; AddLocation_X.SetNum(ShurikenNumber); AddLocation_Y.SetNum(ShurikenNumber);
    float Degree = 360.0f / ShurikenNumber; Degree = FMath::DegreesToRadians(Degree);

    for(int32 i = 0; i<ShurikenNumber; i++)
    {
        AddLocation_X[i] = FMath::Cos(Degree * i); AddLocation_Y[i] =  FMath::Sin(Degree * i);
        FVector AddLocation = FVector(AddLocation_X[i], AddLocation_Y[i], 0.0f) * ShurikenDist;

        auto Shuriken = GetWorld()->SpawnActor<AClutchTrigger>(ShurikenClass, GetActorLocation(), GetActorRotation(), SpawnParams);
        if(Shuriken) 
        {
            Shuriken->SetOwner(this); 
            ShurikenTargetLocation.Add(GetActorLocation() + AddLocation); Shurikens.Add(Shuriken);
            
            Shuriken->SetClutchSphere(ShurikenWidth);
        }
    }
    
    SetActorTickEnabled(true);
}

void AShadowShuriken::CollectShuriken()
{
    if(!bComplete) return;
    for(auto Shuriken : Shurikens)
    {
        Shuriken->GetClutchSphere()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
        
        float Speed = (Shuriken->GetActorLocation() - GetActorLocation()).Size() / ShurikenDist; Speed = Speed * ShurikenSpeed * CollectSpeedCoefficient;
        ShurikenCollectSpeed.Add(Speed);
    } 
    SetActorTickEnabled(true);
    DeActivate_Ult();
}

void AShadowShuriken::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent); // Wide 곱해주기 추가
    if(!OwnerCharacter.IsValid()) return;
    
    CurrentDamage = OwnerCharacter->GetCurrentATK() * DamageCoefficient;

    SpawnShuriken();
    // GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AShadowShuriken::DestroySKill, DestroyTime, false);
}

