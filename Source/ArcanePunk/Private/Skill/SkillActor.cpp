// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "Skill/SkillDataManager.h"


ASkillActor::ASkillActor()
{
	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;
}

// Called when the game starts or when spawned
void ASkillActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillActor::UseSkill()
{
	SetActive(true);
	SetActorLocation(OwnerCharacter->GetMesh()->GetSocketLocation(SocketName));
	SkillEffectComponent->SetActive(true);
	SkillEffectComponent->Activate(true);
	
	SkillEffectComponent->SetVisibility(true);
	SkillEffectComponent->SetHiddenInGame(false);
	SkillEffectComponent->Activate(true);
	SkillEffectComponent->SetActive(true); 
	
	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation: %s"), *OwnerCharacter->GetMesh()->GetSocketLocation(SocketName).ToString());
	UE_LOG(LogTemp, Log, TEXT("Niagara Component Active: %s"), SkillEffectComponent->IsActive() ? TEXT("True") : TEXT("False"));
	UE_LOG(LogTemp, Log, TEXT("Niagara Component Location: %s"), *SkillEffectComponent->GetComponentLocation().ToString());
	if (SkillEffectComponent->GetAsset()) {
		UE_LOG(LogTemp, Log, TEXT("Niagara Asset is loaded."));
	} else {
		UE_LOG(LogTemp, Error, TEXT("Niagara Asset is not loaded."));
	}
	if (SkillEffectComponent->IsRegistered()) {
		UE_LOG(LogTemp, Log, TEXT("Niagara Component registered."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Niagara Component not registered."));
	} 
}

void ASkillActor::InitSkill(FName SkillNameKey, AArcanePunkCharacter* OwnerCharacterPtr)
{
	SkillName = SkillNameKey;
	auto skillDataRow = USkillDataManager::GetInstance()->GetSkillData(SkillName);
	SkillType = skillDataRow.SkillType;
	
	SkillEffectComponent = Cast<UNiagaraComponent>(AddComponentByClass(UNiagaraComponent::StaticClass(), false, FTransform::Identity, true));
	
	if (SkillEffectComponent)
	{
		UNiagaraSystem* NiagaraSystem = skillDataRow.SkillEffect.LoadSynchronous();
		SkillEffectComponent->SetAsset(NiagaraSystem);
		SkillEffectComponent->SetupAttachment(RootComponent); 
		SkillEffectComponent->SetAutoActivate(true);
		SkillEffectComponent->RegisterComponent();
	}
	
	CollisionShape = Cast<UShapeComponent>(AddComponentByClass(skillDataRow.CollisionShapeClass, false, FTransform::Identity, true));
	if (CollisionShape)
	{
		
		CollisionShape->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		CollisionShape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionShape->RegisterComponent();
		
		auto collisionShapeSize = skillDataRow.CollisionSize;
		
		if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CollisionShape))
		{
			CapsuleComponent->SetCapsuleSize(collisionShapeSize.X, collisionShapeSize.Y);  // Size.X = 반지름, Size.Y = 높이
		}
		else if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionShape))
		{
			BoxComponent->SetBoxExtent(collisionShapeSize);  // Box 크기 설정 (X, Y, Z)
		}
		else if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionShape))
		{
			SphereComponent->SetSphereRadius(collisionShapeSize.X);  // Size.X = 반지름
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unsupported collision shape class: %s"), *CollisionShape->GetName());
		}
	}

	SocketName = skillDataRow.SocketName;
	
	this->OwnerCharacter = OwnerCharacterPtr;

	SetActive(false);
}

void ASkillActor::SetActive(bool Active)
{
	SetActorHiddenInGame(!Active);
	CollisionShape->SetActive(Active);
}	
