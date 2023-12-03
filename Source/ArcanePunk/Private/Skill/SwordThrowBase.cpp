// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SwordThrowBase.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Components/BoxComponent.h"

// Sets default values
ASwordThrowBase::ASwordThrowBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	HitTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("HitTrigger"));

	SetRootComponent(HitTrigger);
	Sword->SetupAttachment(HitTrigger);
	TrailEffect->SetupAttachment(HitTrigger);
	
	SwordMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SwordMovementComponent"));
	SwordMovementComponent->MaxSpeed = SwordSpeed;
	SwordMovementComponent->InitialSpeed = SwordSpeed;
}

// Called when the game starts or when spawned
void ASwordThrowBase::BeginPlay()
{
	Super::BeginPlay();
	
	HitTrigger->OnComponentHit.AddDynamic(this, &ASwordThrowBase::OnHitting);
	//HitTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASwordThrowBase::DestroySword, DestroyTime, false);
}

// Called every frame
void ASwordThrowBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordThrowBase::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	UE_LOG(LogTemp, Display, TEXT("Your message"));
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	auto Character = Cast<AArcanePunkCharacter>(MyOwner);
	
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && Character)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Character->GetPlayerStatus().ATK * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
		if(HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), FVector(0.2f,0.2f,0.2f));
		}
	}
}

void ASwordThrowBase::DestroySword()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}
