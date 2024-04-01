
#include "GhostTrail/APGhostTrail.h"

#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMaterialLibrary.h"

AAPGhostTrail::AAPGhostTrail()
{
	PrimaryActorTick.bCanEverTick = true;

	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("POSEABLEMESH"));
	RootComponent = PoseableMesh;
}

void AAPGhostTrail::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for(auto Mat : Materials)
	{
		Opacity = FMath::FInterpConstantTo(Opacity, -0.1f, DeltaTime, FadeOutSpeed);
		Mat->SetScalarParameterValue(TEXT("Opacity"), Opacity);
	}
	if(Opacity < 0.0f) Destroy();
	
}

void AAPGhostTrail::InitPose(USkeletalMeshComponent* OwnerCharacterMesh)
{
	PoseableMesh->CopyPoseFromSkeletalComponent(OwnerCharacterMesh);
	SetActorScale3D(OwnerCharacterMesh->GetComponentScale());

	TArray<UMaterialInterface*> Mats = PoseableMesh->GetMaterials();

	for (int i = 0; i < Mats.Num(); i++)
	{
		Materials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), GhostMaterial));
        PoseableMesh->SetMaterial(i, Materials[i]);
	}

	// //수치가 적을수록 빠르게 삭제됨
 	// fQuarterAlpha = fAlphaCount = 0.5f;
	// bSpawned = true;
}
