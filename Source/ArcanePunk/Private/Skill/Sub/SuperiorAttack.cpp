
#include "Skill/Sub/SuperiorAttack.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"

ASuperiorAttack::ASuperiorAttack()
{
    SuperiorAttackTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SuperiorAttackTrigger"));
    SuperiorAttackEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SuperiorAttackEffect"));

    SetRootComponent(SuperiorAttackTrigger);
    SuperiorAttackEffect->SetupAttachment(SuperiorAttackTrigger);
}

void ASuperiorAttack::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void ASuperiorAttack::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bActivate)
    {
        CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
        SetActorLocation(CurrentLocation);

        CurrentWidth = FMath::FInterpConstantTo(CurrentWidth, TargetWidth, DeltaTime, ScaleSpeed);
        SuperiorAttackTrigger->SetSphereRadius(CurrentWidth);

        SuperiorAttackEffect->SetRelativeScale3D( InitScale * (CurrentWidth / InitWidth));

        if((CurrentLocation - TargetLocation).Size() <= KINDA_SMALL_NUMBER)
        {
            TargetLocation = TargetLocation + GetActorForwardVector() * 150.0f;
            bActivate = false;
            SuperiorAttackTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
        }
    }
    else
    {
        CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed*0.85f);
        SetActorLocation(CurrentLocation);
        CurrentWidth = FMath::FInterpConstantTo(CurrentWidth, 0.0f, DeltaTime, ScaleSpeed*0.45f);
        SuperiorAttackEffect->SetRelativeScale3D( InitScale * (CurrentWidth / InitWidth));
        if((CurrentLocation - TargetLocation).Size() <= KINDA_SMALL_NUMBER) Destroy();
    }     
}

void ASuperiorAttack::DestroySKill()
{
    Super::DestroySKill();
}

void ASuperiorAttack::SetTargetAndWidth(FVector Target, float Width)
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    CurrentLocation = GetActorLocation(); CurrentWidth = SuperiorAttackTrigger->GetScaledSphereRadius(); InitWidth = CurrentWidth;
    TargetLocation = Target; TargetWidth = Width;

    InitScale = SuperiorAttackEffect->GetComponentScale();

    MoveSpeed = (TargetLocation - CurrentLocation).Size() * InitSpeed;
    ScaleSpeed = (TargetWidth - CurrentWidth) * InitSpeed;

    bActivate = true;
    SetActorTickEnabled(true);
    SuperiorAttackTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASuperiorAttack::OnOverlap);
    SuperiorAttackTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ASuperiorAttack::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if(Actors.Contains(OtherActor)) return;
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
		// HitPointComp->DistinctHitPoint(OtherActor->GetActorLocation(), OtherActor);
        HitDelay(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, HitNumbers, HitDelayTime, true);
        Actors.Add(OtherActor);
	}
}
