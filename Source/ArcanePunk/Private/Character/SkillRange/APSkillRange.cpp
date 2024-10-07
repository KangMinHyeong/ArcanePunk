
#include "Character/SkillRange/APSkillRange.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

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
	RangeEffect->SetVariableFloat(TEXT("Size"),  Range / 325.0f);
}

void AAPSkillRange::SetScale_Arrow(float Width, float Length) 
{
	RangeEffect->SetVariableVec2(TEXT("Size2D"),  FVector2D(Length / 500.0f, Width / 100.0f));
}
