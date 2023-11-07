// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanePunkCharacter.h"

// Sets default values
AArcanePunkCharacter::AArcanePunkCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArcanePunkCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArcanePunkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArcanePunkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

