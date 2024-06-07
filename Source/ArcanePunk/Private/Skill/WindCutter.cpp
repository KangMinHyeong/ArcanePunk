
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
    WindEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WindEffect"));

    SetRootComponent(WindRoot);
    WindTrigger->SetupAttachment(WindRoot);
    WindEffect->SetupAttachment(WindTrigger);
}

void AWindCutter::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AWindCutter::DestroySKill()
{
    Super::DestroySKill();
    // WindEffect->Deactivate();
    // WindRoot->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
    // WindTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AWindCutter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // SetWindSpeed(DeltaTime);
    SetWindTrigger(DeltaTime);
}

void AWindCutter::SetWindSpeed(float DeltaTime)
{
    if(LocationSpeed <= 200.0f)
    {
        WitdhSpeed = FMath::FInterpConstantTo(WitdhSpeed, 0.0f, DeltaTime, WindSpeed * 500.0f);
        LocationSpeed = FMath::FInterpConstantTo(LocationSpeed, 0.0f, DeltaTime, WindSpeed * 5000.0f);
        RadiusSpeed = FMath::FInterpConstantTo(RadiusSpeed, 0.0f, DeltaTime, WindSpeed * 500.0f);
    }
    else
    {
        WitdhSpeed = FMath::FInterpTo(WitdhSpeed, 0.0f, DeltaTime, WindSpeed);
        LocationSpeed = FMath::FInterpTo(LocationSpeed, 0.0f, DeltaTime, WindSpeed);
        RadiusSpeed = FMath::FInterpTo(RadiusSpeed, 0.0f, DeltaTime, WindSpeed);
    }

    if(LocationSpeed <= KINDA_SMALL_NUMBER) DestroySKill();
}

void AWindCutter::SetWindTrigger(float DeltaTime)
{
    CurrentWindWitdh = FMath::FInterpConstantTo(CurrentWindWitdh, MaxWindWitdh, DeltaTime, WitdhSpeed);
    WindTrigger->SetSphereRadius(CurrentWindWitdh, false); 

    CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, MaxLocation, DeltaTime, LocationSpeed);
    WindTrigger->SetWorldLocation(CurrentLocation);

    // float CurrentEffectScale = (CurrentWindWitdh * EffectScale) / 100.0f;
    // WindEffect->SetRelativeScale3D(FVector(CurrentEffectScale, CurrentEffectScale, CurrentEffectScale));

    // UE_LOG(LogTemp, Display, TEXT("CurrentEffectScale %f"), CurrentEffectScale);
    CurrentRadius = FMath::FInterpConstantTo(CurrentRadius, MaxRadius, DeltaTime, RadiusSpeed);
    WindRoot->SetSphereRadius(CurrentRadius, true); 

    if(abs(MaxRadius - CurrentRadius) <= KINDA_SMALL_NUMBER) DestroySKill();
}

void AWindCutter::SetMaxLocation(FVector Location)
{
    MaxLocation =  Location + GetActorLocation(); MaxLocation.Z = GetActorLocation().Z;
    
    // MaxLocation = (MaxLocation - GetActorLocation()) / (MaxLocation - GetActorLocation()).Size();
}

void AWindCutter::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    UE_LOG(LogTemp, Display, TEXT("Your OnOverlap"));
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
        if(WindTrigger->IsOverlappingActor(OtherActor))
        {
            HitDelay(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, HitNumbers, HitDelayTime, true);
        }
	}
}

void AWindCutter::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
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

    float CurrentEffectScale = (MaxWindWitdh * EffectScale) / 100.0f;
    WindEffect->SetRelativeScale3D(FVector(CurrentEffectScale, CurrentEffectScale, CurrentEffectScale));
    WindEffect->SetNiagaraVariableFloat(TEXT("Time"), 2.0f/ WindSpeed);

    // WindEffectComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WindEffect, WindTrigger->GetComponentLocation(), WindTrigger->GetComponentRotation());
    // WindEffect->SetNiagaraVariableFloat(TEXT("Time"), 2.0f/ WindSpeed);
    // WindEffectComp->SetNiagaraVariableFloat(TEXT("Size"), MaxWindWitdh * 0.02f);
    // WindEffectComp->SetNiagaraVariableVec3(TEXT("Velocity"), FVector(LocationSpeed, 0.0f, 0.0f));

    // WindGroundEffectComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WindGroundEffect, WindTrigger->GetComponentLocation(), WindTrigger->GetComponentRotation());
    // WindGroundEffectComp->SetNiagaraVariableFloat(TEXT("Time"), WindSpeed);
    // WindGroundEffectComp->SetNiagaraVariableFloat(TEXT("Size"), LocationSpeed*0.001f);
    // NC->SetNiagaraVariableVec3(TEXT("Velocity"), FVector(LocationSpeed, 0.0f, 0.0f));

    SetActorTickEnabled(true);
}
