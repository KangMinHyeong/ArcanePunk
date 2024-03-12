// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/InterfaceTestActor.h"

// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();

	// InteractableData = InstanceInteractableData;
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::BeginFocus()
{
	//IInteractionInterface::BeginFocus();
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus()
{
	//IInteractionInterface::EndFocus();
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::BeginInteract()
{
	//IInteractionInterface::BeginInteract();
	UE_LOG(LogTemp, Warning, TEXT("Calling BegginInteract override on interface test actor"));
}

void AInterfaceTestActor::EndInteract()
{
	//IInteractionInterface::EndInteract();
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor"));
}

void AInterfaceTestActor::Interact(AArcanePunkCharacter* PlayerCharacter)
{
	//IInteractionInterface::Interact();
	UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor"));
}

