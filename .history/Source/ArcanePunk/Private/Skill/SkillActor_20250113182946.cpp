// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Skill/SkillDataManager.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Skill/SkillController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Skill/Install/APInstallBase.h"

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
	if(bDrag) DragSkill(DeltaTime);
	if(bCharging) ChargindCheck(DeltaTime);
}

void ASkillActor::PlaySkillAction(ESkillKey SkillKey)
{
	CurrentSkillKey = SkillKey;
	OwnerCharacter->PlaySkillAction(SkillAction);
	TargetLoc = OwnerController->GetSkillTargetRange()->GetActorLocation();

	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &ASkillActor::UseSkill);
	OwnerCharacter->OnSkillChargingTrigger.AddDynamic(this, &ASkillActor::ChargingSkill);
}

void ASkillActor::UseSkill()
{
	SetActorLocation(OwnerCharacter->GetMesh()->GetSocketLocation(SocketName));
	
	if(!bAttachedEffect)
	{
		SkillEffectComponent->SetRelativeLocation(GetActorLocation()); 
		SkillEffectComponent->SetRelativeRotation(OwnerCharacter->GetActorRotation()); 
		SkillEffectComponent->SetUsingAbsoluteRotation(true);
	}
	
	FVector CharacterForwardVector = OwnerCharacter->GetActorForwardVector();
	FVector LaunchDirection = CharacterForwardVector.RotateAngleAxis(LaunchAngle, FVector(0, 0, 1)); // Z축을 기준으로 회전
	ProjectileMovementComponent->Velocity = LaunchDirection * ProjectileMovementComponent->InitialSpeed;
	
	OwnerCharacter->OnSkillTrigger.RemoveDynamic(this, &ASkillActor::UseSkill);
	OwnerCharacter->OnSkillChargingTrigger.RemoveDynamic(this, &ASkillActor::ChargingSkill);

	OwnerCharacter->GetAPHUD()->OnUpdateMPBar.Broadcast(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->OnOperateSkill.Broadcast(CurrentSkillKey);

	bSkilling = true;
	CurrentPenetrateCount = PenetrateCount;
	CheckSet.Empty();

	// 사거리에 도달하면 비활성화, 상황에 따라 이펙트 재생 추가
	if(SkillType == ESkillType::Projectile)
	{
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASkillActor::SetActive, false);
		float Time = (Range * RangeCoefficient) / ProjectileMovementComponent->InitialSpeed;
		Time *= bDrag ? DragSpeed : 1.0f;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, Time, false);

		StartCoolDown();
	}
	else if(SkillType == ESkillType::Immediate)
	{
		TArray<AActor*> Actors;
		CollisionShape->GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
		for (AActor* Actor : Actors)
		{
			FHitResult HitResult;
			float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Actor, CalculateDamage(), HitResult, true, CrowdControl, CC_Duration);
		}
		bSkilling = false;
		CollisionShape->SetActive(false);
		
		StartCoolDown();
	}
	else if(SkillType == ESkillType::Charging)
	{
		FRotator Rot = OwnerCharacter->GetActorRotation(); Rot.Pitch += 90.0f;
		SetActorRotation(Rot);
		SetActorLocation(OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * HalfHeight * ChargingCoefficient);
		
		CollisionShape->SetRelativeScale3D(FVector(1.0f, 1.0f, ChargingCoefficient));
		
		TArray<AActor*> Actors;
		CollisionShape->GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
		for (AActor* Actor : Actors)
		{
			FHitResult HitResult;
			float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Actor, CalculateDamage(), HitResult, true, CrowdControl, CC_Duration);
		}
		
		bSkilling = false;
		CollisionShape->SetActive(false);

		float Wid = Width / 50.0f;
    	float Len = (HalfHeight * ChargingCoefficient * 2.0f) / 750.0f;
		SkillEffectComponent->SetVariableFloat(TEXT("Width"),  Wid);
    	SkillEffectComponent->SetVariableFloat(TEXT("Length"),  Len);
    	SkillEffectComponent->SetVariableVec2(TEXT("Size2D"),  FVector2D(Wid, Len));
    	SkillEffectComponent->SetVariableVec3(TEXT("Size3D"),  FVector(Wid, Len, 1.0f));
    	SkillEffectComponent->SetVariableVec3(TEXT("AddVector"),  FVector(((HalfHeight * ChargingCoefficient * 2.0f) - 750.0f) * 0.5f, 0.0f, 0.0f));

		StartCoolDown();
	}
	else if(SkillType == ESkillType::Installation)
	{
		FActorSpawnParameters SpawnParams = InitSpawnParams();

		SetActorLocation(TargetLoc);
		auto InstallActor = GetWorld()->SpawnActor<AAPInstallBase>(InstallActorClass, GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
		if(InstallActor)
		{
			InstallActor->InitInstall(this);
		}
	}
	
	SetActive(true);

	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation: %s"), *OwnerCharacter->GetMesh()->GetSocketLocation(SocketName).ToString());
}

void ASkillActor::EnhanceSkill(int32 EnhanceNumber)
{
	uint8 CurrentNestingNumber = 0;
	if(EnhanceNestingData.Contains(EnhanceNumber))
	{
		CurrentNestingNumber = EnhanceNestingData[EnhanceNumber];
	}
	CurrentNestingNumber++;

	switch (EnhanceNumber)
	{
	case 1: // Damege Module
		DamageCoefficient = 1.0f + ((float)CurrentNestingNumber * 0.1f);
		break;
	
	case 2: // Range Module - Test
		RangeCoefficient = 1.0f + ((float)CurrentNestingNumber * 0.3f);
		break;
	}
}

void ASkillActor::InitSkill(FName SkillNameKey, AArcanePunkCharacter* OwnerCharacterPtr, ASkillController* OwnerControllerPtr)
{
	SkillName = SkillNameKey;
	auto skillDataRow = USkillDataManager::GetInstance()->GetSkillData(SkillName);
	SkillType = skillDataRow.SkillType;
	CrowdControl = skillDataRow.CrowdControl;
	SkillTargetRangeClass = skillDataRow.SkillTargetRangeClass;
	SkillGroundRangeClass = skillDataRow.SkillGroundRangeClass;
	InstallActorClass = skillDataRow.InstallActorClass;

	// 스킬 이펙트 Attached 인지
	bAttachedEffect = skillDataRow.bAttachedEffect;

	// 스킬 사거리
	Range = skillDataRow.Range - skillDataRow.CollisionSize.X * 2.0f;
	Width = skillDataRow.CollisionSize.X;

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
			HalfHeight = collisionShapeSize.Y;
		}
		else if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionShape))
		{
			BoxComponent->SetBoxExtent(collisionShapeSize);  // Box 크기 설정 (X, Y, Z)
			HalfHeight = collisionShapeSize.Z;
		}
		else if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionShape))
		{
			SphereComponent->SetSphereRadius(collisionShapeSize.X);  // Size.X = 반지름
			HalfHeight = collisionShapeSize.X;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unsupported collision shape class: %s"), *CollisionShape->GetName());
		}

		// 충돌처리 및 오버랩함수 바인딩
		CollisionShape->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionShape->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
        CollisionShape->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap); 
		CollisionShape->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap); 
		
		switch (SkillType)
		{
		case ESkillType::Projectile:
			CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &ASkillActor::OnOverlap_Projectile);
			break;		
		}
		
	}

	// 스킬 이펙트 설정
	SkillEffectComponent = Cast<UNiagaraComponent>(AddComponentByClass(UNiagaraComponent::StaticClass(), false, FTransform::Identity, true));
	if (SkillEffectComponent)
	{
		UNiagaraSystem* NiagaraSystem = skillDataRow.SkillEffect.LoadSynchronous();
		SkillEffectComponent->SetAsset(NiagaraSystem);
		if(bAttachedEffect) SkillEffectComponent->SetupAttachment(RootComponent); 
		SkillEffectComponent->SetAutoActivate(true);
		SkillEffectComponent->RegisterComponent();
		SkillEffectComponent->Deactivate();
	}
	SkillOffEffect = skillDataRow.SkillOffEffect.Get();

	// 발사 속도 설정
	ProjectileMovementComponent->InitialSpeed = skillDataRow.ProjectileSpeed;
	SkillEffectComponent->SetVariableFloat(TEXT("Speed"),  ProjectileMovementComponent->InitialSpeed);

	// 발사 각 설정
	LaunchAngle = skillDataRow.LaunchAngle;

	// 생성 위치 소켓 설정
	SocketName = skillDataRow.SocketName;

	// 스킬 몽타주 설정
	SkillAction = skillDataRow.SkillAction;

	// 스킬 쿨타임
	CoolTime = skillDataRow.CoolTime;

	// CC 지속시간
	CC_Duration = skillDataRow.CC_Duration;

	// 마나 소모량
	MPConsumption = skillDataRow.MPConsumption;

	// 관통 횟수 - 몬스터 통과 제한 횟수
	PenetrateCount = skillDataRow.PenetrateCount;

	// 최대 차징 계수
	MaxCharging = skillDataRow.MaxCharging;

	// Drag On, Off
	bDrag = skillDataRow.bDrag;
	DragSpeed = ProjectileMovementComponent->InitialSpeed / Range;
	SkillEffectComponent->SetVariableFloat(TEXT("Drag"),  DragSpeed);
	
	this->OwnerCharacter = OwnerCharacterPtr;
	this->OwnerController = OwnerControllerPtr;

	SetActive(false);
}

void ASkillActor::SetActive(bool Active)
{
	// SetActorHiddenInGame(!Active); // SetHiddenInGame으로 처리하기기
	CollisionShape->SetActive(Active);

	if(Active) {SkillEffectComponent->Activate();}
	else {SkillEffectComponent->DeactivateImmediate();}	

	if(bSkilling && !Active)
	{
		// Skill Off Effect
		ProjectileMovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);
		bSkilling = false;
		auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SkillOffEffect, GetActorLocation(), GetActorRotation());
	}
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

void ASkillActor::OnOverlap_Projectile(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(!bSkilling) return;
	if(!OwnerCharacter || CurrentPenetrateCount == 0) return;
	if(!OtherActor || OtherActor == this || OtherActor == OwnerCharacter) return;
	if(CheckSet.Contains(OtherActor)) return;
	
	float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, CalculateDamage(), SweepResult, true, CrowdControl, CC_Duration);
	// OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());

	if(OtherActor->ActorHasTag("Enemy"))
	{
		CurrentPenetrateCount--;
		CheckSet.Emplace(OtherActor);
		UE_LOG(LogTemp, Display, TEXT("CurrentPenetrateCount %d"), CurrentPenetrateCount);
		if(CurrentPenetrateCount == 0) SetActive(false);
	}
	else {SetActive(false);}
}

void ASkillActor::DragSkill(float DeltaTime)
{
	ProjectileMovementComponent->Velocity = FMath::VInterpTo(
		ProjectileMovementComponent->Velocity, 
		FVector::ZeroVector, 
		DeltaTime, 
		DragSpeed
	);
}

void ASkillActor::ChargindCheck(float DeltaTime)
{
	bool Check = false;
	switch (CurrentSkillKey)
    {
    case ESkillKey::Q:
        Check = OwnerCharacter->GetOnQSkill();
        break;
    case ESkillKey::E:
        Check = OwnerCharacter->GetOnESkill();
        break;
    case ESkillKey::R:
        Check = OwnerCharacter->GetOnRSkill();
        break;
    }

	if(Check)
	{
		OwnerCharacter->GetAPHUD()->OnChargingGauge.Broadcast(ChargingCoefficient - 1.0f , MaxCharging - 1.0f);
    	ChargingCoefficient = FMath::FInterpConstantTo(ChargingCoefficient, MaxCharging, DeltaTime, 0.7f);
		OwnerController->GetSkillTargetRange()->SetLength(ChargingCoefficient * HalfHeight);
	}
	else
	{
		auto Anim = OwnerCharacter->GetMesh()->GetAnimInstance();
		Anim->Montage_JumpToSection(TEXT("Fire"), SkillAction);		
	}
}

void ASkillActor::StartCoolDown()
{
	OwnerCharacter->GetAPHUD()->OnStartCoolTime.Broadcast(CurrentSkillKey, CoolTime);
}

void ASkillActor::ChargingSkill()
{
	OwnerCharacter->GetAPHUD()->OnChargingEnd.Broadcast(true);
	bCharging = true;
	ChargingCoefficient = 1.0f;
	ChargingEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SkillOffEffect, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());

	OwnerCharacter->OnSkillChargingEndTrigger.AddDynamic(this, &ASkillActor::ChargingEnd);
}

void ASkillActor::ChargingEnd()
{
	OwnerCharacter->OnSkillChargingEndTrigger.RemoveDynamic(this, &ASkillActor::ChargingEnd);

	ChargingEffect->DeactivateImmediate();
	bCharging = false;
	OwnerCharacter->GetAPHUD()->OnChargingEnd.Broadcast(false);
	OwnerController->RotatePlayer();
}

float ASkillActor::CalculateDamage()
{
    return OwnerCharacter->GetCurrentATK() * DamageCoefficient * ChargingCoefficient;
}

FActorSpawnParameters ASkillActor::InitSpawnParams()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    return SpawnParams;
}