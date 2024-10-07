
#include "Skill/ArcaneAlterEgo.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Engine/DamageEvents.h"
#include "Components/Character/APSkillHubComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APAnimHubComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "UserInterface/Common/WidgetComponent/APHPWidgetComponent.h"
#include "GameInstance/APGameInstance.h"

AArcaneAlterEgo::AArcaneAlterEgo()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComp"));
	HealthWidgetComp->SetupAttachment(GetCapsuleComponent());
}

void AArcaneAlterEgo::BeginPlay()
{
	Super::BeginPlay();

	MoveComponent->SetBind();
	AttackComponent->InitAttackComp();
	AnimHubComponent->BindAttackCheck();
	AnimHubComponent->BindComboCheck();
}

void AArcaneAlterEgo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArcaneAlterEgo::MoveToMouseClick(FVector ClickPoint)
{
	if(!bCanMove) return;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), ClickPoint);
}

// void AArcaneAlterEgo::CopyAttack(uint8 ComboStack)
// {
// 	if(bDoing) return;
// 	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;

// 	if(ComboStack == 1) {OwnerAnim->PlayCombo_Montage();}
// 	else {OwnerAnim->JumpToComboSection(ComboStack);}
// 	bCanMove = false;
// 	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), GetActorLocation());
// }

float AArcaneAlterEgo::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(IsDead()) return 0.0f;
	float HP = TotalStatus.StatusData.HP;
	DamageApplied = FMath::Min(HP, DamageApplied);
	
	HP = HP - DamageApplied * Defense_constant * (1/(Defense_constant + TotalStatus.StatusData.DEF));
	TotalStatus.StatusData.HP = HP;
	UE_LOG(LogTemp, Display, TEXT("Ego HP : %f"), HP);
	OnEgoHPChanged.Broadcast();
	//GetWorldTimerManager().SetTimer(HitTimerHandle, this, &ABossMonster_Stage1::CanBeDamagedInit, bGodModeTime, false);

	if(IsDead())
	{
	// 	UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StunEffectComp->DeactivateImmediate();

		StunExplosion();
	}
	else
	{
		bHitting = true;
		bDoing = true;
		GetWorld()->GetTimerManager().SetTimer(HittingTimerHandle, this, &AArcaneAlterEgo::EndHitting, HitMotionTime, false);
	}

	
	if(HPUI.IsValid()) HPUI->UpdatingMaxHPAndHP(TotalStatus.StatusData.MaxHP, TotalStatus.StatusData.HP);

	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamagedEffect, GetActorLocation() + GetActorUpVector() * 20.0f, GetActorRotation(), DamagedScale);

    return DamageApplied;
}

void AArcaneAlterEgo::EndHitting()
{
	GetWorld()->GetTimerManager().ClearTimer(HittingTimerHandle);
	bDoing = false;
	bHitting = false;
}

void AArcaneAlterEgo::StunExplosion()
{
	if(!OwnerCharacter.IsValid()) return;

	OwnerCharacter->GetRSkillNumber()->SkillEnd();
	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), ExplosionScale);
	OwnerCharacter->GetAttackComponent()->MultiAttack(GetActorLocation(), GetActorLocation() - FVector(0,0,-20.0f), ExplosionRadius, ExplosionCoefficient, 1, true, StunTime );
	Destroy();
}

void AArcaneAlterEgo::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
	SkillAbilityData = SkillAbilityNestingData;
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	
	auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;
	RowDataTable = DataTableGI->GetSkillAbilityRowDataTable()->FindRow<FSkillAbilityRowNameData>(SkillAbilityData.SkillName, SkillAbilityData.SkillName.ToString());

	OwnerCharacter->OnAltRightMouseClick.AddDynamic(this, &AArcaneAlterEgo::MoveToMouseClick);
	// OwnerCharacter->OnComboAttackStart.AddDynamic(this, &AArcaneAlterEgo::CopyAttack);

	TotalStatus = OwnerCharacter->GetPlayerStatus(); // 후에 조정

	HPUI = Cast<UAPHPWidgetComponent>(HealthWidgetComp->GetUserWidgetObject());
	if(HPUI.IsValid()) HPUI->InitMaxHPAndHP(TotalStatus.StatusData.MaxHP, TotalStatus.StatusData.HP);
	
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneAlterEgo::StunExplosion, DestroyTime, false);
}
