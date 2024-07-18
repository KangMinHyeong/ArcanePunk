
#include "GameElements/Prop/APDestructibleMeshBase.h"

#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Common/APManaDropComponent.h"

AAPDestructibleMeshBase::AAPDestructibleMeshBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	APDestructibleMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("DestructibleMesh"));
	APFieldSystem = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystem"));
	APDestructionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestructionSphere"));

	APRadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloff"));
	APCullingField = CreateDefaultSubobject<UCullingField>(TEXT("CullingField"));
	APRadialVector = CreateDefaultSubobject<URadialVector>(TEXT("RadialVector"));
	APManaDropComp = CreateDefaultSubobject<UAPManaDropComponent>(TEXT("APManaDropComp"));

	SetRootComponent(APDestructibleMesh);
	APFieldSystem->SetupAttachment(APDestructibleMesh);
	APDestructionSphere->SetupAttachment(APDestructibleMesh);
}

void AAPDestructibleMeshBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPDestructibleMeshBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AAPDestructibleMeshBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
	if(HP <= KINDA_SMALL_NUMBER) return 0.0f;

	Explode();
	APManaDropComp->SpawnManaDrop(DamageCauser);
	HP = 0.0f;
	
    return DamageApplied;
}

void AAPDestructibleMeshBase::Explode()
{
	APDestructibleMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	APDestructibleMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	APDestructibleMesh->RecreatePhysicsState();

	APRadialFalloff->SetRadialFalloff(FalloffCoefficient, 0.0f, 1.0f, 0.0f, APDestructionSphere->GetScaledSphereRadius(), APDestructionSphere->GetComponentLocation(), EFieldFalloffType::Field_FallOff_None);
	APRadialVector->SetRadialVector(VectorCoefficient, APDestructionSphere->GetComponentLocation());
	APCullingField->SetCullingField(APRadialFalloff, APRadialVector, EFieldCullingOperationType::Field_Culling_Outside);
	
	APFieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, APRadialFalloff);
	APFieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearVelocity, nullptr, APCullingField);

}