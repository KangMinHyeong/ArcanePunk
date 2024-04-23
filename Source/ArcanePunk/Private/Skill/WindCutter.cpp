
#include "Skill/WindCutter.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Enemy/Enemy_CharacterBase.h"

AWindCutter::AWindCutter()
{
    WindRoot = CreateDefaultSubobject<USphereComponent>(TEXT("WindRoot"));
    WindTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("WindTrigger"));

    SetRootComponent(WindRoot);
    WindTrigger->SetupAttachment(WindRoot);
}

void AWindCutter::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AWindCutter::DestroySKill()
{
    WindEffectComp->DeactivateImmediate();
    WindGroundEffectComp->Deactivate();
    Super::DestroySKill();
}

void AWindCutter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetWindTrigger(DeltaTime);
}

void AWindCutter::SetWindTrigger(float DeltaTime)
{
    CurrentWindWitdh = FMath::FInterpConstantTo(CurrentWindWitdh, MaxWindWitdh, DeltaTime, WitdhSpeed);
    WindTrigger->SetSphereRadius(CurrentWindWitdh, false); 

    CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, MaxLocation, DeltaTime, LocationSpeed);
    WindTrigger->SetWorldLocation(CurrentLocation);
    
    CurrentRadius = FMath::FInterpConstantTo(CurrentRadius, MaxRadius, DeltaTime, RadiusSpeed);
    WindRoot->SetSphereRadius(CurrentRadius, false); 

    if(abs(MaxRadius - CurrentRadius) <= KINDA_SMALL_NUMBER) DestroySKill();
}

void AWindCutter::SetMaxLocation(FVector Location)
{
    MaxLocation =  Location + GetActorLocation(); MaxLocation.Z = GetActorLocation().Z;
    
    // MaxLocation = (MaxLocation - GetActorLocation()) / (MaxLocation - GetActorLocation()).Size();
}

void AWindCutter::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	
    if(OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
	{
		// HitPointComp->DistinctHitPoint(OtherActor->GetActorLocation(), OtherActor);
        auto Enemy = Cast<AEnemy_CharacterBase>(OtherActor); if(!Enemy) return;
        if(Enemy->GetMesh()->IsOverlappingComponent(WindTrigger))
        {
            HitDelay(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, HitNumbers, HitDelayTime, true);
        }
	}
}

void AWindCutter::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    
    CurrentWindWitdh = WindTrigger->GetScaledSphereRadius();
    CurrentLocation = GetActorLocation();
    CurrentRadius = WindRoot->GetScaledSphereRadius();
    WitdhSpeed = WindSpeed *MaxWindWitdh;
    LocationSpeed = WindSpeed *(MaxLocation - CurrentLocation).Size();
    RadiusSpeed = WindSpeed * MaxRadius;

    WindRoot->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    WindTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    WindRoot->OnComponentBeginOverlap.AddDynamic(this, &AWindCutter::OnOverlap);

    WindEffectComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WindEffect, WindTrigger->GetComponentLocation(), WindTrigger->GetComponentRotation());
    WindEffectComp->SetNiagaraVariableFloat(TEXT("Time"), WindSpeed);
    WindEffectComp->SetNiagaraVariableFloat(TEXT("Size"), MaxWindWitdh * 0.02f);
    WindEffectComp->SetNiagaraVariableVec3(TEXT("Velocity"), FVector(LocationSpeed, 0.0f, 0.0f));

    WindGroundEffectComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WindGroundEffect, WindTrigger->GetComponentLocation(), WindTrigger->GetComponentRotation());
    WindGroundEffectComp->SetNiagaraVariableFloat(TEXT("Time"), WindSpeed);
    WindGroundEffectComp->SetNiagaraVariableFloat(TEXT("Size"), LocationSpeed*0.001f);
    // NC->SetNiagaraVariableVec3(TEXT("Velocity"), FVector(LocationSpeed, 0.0f, 0.0f));

    SetActorTickEnabled(true);
}
