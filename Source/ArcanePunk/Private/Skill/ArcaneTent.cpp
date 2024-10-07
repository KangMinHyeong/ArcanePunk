
#include "Skill/ArcaneTent.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/Character/APSkillHubComponent.h"

AArcaneTent::AArcaneTent()
{
    TentTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("TentTrigger"));
    TentDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("TentDecal"));

    SetRootComponent(TentTrigger);
    TentDecal->SetupAttachment(TentTrigger);

    SkillCategory = ESkillCategory::Spawn_Static;
}

void AArcaneTent::BeginPlay()
{
    Super::BeginPlay();  
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);  
}

void AArcaneTent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bFollowing)
    {
        SetActorLocation(FMath::VInterpTo(GetActorLocation(), FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, GetActorLocation().Z) , DeltaTime, FollowingSpeed));
        if(abs(OwnerCharacter->GetActorLocation().X - GetActorLocation().X) < 1.0f && abs(OwnerCharacter->GetActorLocation().Y - GetActorLocation().Y) < 1.0f ) 
        {
            bFollowing = false;
            SetActorTickEnabled(false);
        }
    }
}

void AArcaneTent::DestroySKill()
{
    DeActivate(ESkillNumber::Skill_13);

    if(OwnerCharacter.IsValid()) OwnerCharacter->SetInArcaneTent(false);
    Super::DestroySKill();
}

void AArcaneTent::OnBeginHiding(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if(bDrainField)
    {
        auto Enemy = Cast<AEnemy_CharacterBase>(OtherActor);
        if(Enemy) Enemy->AddInDrainField(true);
        UE_LOG(LogTemp, Display, TEXT("Your a"));
    }
    
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
    if(OtherActor != GetOwner()) return; if(!OwnerCharacter.IsValid()) return;

    OwnerCharacter->SetInArcaneTent(true);

    CheckBuff(true);
    OwnerCharacter->GetAttackComponent()->SetFieldDrainCoefficient(OwnerCharacter->GetAttackComponent()->GetFieldDrainCoefficient() + TentFieldDrainCoefficient );
}

void AArcaneTent::OnEndHiding(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    if(bDrainField)
    {
        auto Enemy = Cast<AEnemy_CharacterBase>(OtherActor);
        if(Enemy) Enemy->AddInDrainField(false);
        UE_LOG(LogTemp, Display, TEXT("Your b"));
    }

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
    if(OtherActor != GetOwner()) return; if(!OwnerCharacter.IsValid()) return;

    OwnerCharacter->SetInArcaneTent(false);
    if(bFollowing) SetActorTickEnabled(true);
    CheckBuff(false);
    OwnerCharacter->GetAttackComponent()->SetFieldDrainCoefficient(OwnerCharacter->GetAttackComponent()->GetFieldDrainCoefficient() - TentFieldDrainCoefficient );
}

void AArcaneTent::SetDotDamage()
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(TEXT("Enemy")))
        {              
            auto Enemy = Cast<AEnemy_CharacterBase>(Actor); bool AlreadyHitCheck = Enemy->IsHitting(); FHitResult HitResult;
            
            float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Enemy, Enemy->GetDefaultHP() * DotPercent, HitResult, false);
		    OwnerCharacter->GetAttackComponent()->DrainCheck(Enemy, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
            // UGameplayStatics::ApplyDamage(Actor, , OwnerCharacter->GetInstigatorController(), this, UDamageType::StaticClass());
            if(!AlreadyHitCheck) Enemy->SetHitting(false);        
        }
    } 
}

void AArcaneTent::CheckOverlap(bool NewBool)
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(TEXT("Enemy")))
        {
            auto Enemy = Cast<AEnemy_CharacterBase>(Actor); if(!Enemy) return;
            Enemy->AddInDrainField(NewBool);
        }
    }
}

void AArcaneTent::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    
    for(auto It : SkillAbilityNestingData.SilverAbilityNestingNum)
    {
        CheckSilverEnhance(It.Key, It.Value);
    }
    for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    {
        CheckGoldEnhance(It.Key, It.Value);
    }
    for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    {
        CheckPlatinumEnhance(It.Key, It.Value);
    }

    OwnerCharacter->SetReturnToHideTime(ReturnToHideTime); OwnerCharacter->SetInArcaneTent(true); CheckBuff(true);
    TentTrigger->OnComponentBeginOverlap.AddDynamic(this, &AArcaneTent::OnBeginHiding);
    TentTrigger->OnComponentEndOverlap.AddDynamic(this, &AArcaneTent::OnEndHiding);
    if(bDrainField) CheckOverlap(true);
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneTent::DestroySKill, DestroyTime, false);
}

void AArcaneTent::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    float Max;
    switch (AbilityNum)
    {
        case 1: // 지속시간 증가
        SkillAbilityComponent->Coefficient_Add(DestroyTime, AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 텐트 크기 증가
        Max = SkillAbilityComponent->SizeUp_Multiple(AbilityData->Coefficient_X, NestingNum);
        SetActorScale3D(GetActorScale3D()* Max);
        break;

    }
}

void AArcaneTent::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 추가 도트뎀
        TentTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
        SkillAbilityComponent->Coefficient_Add(DotPercent, AbilityData->Coefficient_X, NestingNum);
        GetWorld()->GetTimerManager().SetTimer(DotRateTimerHandle, this, &AArcaneTent::SetDotDamage, DotRateTime, true);
        SetDotDamage();
        break;

        case 2: // 이속 증가
        SkillAbilityComponent->Coefficient_Add(FastCoefficient, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::Speed;
        break;

        case 3: // 공속 증가
        SkillAbilityComponent->Coefficient_Add(ATKSpeedCoefficient, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::ATKSpeed;
        break;
        
        case 4: // 공격력 증가
        SkillAbilityComponent->Coefficient_Add(ATKCoefficient, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::ATK;
        break;
    }
}

void AArcaneTent::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 체력 회복
        bDrainField = true;
        TentFieldDrainCoefficient = AbilityData->Coefficient_X;
        OwnerCharacter->GetAttackComponent()->SetFieldDrainCoefficient(TentFieldDrainCoefficient + OwnerCharacter->GetAttackComponent()->GetFieldDrainCoefficient());
        TentTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
        break;

        case 2: // 절대 은신
        OwnerCharacter->SetEnhanceTent(true);
        break;
        
        case 3: // 텐트 플레이어 따라옴
        bFollowing = true;
        FollowingSpeed = AbilityData->Coefficient_X;
        break;
    }
}