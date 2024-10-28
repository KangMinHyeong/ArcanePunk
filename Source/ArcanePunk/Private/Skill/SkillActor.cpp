// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Skill/SkillDataManager.h"


ASkillActor::ASkillActor()
{
	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	RootComponent = SceneComp;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASkillActor::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITOR
	// 에디터에서만 틱 활성화
	SetActorTickEnabled(true);
#endif
}

// Called every frame
void ASkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugForShapeComponent();
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

	FVector CharacterForwardVector = OwnerCharacter->GetActorForwardVector();
	FVector LaunchDirection = CharacterForwardVector.RotateAngleAxis(LaunchAngle, FVector(0, 0, 1)); // Z축을 기준으로 회전
	ProjectileMovementComponent->Velocity = LaunchDirection * ProjectileMovementComponent->InitialSpeed;

	OwnerCharacter->PlaySkillAction(SkillAction);
	
	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation: %s"), *OwnerCharacter->GetMesh()->GetSocketLocation(SocketName).ToString());
}

void ASkillActor::InitSkill(FName SkillNameKey, AArcanePunkCharacter* OwnerCharacterPtr)
{
	SkillName = SkillNameKey;
	auto skillDataRow = USkillDataManager::GetInstance()->GetSkillData(SkillName);
	SkillType = skillDataRow.SkillType;

	// 스킬 이펙트 설정
	SkillEffectComponent = Cast<UNiagaraComponent>(AddComponentByClass(UNiagaraComponent::StaticClass(), false, FTransform::Identity, true));
	if (SkillEffectComponent)
	{
		UNiagaraSystem* NiagaraSystem = skillDataRow.SkillEffect.LoadSynchronous();
		SkillEffectComponent->SetAsset(NiagaraSystem);
		SkillEffectComponent->SetupAttachment(RootComponent); 
		SkillEffectComponent->SetAutoActivate(true);
		SkillEffectComponent->RegisterComponent();
	}

	// 충돌체 설정
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

	// 발사 속도 설정
	ProjectileMovementComponent->InitialSpeed = skillDataRow.ProjectileSpeed;

	// 발사 각 설정
	LaunchAngle = skillDataRow.LaunchAngle;

	// 생성 위치 소켓 설정
	SocketName = skillDataRow.SocketName;

	// 스킬 몽타주 설정
	SkillAction = skillDataRow.SkillAction;
	
	this->OwnerCharacter = OwnerCharacterPtr;

	SetActive(false);
}

void ASkillActor::SetActive(bool Active)
{
	SetActorHiddenInGame(!Active);
	CollisionShape->SetActive(Active);
}	

void ASkillActor::DrawDebugForShapeComponent() const
{
	UWorld* World = GetWorld();
	if (!World || !CollisionShape || !CollisionShape->IsActive())
	{
		return;
	}

	FVector Location = CollisionShape->GetComponentLocation();
	FQuat Rotation = CollisionShape->GetComponentQuat();
	
	if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(CollisionShape))
	{
		DrawDebugCapsule(
			World,
			Location,
			Capsule->GetScaledCapsuleHalfHeight(),
			Capsule->GetScaledCapsuleRadius(),
			Rotation,
			FColor::Green,
			false,
			0,
			0,
			2.0f
		);
	}
	else if (USphereComponent* Sphere = Cast<USphereComponent>(CollisionShape))
	{
		DrawDebugSphere(
			World,
			Location,
			Sphere->GetScaledSphereRadius(),
			12,
			FColor::Blue,
			false,
			0,
			0,
			2.0f
		);
	}
	else if (UBoxComponent* Box = Cast<UBoxComponent>(CollisionShape))
	{
		DrawDebugBox(
			World,
			Location,
			Box->GetScaledBoxExtent(),
			Rotation,
			FColor::Red,
			false,
			0,
			0,
			2.0f
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unsupported UShapeComponent type for debug drawing."));
	}
}