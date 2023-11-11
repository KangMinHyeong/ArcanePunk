// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeWallBase.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"

// Sets default values
AFadeWallBase::AFadeWallBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkinMesh"));

}

// Called when the game starts or when spawned
void AFadeWallBase::BeginPlay()
{
	Super::BeginPlay();
	
	
	SkinMaterial = SkinMesh->GetMaterial(0)->GetMaterial();

	// SkinMesh->GetMaterials()
	// if(!SkinMaterial) return;
	
	
	// SetScalarParameterValue
}

// Called every frame
void AFadeWallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

