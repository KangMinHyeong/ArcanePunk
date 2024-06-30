
#include "GhostTrail/APGhostTrail.h"

#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Common/APGhostTrailSpawnComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"

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

	if(bRunTrail)
	{
		Opacity = FMath::FInterpConstantTo(Opacity, -0.1f, DeltaTime, FadeOutSpeed);
		for(auto Mat : Materials)
		{
			Mat->SetScalarParameterValue(TEXT("Opacity"), Opacity);
		}
		if(Opacity <= 0.0f) Destroy();
	}
	if(bSkillTrail && OwnerCharacter.IsValid())
	{
		if(OwnerCharacter->GetRSkillNumber()->IsActivate()) {Destroy(); return;}
	}
	
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
	bRunTrail = true;
	// //수치가 적을수록 빠르게 삭제됨
 	// fQuarterAlpha = fAlphaCount = 0.5f;
	// bSpawned = true;
}

void AAPGhostTrail::InitSkillPose(USkeletalMeshComponent* OwnerCharacterMesh, float TraceTime)
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());

	SetActorTickEnabled(false);
	PoseableMesh->CopyPoseFromSkeletalComponent(OwnerCharacterMesh);
	SetActorScale3D(OwnerCharacterMesh->GetComponentScale());

	TArray<UMaterialInterface*> Mats = PoseableMesh->GetMaterials();

	for (int i = 0; i < Mats.Num(); i++)
	{
		Materials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), GhostMaterial));
        PoseableMesh->SetMaterial(i, Materials[i]);
	}
	for(auto Mat : Materials)
	{
		Mat->SetScalarParameterValue(TEXT("Opacity"), DefaultOpacity);
	}
	
	if(TraceTime > 0.0f) GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPGhostTrail::SetVisibility, TraceTime, false);
	// //수치가 적을수록 빠르게 삭제됨
 	// fQuarterAlpha = fAlphaCount = 0.5f;
	// bSpawned = true;
}

void AAPGhostTrail::InitSkillPoseFirst(USkeletalMeshComponent* OwnerCharacterMesh, float TraceTime)
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());

	PoseableMesh->CopyPoseFromSkeletalComponent(OwnerCharacterMesh);
	SetActorScale3D(OwnerCharacterMesh->GetComponentScale());

	TArray<UMaterialInterface*> Mats = PoseableMesh->GetMaterials();

	for (int i = 0; i < Mats.Num(); i++)
	{
		Materials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), GhostMaterial));
        PoseableMesh->SetMaterial(i, Materials[i]);
	}
	for(auto Mat : Materials)
	{
		Mat->SetScalarParameterValue(TEXT("Opacity"), DefaultOpacity);
	}

	bSkillTrail = true;

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPGhostTrail::OnDestory, TraceTime, false);
}

void AAPGhostTrail::SetVisibility()
{
	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetGhostTrailSpawnComp()->SetSkillTrail(this);
	if(OwnerCharacter->GetRSkillNumber()->IsActivate()) {Destroy(); return;}
	SetActorHiddenInGame(false); 
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPGhostTrail::OnDestory, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), false);
}

void AAPGhostTrail::OnDestory()
{
	Destroy();
}