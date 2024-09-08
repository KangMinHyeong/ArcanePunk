
#include "GameElements/Prop/APBackGroundAssetBase.h"

AAPBackGroundAssetBase::AAPBackGroundAssetBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BackGroundAsset = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackGroundAsset"));
	SetRootComponent(BackGroundAsset);
}

void AAPBackGroundAssetBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Your BeginPlay"));
	IsSpawn = true;
}
