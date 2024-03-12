
#include "Skill/ArcaneBall.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"

AArcaneBall::AArcaneBall()
{
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("BallTrigger"));
    BallEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BallEffect"));

    SetRootComponent(BallMesh);
    BallTrigger->SetupAttachment(BallMesh);
    BallEffect->SetupAttachment(BallMesh);

    BallMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BallMoveComp"));
    BallMoveComp->MaxSpeed = BallSpeed;
	BallMoveComp->InitialSpeed = BallSpeed;

    SkillCategory = ESkillCategory::Projecitle;
}

void AArcaneBall::BeginPlay()
{
    Super::BeginPlay();

    BintOverlap();
}

void AArcaneBall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AArcaneBall::BintOverlap()
{
    BallTrigger->OnComponentBeginOverlap.AddDynamic(this, &AArcaneBall::OnOverlap);
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
	
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    if(OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
	{
		// if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
		HitPointComp->DistinctHitPoint(SweepResult.Location, OtherActor);
		UGameplayStatics::ApplyDamage(OtherActor, OwnerCharacter->GetFinalATK() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
	}
}

float AArcaneBall::GetCutterSpeed() const
{
    return BallMoveComp->InitialSpeed;
}

void AArcaneBall::SetDeadTime(float DeadTime)
{
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneBall::Explosion, DeadTime, false);
}

void AArcaneBall::SetBallRadius(float Radius)
{
    float size = Radius / BallTrigger->GetScaledSphereRadius();
    SetActorScale3D(GetActorScale3D() * size);
}

void AArcaneBall::Explosion()
{
    float Size =  GetActorScale3D().Y / DefaultSize;
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAttackComponent()->MultiAttack(GetActorLocation(), GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, ExplosionRadius * Size, DamageCoefficient, HitNumbers, bStun, StateTime);
    DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius* Size, 18, FColor::Green,false, 2.5f);

    TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_8);
	if(SkillNum.IsValid())
	{
		SkillNum->SkillEnd();
	} 

    Destroy();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AArcaneBall::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);

	BallEffect->SetNiagaraVariableLinearColor(TEXT("Color"),  EffectColor);
}


