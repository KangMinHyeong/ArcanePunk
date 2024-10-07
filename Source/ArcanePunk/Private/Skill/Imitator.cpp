
#include "Skill/Imitator.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameElements/DamageText/DamageText.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/Character/SkillNumber/UltSkillNumber_20.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

AImitator::AImitator()
{   
    ImitatorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ImitatorTrigger"));
    ImitatorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ImitatorMesh"));
    ImitatorEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ImitatorEffect"));
     
    SetRootComponent(ImitatorTrigger);
    ImitatorMesh->SetupAttachment(ImitatorTrigger);
    ImitatorEffect->SetupAttachment(ImitatorTrigger);

}

void AImitator::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);    
}

void AImitator::DestroySKill()
{
	Super::DestroySKill();
}

void AImitator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CurrentRot = CurrentRot + CustomRot;
    SetActorRotation(CurrentRot);
}

void AImitator::OnOverlaping(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor == GetOwner())
	{	
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;

        CopySkillNum = FMath::RandRange(1, SkillNum->NumEnums() - 2);
        while((UltSkillNumber_20->GetCurrentSkillNumber() == (ESkillNumber)CopySkillNum))
        {
            CopySkillNum = FMath::RandRange(1, SkillNum->NumEnums() - 2);
        }

        UltSkillNumber_20->SetCopySkill((ESkillNumber)CopySkillNum);	
        DestroySKill();
	}
}

void AImitator::CollisionEnableStart()
{
    SetActorHiddenInGame(false);
    ImitatorTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
    ImitatorTrigger->OnComponentBeginOverlap.AddDynamic(this, &AImitator::OnOverlaping);
    CurrentRot = GetActorRotation();
    SetActorTickEnabled(true);
}

void AImitator::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData , SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    UltSkillNumber_20 = Cast<UUltSkillNumber_20>(SkillComp.Get()); if(!UltSkillNumber_20.IsValid()) return;

    auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImitatorSpawnEffect, GetActorLocation(), GetActorRotation());
    SetActorHiddenInGame(true);

    CustomRot.Yaw = FMath::RandRange(CustomRot.Yaw - 1.0f, CustomRot.Yaw+1.0f);

    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AImitator::CollisionEnableStart, DestroyTime, false);
}
