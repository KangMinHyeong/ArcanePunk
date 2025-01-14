
#include "Character/SkillRange/APSkillRange.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerController/ArcanePunkPlayerController.h"

AAPSkillRange::AAPSkillRange()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RangeDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("RangeDecal"));
	RangeEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RangeEffect"));

	SetRootComponent(RootComp);
	RangeDecal->SetupAttachment(RootComp);
	RangeEffect->SetupAttachment(RootComp);
}

void AAPSkillRange::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
	DefaultDecal = RangeDecal->GetDecalMaterial();
	DefaultSize = GetActorScale3D().Z;
}

void AAPSkillRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

}

void AAPSkillRange::SetActive(bool bActive)
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    OwnerPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerPC.IsValid()) return;
	
	SetActorHiddenInGame(!bActive);
	Targeting = bActive;
    SetActorTickEnabled(bActive);
}

void AAPSkillRange::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
	// if(SkillAbility.Contains(ESkillAbility::Gigant)) SetActorScale3D(GetActorScale3D() * 2.0f);
	// if(SkillAbility.Contains(ESkillAbility::Homing)) SetHoming();
	// if(SkillAbility.Contains(ESkillAbility::Stun)) SetStun();
}

void AAPSkillRange::SetAngle(float AngleSize) // 0~1 사이값
{
	float Angle =  90.000f - AngleSize * 180.000f; 
	RangeDecal->CreateDynamicMaterialInstance()->SetScalarParameterValue(TEXT("Percent"), AngleSize);
	RangeDecal->SetRelativeRotation(RangeDecal->GetRelativeRotation() + FRotator(0.0f, 0.0f, Angle)); // 
}

void AAPSkillRange::SetScale(float Range) 
{
	RangeEffect->DeactivateImmediate();
	RangeEffect->SetVariableFloat(TEXT("Size2D"),  Range / 325.0f);
	RangeEffect->Activate();

	RangeDecal->DecalSize = FVector(5.0f, Range, Range);
}

void AAPSkillRange::SetScale_Arrow(float Width, float Length) 
{
	Wide = Width;
	RangeEffect->DeactivateImmediate();
	RangeEffect->SetVariableVec2(TEXT("Size2D"),  FVector2D(Length / 500.0f, Width / 100.0f));
	RangeEffect->Activate();
}

void AAPSkillRange::SetSkillRangeType(ESkillRangeType NewValue)
{
	SkillRangeType = NewValue;

	MaxDistance = MaxDistance * (GetActorScale3D().Z / DefaultSize);
}

void AAPSkillRange::SetScale_Target(ESkillType SkillType, float X, float Y, float Range)
{
	switch (SkillType)
	{
	case ESkillType::Projectile:
		SetScale_Arrow(X, Range);
		break;

	case ESkillType::Charging:
		SetScale_Arrow(X, Y);
		break;
	
	default:
		SetScale(X);
		break;
	}
}

void AAPSkillRange::SetLength(float Length)
{
	RangeEffect->DeactivateImmediate();
	RangeEffect->SetVariableVec2(TEXT("Size2D"),  FVector2D(Length / 500.0f, Wide / 100.0f));
	RangeEffect->Activate();
}