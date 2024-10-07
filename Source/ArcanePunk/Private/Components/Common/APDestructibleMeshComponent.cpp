

#include "Components/Common/APDestructibleMeshComponent.h"

#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

UAPDestructibleMeshComponent::UAPDestructibleMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    for(float & Limit : DamageThreshold) Limit = 1.0f;
    bEnableDamageFromCollision = false;

	APFieldSystem = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystem"));
	APDestructionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestructionSphere"));

	APRadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloff"));
	APCullingField = CreateDefaultSubobject<UCullingField>(TEXT("CullingField"));
	APRadialVector = CreateDefaultSubobject<URadialVector>(TEXT("RadialVector"));
    
	APFieldSystem->SetupAttachment(this); APFieldSystem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	APDestructionSphere->SetupAttachment(this); APDestructionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UAPDestructibleMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	int32 Index = 0;
	for(auto Mat : GetMaterials())
	{
		auto CurrentMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mat);
		Materials.Emplace(CurrentMat);
		SetMaterial(Index , CurrentMat); Index++;
	}	
	CurrentFadeOutSpeed = FadeOutSpeed;

    SetComponentTickEnabled(false);
}

void UAPDestructibleMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bFadeOut)
	{
		Opacity = FMath::FInterpConstantTo(Opacity, 0.0f, DeltaTime, CurrentFadeOutSpeed);
		for(auto Mat : Materials)
		{
			Mat->SetScalarParameterValue(TEXT("Apperence"), Opacity);
		}

		if(Opacity <= 0.5f) CurrentFadeOutSpeed = FadeOutSpeed * 4.5f;
		if(Opacity <= KINDA_SMALL_NUMBER) GetOwner()->Destroy();
	}
}

void UAPDestructibleMeshComponent::Explode(const FVector & HitLocation)
{
	SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	RecreatePhysicsState();

	APRadialFalloff->SetRadialFalloff(FalloffCoefficient, 0.0f, 1.0f, 0.0f, APDestructionSphere->GetScaledSphereRadius(), HitLocation, EFieldFalloffType::Field_FallOff_None);
	APRadialVector->SetRadialVector(VectorCoefficient, HitLocation);
	APCullingField->SetCullingField(APRadialFalloff, APRadialVector, EFieldCullingOperationType::Field_Culling_Outside);
	
	APFieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, APRadialFalloff);
	APFieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearVelocity, nullptr, APCullingField);
}

void UAPDestructibleMeshComponent::StartFadeOut()
{
	SetComponentTickEnabled(true);
	bFadeOut = true;
}