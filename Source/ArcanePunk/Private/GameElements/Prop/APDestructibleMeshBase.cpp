
#include "GameElements/Prop/APDestructibleMeshBase.h"

#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Common/APManaDropComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMaterialLibrary.h"

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
	
	int32 Index = 0;
	for(auto Mat : APDestructibleMesh->GetMaterials())
	{
		auto CurrentMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mat);
		Materials.Emplace(CurrentMat);
		APDestructibleMesh->SetMaterial(Index , CurrentMat); Index++;
	}	

	CurrentFadeOutSpeed = FadeOutSpeed;

	SetActorTickEnabled(false);
}

void AAPDestructibleMeshBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bFadeOut)
	{
		Opacity = FMath::FInterpConstantTo(Opacity, 0.0f, DeltaTime, CurrentFadeOutSpeed);
		for(auto Mat : Materials)
		{
			Mat->SetScalarParameterValue(TEXT("Apperence"), Opacity);
		}

		if(Opacity <= 0.5f) CurrentFadeOutSpeed = FadeOutSpeed * 5.0f;
		if(Opacity <= KINDA_SMALL_NUMBER) Destroy();
	}
}

float AAPDestructibleMeshBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
	if(HP <= KINDA_SMALL_NUMBER) return 0.0f;

	FVector HitLocation = GetActorLocation();

	if(!bCenterExplosion)
	{
		float ActualDamage = DamageAmount;
		UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*) &DamageEvent;
			ActualDamage = InternalTakePointDamage(ActualDamage, *PointDamageEvent, EventInstigator, DamageCauser);

			if (ActualDamage != 0.f)
			{
				ReceivePointDamage(ActualDamage, DamageTypeCDO, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.ImpactNormal, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.BoneName, PointDamageEvent->ShotDirection, EventInstigator, DamageCauser, PointDamageEvent->HitInfo);
				OnTakePointDamage.Broadcast(this, ActualDamage, EventInstigator, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.BoneName, PointDamageEvent->ShotDirection, DamageTypeCDO, DamageCauser);
				HitLocation = PointDamageEvent->HitInfo.ImpactPoint;
			}
		}
	}
	

	Explode(HitLocation);
	APManaDropComp->SpawnManaDrop(DamageCauser);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPDestructibleMeshBase::StartFadeOut, FadeOutTime, false);

	HP = 0.0f;
    return DamageApplied;
}

void AAPDestructibleMeshBase::Explode(FVector HitLocation)
{
	APDestructibleMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	APDestructibleMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	APDestructibleMesh->RecreatePhysicsState();

	APRadialFalloff->SetRadialFalloff(FalloffCoefficient, 0.0f, 1.0f, 0.0f, APDestructionSphere->GetScaledSphereRadius(), HitLocation, EFieldFalloffType::Field_FallOff_None);
	APRadialVector->SetRadialVector(VectorCoefficient, HitLocation);
	APCullingField->SetCullingField(APRadialFalloff, APRadialVector, EFieldCullingOperationType::Field_Culling_Outside);
	
	APFieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, APRadialFalloff);
	APFieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearVelocity, nullptr, APCullingField);

}

void AAPDestructibleMeshBase::StartFadeOut()
{
	SetActorTickEnabled(true);
	bFadeOut = true;
}