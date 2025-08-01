// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActors/SkillActor.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Skill/SkillController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Skill/Install/APInstallBase.h"
#include "Character/ArcanePunkCharacter.h"
#include "UserInterface/HUD/APHUD.h"
#include "Components/Character/APAttackComponent.h"

ASkillActor::ASkillActor()
{
	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// TODO: ProjectileActor로 옮기기
	// ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	// ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
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
	// DrawDebugForShapeComponent();
}

void ASkillActor::PlaySkillAction()
{
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &ASkillActor::UseSkill);
}

void ASkillActor::UseSkill()
{
	// SetActorLocation(OwnerCharacter->GetMesh()->GetSocketLocation(SkillActorData.SocketName));
	
	// if(!bAttachedEffect)
	// {
	// 	SkillEffectComponent->SetRelativeLocation(GetActorLocation()); 
	// 	SkillEffectComponent->SetRelativeRotation(OwnerCharacter->GetActorRotation()); 
	// 	SkillEffectComponent->SetUsingAbsoluteRotation(true);
	// }
	
	// FVector CharacterForwardVector = OwnerCharacter->GetActorForwardVector();
	// FVector LaunchDirection = CharacterForwardVector.RotateAngleAxis(SkillActorData.LaunchAngle, FVector(0, 0, 1)); // Z축을 기준으로 회전
	// ProjectileMovementComponent->Velocity = LaunchDirection * ProjectileMovementComponent->InitialSpeed;
	// 
	// OwnerCharacter->OnSkillTrigger.RemoveDynamic(this, &ASkillActor::UseSkill);
	// OwnerCharacter->OnSkillChargingTrigger.RemoveDynamic(this, &ASkillActor::ChargingSkill);

	// TODO: SkillController 쪽으로 옮기기
	// OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	// OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(CurrentSkillKey);

	// TODO: if문 없애고 상속받아 타입별 클래스 구분
	// 사거리에 도달하면 비활성화, 상황에 따라 이펙트 재생 추가
	// if(SkillType == ESkillType::Projectile)
	// {
	// 	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASkillActor::SetActive, false);
	// 	float Time = (Range * RangeCoefficient) / ProjectileMovementComponent->InitialSpeed;
	// 	Time *= bDrag ? DragSpeed : 1.0f;
	// 	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, Time, false);
	//
	// 	StartCoolDown();
	// }
	// else if(SkillType == ESkillType::Immediate)
	// {
	// 	TArray<AActor*> Actors;
	// 	CollisionShape->GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
	// 	for (AActor* Actor : Actors)
	// 	{
	// 		FHitResult HitResult;
	// 		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Actor, CalculateDamage(), HitResult, true, CrowdControl, CC_Duration);
	// 	}
	// 	bSkilling = false;
	// 	CollisionShape->SetActive(false);
	// 	
	// 	StartCoolDown();
	// }
	// else if(SkillType == ESkillType::Charging)
	// {
	// 	FRotator Rot = OwnerCharacter->GetActorRotation(); Rot.Pitch += 90.0f;
	// 	SetActorRotation(Rot);
	// 	SetActorLocation(OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * HalfHeight * ChargingCoefficient);
	// 	
	// 	CollisionShape->SetRelativeScale3D(FVector(1.0f, 1.0f, ChargingCoefficient));
	// 	
	// 	TArray<AActor*> Actors;
	// 	CollisionShape->GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
	// 	for (AActor* Actor : Actors)
	// 	{
	// 		FHitResult HitResult;
	// 		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Actor, CalculateDamage(), HitResult, true, CrowdControl, CC_Duration);
	// 	}
	// 	
	// 	bSkilling = false;
	// 	CollisionShape->SetActive(false);
	//
	// 	float Wid = Width / 50.0f;
 //    	float Len = (HalfHeight * ChargingCoefficient * 2.0f) / 750.0f;
	// 	SkillEffectComponent->SetVariableFloat(TEXT("Width"),  Wid);
 //    	SkillEffectComponent->SetVariableFloat(TEXT("Length"),  Len);
 //    	SkillEffectComponent->SetVariableVec2(TEXT("Size2D"),  FVector2D(Wid, Len));
 //    	SkillEffectComponent->SetVariableVec3(TEXT("Size3D"),  FVector(Wid, Len, 1.0f));
 //    	SkillEffectComponent->SetVariableVec3(TEXT("AddVector"),  FVector(((HalfHeight * ChargingCoefficient * 2.0f) - 750.0f) * 0.5f, 0.0f, 0.0f));
	//
	// 	StartCoolDown();
	// }
	// else if(SkillType == ESkillType::Installation)
	// {
	// 	FActorSpawnParameters SpawnParams = InitSpawnParams();
	//
	// 	SetActorLocation(TargetLoc);
	// 	auto InstallActor = GetWorld()->SpawnActor<AAPInstallBase>(InstallActorClass, GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);
	// 	if(InstallActor)
	// 	{
	// 		InstallActor->InitInstall(this);
	// 	}
	// }
	//
	// SetActive(true);

	// UE_LOG(LogTemp, Warning, TEXT("SpawnLocation: %s"), *OwnerCharacter->GetMesh()->GetSocketLocation(SkillActorData.SocketName).ToString());
}

void ASkillActor::InitSkill(FName SkillID, TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacterPtr, TWeakObjectPtr<ASkillController> OwnerControllerPtr)
{
	
	SkillId = SkillID;

	// SkillActorData = GetGameInstance()->GetSubsystem<USkillActorDataManager>()->GetSkillData(SkillId);

	// TODO: FSkillData에 들어가는 데이터만 처리하고, 나머지는 상속받는 타입별 스킬 액터 클래스에서 처리
	// CrowdControl = skillDataRow.CrowdControl;
	// SkillTargetRangeClass = skillDataRow.SkillTargetRangeClass;
	// SkillGroundRangeClass = skillDataRow.SkillGroundRangeClass;
	// InstallActorClass = skillDataRow.InstallActorClass;
 //
	// // 스킬 이펙트 Attached 인지
	// bAttachedEffect = skillDataRow.bAttachedEffect;
 //
	// // 스킬 사거리
	// Range = skillDataRow.Range - skillDataRow.CollisionSize.X * 2.0f;
	// Width = skillDataRow.CollisionSize.X;
 //
	// // 충돌체 설정
	// CollisionShape = Cast<UShapeComponent>(AddComponentByClass(skillDataRow.CollisionShapeClass, false, FTransform::Identity, true));
	// if (CollisionShape)
	// {
	// 	
	// 	CollisionShape->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// 	CollisionShape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// 	CollisionShape->RegisterComponent();
	// 	
	// 	auto collisionShapeSize = skillDataRow.CollisionSize;
	// 	
	// 	if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CollisionShape))
	// 	{
	// 		CapsuleComponent->SetCapsuleSize(collisionShapeSize.X, collisionShapeSize.Y);  // Size.X = 반지름, Size.Y = 높이
	// 		HalfHeight = collisionShapeSize.Y;
	// 	}
	// 	else if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionShape))
	// 	{
	// 		BoxComponent->SetBoxExtent(collisionShapeSize);  // Box 크기 설정 (X, Y, Z)
	// 		HalfHeight = collisionShapeSize.Z;
	// 	}
	// 	else if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionShape))
	// 	{
	// 		SphereComponent->SetSphereRadius(collisionShapeSize.X);  // Size.X = 반지름
	// 		HalfHeight = collisionShapeSize.X;
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("Unsupported collision shape class: %s"), *CollisionShape->GetName());
	// 	}
 //
	// 	// 충돌처리 및 오버랩함수 바인딩
	// 	CollisionShape->SetCollisionObjectType(ECC_GameTraceChannel1); 
	// 	CollisionShape->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// 	CollisionShape->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
 //        CollisionShape->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap); 
	// 	CollisionShape->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap); 
	// 	
	// 	switch (SkillType)
	// 	{
	// 	case ESkillType::Projectile:
	// 		CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &ASkillActor::OnOverlap_Projectile);
	// 		break;		
	// 	}
	// }

	// // 스킬 이펙트 설정
	// SkillEffectComponent = Cast<UNiagaraComponent>(AddComponentByClass(UNiagaraComponent::StaticClass(), false, FTransform::Identity, true));
	// if (SkillEffectComponent)
	// {
	// 	UNiagaraSystem* NiagaraSystem = skillDataRow.SkillEffect.LoadSynchronous();
	// 	SkillEffectComponent->SetAsset(NiagaraSystem);
	// 	if(bAttachedEffect) SkillEffectComponent->SetupAttachment(RootComponent); 
	// 	SkillEffectComponent->SetAutoActivate(true);
	// 	SkillEffectComponent->RegisterComponent();
	// 	SkillEffectComponent->Deactivate();
	// }
	// SkillOffEffect = skillDataRow.SkillOffEffect.Get();
	//
	// // 발사 속도 설정
	// ProjectileMovementComponent->InitialSpeed = skillDataRow.ProjectileSpeed;
	// SkillEffectComponent->SetVariableFloat(TEXT("Speed"),  ProjectileMovementComponent->InitialSpeed);
	//
	// // 발사 각 설정
	// LaunchAngle = skillDataRow.LaunchAngle;
	//
	// // 생성 위치 소켓 설정
	// SocketName = skillDataRow.SocketName;
	//
	// // 스킬 몽타주 설정
	// SkillAction = skillDataRow.SkillAction;
	//
	// // 스킬 쿨타임
	// CoolTime = skillDataRow.CoolTime;
	//
	// // CC 지속시간
	// CC_Duration = skillDataRow.CC_Duration;
	//
	// // 마나 소모량
	// MPConsumption = skillDataRow.MPConsumption;
	//
	// // 관통 횟수 - 몬스터 통과 제한 횟수
	// PenetrateCount = skillDataRow.PenetrateCount;
	//
	// // 최대 차징 계수
	// MaxCharging = skillDataRow.MaxCharging;
	//
	// // 스킬 슬롯이미지
	// SkillSlotImage = skillDataRow.SkillImage;
	//
	// // Drag On, Off
	// bDrag = skillDataRow.bDrag;
	// DragSpeed = ProjectileMovementComponent->InitialSpeed / Range;
	// SkillEffectComponent->SetVariableFloat(TEXT("Drag"),  DragSpeed);
	
	this->OwnerCharacter = OwnerCharacterPtr;
	this->OwnerController = OwnerControllerPtr;

	// SetActive(false);
}

// void ASkillActor::SetActive(bool Active)
// {
// 	// SetActorHiddenInGame(!Active); // SetHiddenInGame으로 처리하기기
// 	CollisionShape->SetActive(Active);
//
// 	if(Active) {SkillEffectComponent->Activate();}
// 	else {SkillEffectComponent->DeactivateImmediate();}	
//
// 	if(bSkilling && !Active)
// 	{
// 		// Skill Off Effect
// 		ProjectileMovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);
// 		bSkilling = false;
// 		auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SkillOffEffect, GetActorLocation(), GetActorRotation());
// 	}
// }	

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

// void ASkillActor::OnOverlap_Projectile(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
// {
// 	if(!bSkilling) return;
// 	if(!OwnerCharacter || CurrentPenetrateCount == 0) return;
// 	if(!OtherActor || OtherActor == this || OtherActor == OwnerCharacter) return;
// 	if(OtherActor->ActorHasTag("Player")) return;
// 	if(OverlappedComp->GetCollisionObjectType() == ECC_GameTraceChannel3) return;
// 	if(CheckSet.Contains(OtherActor)) return;
// 	
// 	float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, CalculateDamage(), SweepResult, true, CrowdControl, CC_Duration);
// 	// OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
//
// 	if(OtherActor->ActorHasTag("Enemy"))
// 	{
// 		CurrentPenetrateCount--;
// 		CheckSet.Emplace(OtherActor);
// 		if(CurrentPenetrateCount == 0) SetActive(false);
// 	}
// 	else 
// 	{
// 		SetActive(false);
// 	}
// }

float ASkillActor::CalculateDamage() const
{
    return OwnerCharacter->GetCurrentATK();
}

FActorSpawnParameters ASkillActor::InitSpawnParams()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter.Get();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    return SpawnParams;
}

void ASkillActor::UpdateSkillSlotImage()
{
	// TODO: 이런건 SkillController에서 처리하는게 좋을듯
	// if(OwnerCharacter->GetAPHUD())
	// {
	// 	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateSkillSlotImage(CurrentSkillKey, SkillSlotImage);
	// }
}