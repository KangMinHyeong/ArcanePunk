// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FootPrint/APFootPrint.h"

#include "Components/DecalComponent.h"

// Sets default values
AAPFootPrint::AAPFootPrint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	FootDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("FootDecal"));

	SetRootComponent(RootComp);
	FootDecal->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AAPFootPrint::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	FootDecal->FadeStartDelay = FadeStartDelay;
	FootDecal->FadeDuration = FadeOutTime;
	FootDecal->CreateDynamicMaterialInstance()->SetScalarParameterValue(TEXT("Opacity"),Opacity);

	GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, this, &AAPFootPrint::FadeOut, FadeStartDelay, false);
}

void AAPFootPrint::FadeOut()
{
	GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	SetActorTickEnabled(true);
}

// Called every frame
void AAPFootPrint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Opacity = FMath::FInterpConstantTo(Opacity, 0.0f, DeltaTime, FadeOutSpeed);
	FootDecal->CreateDynamicMaterialInstance()->SetScalarParameterValue(TEXT("Opacity"), Opacity);
}

