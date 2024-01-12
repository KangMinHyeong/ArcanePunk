
#include "SpawnPoint/APSpawnPointBase.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/SkillActor/APSkillType.h"

AAPSpawnPointBase::AAPSpawnPointBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BlockingArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BlockingArea"));
	SpawnPointEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpawnPointEffect"));
	PanelComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PanelComp"));
	SkillTypeComp = CreateDefaultSubobject<UAPSkillType>(TEXT("SkillTypeComp"));

	SetRootComponent(BlockingArea);
	SpawnPointEffect->SetupAttachment(BlockingArea);
	PanelComp->SetupAttachment(BlockingArea);
	
}

void AAPSpawnPointBase::SetAttackMouse(bool NewBool)
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());

	SetActorTickEnabled(NewBool);
	AttackMouse = NewBool;
}

void AAPSpawnPointBase::SetRangeLocation()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	float Range = OwnerCharacter->GetSkill3_LimitDist()/(BlockingArea->GetScaledCapsuleRadius());
	SetActorScale3D(FVector(Range, Range, 1.0f));
	
	RangeSpawnPoint = true;
}

void AAPSpawnPointBase::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultSize = GetActorScale3D().X;
	DefaultMaterial = PanelComp->GetMaterial(0);
	DefaultVector = GetActorLocation();
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, BlockingArea->GetScaledCapsuleHalfHeight()));
}

void AAPSpawnPointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(AttackMouse)
	{ 
		if(!OwnerCharacterPC) return;
		FHitResult HitResult;
		OwnerCharacterPC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		if(HitResult.bBlockingHit)
		{
			float Distance =  FVector::Distance(OwnerCharacter->GetActorLocation(), HitResult.Location);
			if(Distance <= OwnerCharacter->GetSkill3_LimitDist() * (GetActorScale3D().X / DefaultSize)) 
			{
				CanCast = true;
				PanelComp->SetMaterial(0,DefaultMaterial);
			}
			else
			{
				CanCast = false;
				PanelComp->SetMaterial(0,ProhibitMaterial);
			}
			SetActorLocation(FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z + BlockingArea->GetScaledCapsuleHalfHeight()));
		}
	}
	else if(RangeSpawnPoint)
	{
		SetActorScale3D(FVector(GetActorScale3D().X, GetActorScale3D().Y, 1.0f));
		SetActorLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, BlockingArea->GetScaledCapsuleHalfHeight()));
	}
}

void AAPSpawnPointBase::SetSkillType(uint8 SkillType)
{
	uint8 UpdateSkillType = SkillType;
	if(UpdateSkillType == 2) UpdateSkillType = 1;
	SkillTypeComp->SetSkillType(UpdateSkillType, bStun);
}