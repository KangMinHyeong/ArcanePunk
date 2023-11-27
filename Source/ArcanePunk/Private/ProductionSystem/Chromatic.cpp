#include "ArcanePunk/Public/ProductionSystem/Chromatic.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"

AChromatic::AChromatic()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetRelativeScale3D(FVector(1, 1, 1));
	BoxComponent->SetHiddenInGame(false);


	// 머티리얼 변경할 Mesh 생성 및 설정
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(RootComponent);
	BoxMesh->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Game/Material/ChromaticMaterial.ChromaticMaterial"));

	if (MaterialFinder.Succeeded())
	{
		YourDesiredMaterial = MaterialFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> OriginalMaterialFinder(TEXT("/Game/Materials/M_Mesh.M_Mesh"));

	if (OriginalMaterialFinder.Succeeded())
	{
		OriginalMaterial = OriginalMaterialFinder.Object;
	}
}

void AChromatic::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AChromatic::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AChromatic::OnOverlapEnd);
}

void AChromatic::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
   if (YourDesiredMaterial)
   {
       UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(YourDesiredMaterial, this);

       if (DynamicMaterial)
       {
           BoxMesh->SetMaterial(0, DynamicMaterial);
       }
   }
}

void AChromatic::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OriginalMaterial)
	{
		UMaterialInstanceDynamic* Origin = UMaterialInstanceDynamic::Create(OriginalMaterial, this);

		if (Origin)
		{
			BoxMesh->SetMaterial(0, Origin);
		}
	}
}
