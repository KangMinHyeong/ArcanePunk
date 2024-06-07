
#include "Skill/ArcaneBall.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcaneBall::AArcaneBall()
{
    BallCenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallCenter"));
    BallTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("BallTrigger"));
    BallEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BallEffect"));
    GravityBallTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("GravityBallTrigger"));

    SetRootComponent(BallTrigger);
    BallCenter->SetupAttachment(BallTrigger);
    BallEffect->SetupAttachment(BallTrigger);
    GravityBallTrigger->SetupAttachment(BallTrigger);

    BallMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BallMoveComp"));
    HomingMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("HomingMoveComp"));

    SkillCategory = ESkillCategory::Projecitle;
}

void AArcaneBall::BeginPlay()
{
    Super::BeginPlay();
    UpdateBallSpeed();
}

void AArcaneBall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bGravityMode)
    {
        TArray<AActor*> DragegedActors;
        GravityBallTrigger->GetOverlappingActors(DragegedActors, AEnemy_CharacterBase::StaticClass());

        for(auto Actor : DragegedActors)
        {
            auto Enemy = Cast<AEnemy_CharacterBase>(Actor); if(!Enemy) continue; 
            if(Enemy->GetDefaultHP() <= KINDA_SMALL_NUMBER ) continue;
            FVector Current = Enemy->GetActorLocation();
            Current = FMath::VInterpConstantTo(Current, GetActorLocation(), DeltaTime, GravitySpeed);
            Enemy->SetActorLocation(Current);
            
        } 
    }
    if(bHoming && !HomingActors.IsEmpty())
    {
        auto Enemy = Cast<AEnemy_CharacterBase>(HomingActors.Top()); if(!Enemy) { HomingActors.Pop(); HomingOrderSet(); return;}
        if(Enemy->GetDefaultHP() > KINDA_SMALL_NUMBER )
        {
            
            
            
		    HomingMoveComp->HomingTargetComponent = Enemy->GetRootComponent();
            
            // FVector Current = GetActorLocation();
            // Current = FMath::VInterpConstantTo(Current, Enemy->GetActorLocation(), DeltaTime, HomingSpeed);
            // SetActorLocation(Current);
        }
        else
        {
            HomingActors.Pop();
            HomingOrderSet();
        }
    }
    if(bDrag)
    {
        CurrentBallSpeed = FMath::FInterpConstantTo(CurrentBallSpeed, 0.01f, DeltaTime, HomingSpeed);
        BallMoveComp->SetVelocityInLocalSpace(FVector(CurrentBallSpeed, 0.0f, 0.0f));
        if(CurrentBallSpeed < 1.0f) 
        {
            BallMoveComp->SetVelocityInLocalSpace(FVector(0.0f, 0.0f, 0.0f));
            bDrag = false;  
        }
    }
}

void AArcaneBall::BintOverlap()
{
    if(bPenetrate)
    {
        BallTrigger->OnComponentBeginOverlap.AddDynamic(this, &AArcaneBall::OnOverlap);
        BallTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    }
    else
    {
        BallTrigger->OnComponentHit.AddDynamic(this, &AArcaneBall::OnHitting);
        BallTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
        BallTrigger->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
        BallTrigger->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block); 
        BallTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Block);
        BallTrigger->SetCollisionResponseToChannel(ECC_Vehicle, ECollisionResponse::ECR_Block);
        BallTrigger->SetCollisionResponseToChannel(ECC_Destructible, ECollisionResponse::ECR_Block);
        BallMoveComp->bShouldBounce = true;
        HomingMoveComp->bShouldBounce = true;
    }    
}

void AArcaneBall::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	
    if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
		HitPointComp->DistinctHitPoint(BallHitEffect, OtherActor->GetActorLocation(), OtherActor);
        OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); 
        if(OwnerCharacter.IsValid()) 
        {
            float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, SweepResult, true);
		    OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
        }
		// if(OwnerCharacter.IsValid()) UGameplayStatics::ApplyDamage(OtherActor, OwnerCharacter->GetCurrentATK()* OwnerCharacter->CriticalCalculate() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
	}

    switch (PenetrateType)
    {
        case EPenetrateType::None:
        Destroy();
        break;
    
        case EPenetrateType::Enemy:
        if(OtherActor->ActorHasTag(TEXT("Enemy"))) {PenetrateCount--;}
        else {Destroy();}
        break;

        case EPenetrateType::Object:
        if(!OtherActor->ActorHasTag(TEXT("Enemy"))) {PenetrateCount--;}
        else {Destroy();}
        break;
    }

    if(PenetrateCount == 0) Destroy();
}

void AArcaneBall::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

    if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
        if(OtherActor->ActorHasTag(TEXT("Enemy"))) {PenetrateCount--;}
        else {Destroy();}
       	if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);

		HitPointComp->DistinctHitPoint(BallHitEffect, Hit.Location, OtherActor);
        OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); 
        if(OwnerCharacter.IsValid()) 
        {
            float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, Hit, true);
		    OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
        }
		//  UGameplayStatics::ApplyDamage(OtherActor, OwnerCharacter->GetCurrentATK()* OwnerCharacter->CriticalCalculate() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
	}
    if(PenetrateCount == 0) Destroy();
}

void AArcaneBall::UpdateBallSpeed()
{
    BallMoveComp->SetVelocityInLocalSpace(FVector(BallSpeed, 0.0f, 0.0f));
    BallMoveComp->MaxSpeed = BallSpeed;
    HomingMoveComp->MaxSpeed = BallSpeed;
    CurrentBallSpeed = BallSpeed;
}

float AArcaneBall::GetBallSpeed() const
{
    return BallSpeed;
}

void AArcaneBall::SetDeadTime(float DeadTime)
{
    Super::SetDeadTime(DeadTime);
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneBall::Explosion, DeadTime, false);
}

void AArcaneBall::SetBallRadius(float Radius)
{
    float size = Radius / BallTrigger->GetScaledSphereRadius();
    SetActorScale3D(GetActorScale3D() * size);
}

void AArcaneBall::Explosion()
{
    Destroy();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AArcaneBall::SpawnGravityBall(float Radius)
{
    GravityBallTrigger->SetSphereRadius(Radius); 
    GravityBallTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    bGravityMode = true;
}

void AArcaneBall::HomingOrderSet()
{
    Super::HomingOrderSet();
    HomingMoveComp->bIsHomingProjectile = true;
    HomingMoveComp->HomingAccelerationMagnitude = HomingSpeed;
}

void AArcaneBall::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
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
    
    // CheckGoldEnhance(SkillAbilityNestingData.GoldAbilityNestingNum);
    // CheckPlatinumEnhance(SkillAbilityNestingData.PlatinumAbilityNestingNum);
    BintOverlap();
        
	BallEffect->SetNiagaraVariableLinearColor(TEXT("Color"),  EffectColor);
    SetActorTickEnabled(true);
}

void AArcaneBall::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 투사체 속도 증가
        SkillAbilityComponent->Coefficient_Add(BallSpeed,AbilityData->Coefficient_X, NestingNum);
        UpdateBallSpeed();
        break;

        case 2: // 투사체 사거리 증가, SkillNumber8에서 이미 처리
        break;

        case 3:
        SkillAbilityComponent->GigantGradually(AbilityData->Coefficient_X, NestingNum );
        break;
    }
}

void AArcaneBall::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 관통 - 몬스터
        SkillAbilityComponent->Coefficient_Add(PenetrateCount,AbilityData->Coefficient_X, NestingNum);
        PenetrateType = EPenetrateType::Enemy;
        bPenetrate = true;
        break;

        case 2: // 관통 - 오브젝트
        SkillAbilityComponent->Coefficient_Add(PenetrateCount,AbilityData->Coefficient_X, NestingNum);
        PenetrateType = EPenetrateType::Object;
        bPenetrate = true;
        break;

        case 3: // 바운스
        SkillAbilityComponent->Coefficient_Add(PenetrateCount,AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

void AArcaneBall::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    StateTime = 0.0f;
    FTimerHandle HomingTimerHandle;
    switch (AbilityNum)
    {
        case 1: // 중력장
        SpawnGravityBall(BallTrigger->GetScaledSphereRadius()* AbilityData->Coefficient_X);
        GravitySpeed =  AbilityData->Coefficient_Y;
        break;

        case 2: // 기절
        SkillAbilityComponent->Coefficient_Add(StateTime, AbilityData->Coefficient_X, NestingNum);
        StateTime = FMath::Min(SkillComp->GetCoolTime() *0.8f, StateTime);
        bStun = true;
        UE_LOG(LogTemp, Display, TEXT("StateTime %f"), StateTime);
        break;

        case 3: // 가장 가까운적 유도
        // GetWorld()->GetTimerManager().SetTimer(HomingTimerHandle, this, &AArcaneBall::HomingOrderSet, AbilityData->Coefficient_X, false);
        bDrag = true;
        HomingOrderSet();
        GetWorldTimerManager().ClearTimer(DestroyTimerHandle);   
        break;
    }
}
