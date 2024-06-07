
#include "Skill/APSkillActorBase.h"

#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Engine/DamageEvents.h"
#include "Components/Character/APSkillHubComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameInstance/APGameInstance.h"

AAPSkillActorBase::AAPSkillActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// SkillTypeComp = CreateDefaultSubobject<UAPSkillType>(TEXT("SkillTypeComp"));
	SkillAbilityComponent = CreateDefaultSubobject<UAPSkillAbility>(TEXT("SkillAbilityComponent"));
	HitPointComp = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComp"));
}

void AAPSkillActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	// APGI = Cast<UAPGameInstance>(GetGameInstance()); 
	SetActorTickEnabled(false);
	DefaultSize = GetActorScale3D().Y; 
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAPSkillActorBase::DestroySKill, DestroyTime, false);
}

void AAPSkillActorBase::OnCharging()
{
    if(!OwnerCharacter.IsValid()) return;
    bCharging = true;
    SetActorTickEnabled(true);
    OwnerCharacter->GetAPHUD()->OnChargingEnd.Broadcast(true);
}

void AAPSkillActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPSkillActorBase::DestroySKill()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AAPSkillActorBase::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
	// SkillAbilityComp->SetSkillAbility(SkillAbility, SkillCategory);

    // SkillTypeComp->SetSkillType(SkillType, SkillCategory);
	
	SkillAbilityData = SkillAbilityNestingData;
    SkillComp = SkillComponent;
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;
	RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(SkillAbilityData.SkillName, SkillAbilityData.SkillName.ToString());
}

void AAPSkillActorBase::SetDeadTime(float DeadTime)
{
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AAPSkillActorBase::DeActivate(ESkillNumber SkillNumber)
{
	// auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
	// TWeakObjectPtr<USkillNumberBase> SkillNum = Character->GetAPSkillHubComponent()->GetSKillNumberComponent(SkillNumber);
	// if(SkillNum.IsValid())
	// {
	// 	SkillNum->SkillEnd();
	// } 
    if(!OwnerCharacter.IsValid() || !SkillComp.IsValid()) return;
    SkillComp->SkillEnd();

}

void AAPSkillActorBase::DeActivate_Ult()
{
	// auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
	// TWeakObjectPtr<USkillNumberBase> SkillNum = Character->GetRSkillNumber();
	// if(SkillNum.IsValid())
	// {
	// 	SkillNum->SkillEnd();
	// } 
    if(!OwnerCharacter.IsValid() || !SkillComp.IsValid()) return;
    SkillComp->SkillEnd();
}

void AAPSkillActorBase::HitDelay(AActor* DamagedActor, float Damage, uint8 HitNums, float DelayTime, bool bCriticalApply)
{
	if(!DamagedActor) return; if(!OwnerCharacter.IsValid()) return;
    FHitResult HitResult; 
    float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(DamagedActor, Damage, HitResult, bCriticalApply);
    OwnerCharacter->GetAttackComponent()->DrainCheck(DamagedActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());

    if(HitNums > 1)
    {
        HitNums--;
        FTimerHandle Timer;
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AAPSkillActorBase::HitDelay, DamagedActor, Damage, HitNums, DelayTime, bCriticalApply);
        GetWorld()->GetTimerManager().SetTimer(Timer, TimerDelegate, DelayTime, false);
    }
}

void AAPSkillActorBase::HomingOrderSet()
{
	HomingActors.Empty();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), HomingActors);

	if(HomingActors.IsEmpty()) {bHoming = false; return;}

	if(HomingActors.Num() >= 2)
	{
		HomingActors.Sort([this](AActor& A, AActor& B) 
		{
			return (A.GetActorLocation() - GetActorLocation()).Size() > (B.GetActorLocation() - GetActorLocation()).Size();
		});
	}

	bHoming = true;
}


void AAPSkillActorBase::CheckBuff(bool NewBool)
{
    if(BuffType == EBuffType::Speed) {OwnerCharacter->GetCrowdControlComp()->FastState(FastCoefficient, NewBool);}
    if(BuffType == EBuffType::ATKSpeed) {OwnerCharacter->GetBuffComp()->ATKSpeedUp(ATKSpeedCoefficient, NewBool);}
    if(BuffType == EBuffType::ATK) {OwnerCharacter->GetBuffComp()->ATKUp(ATKCoefficient, NewBool);}
}

bool AAPSkillActorBase::CheckSkillKey(USkillNumberBase* SkillNum)
{
    bool Check = false;
    if(SkillNum)
	{
        switch (SkillNum->GetSkillKey())
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

            case ESkillKey::None:
            Check = OwnerCharacter->GetOnRSkill();
            break;
        }
	} 
    return Check;
}

void AAPSkillActorBase::SetPlayerCollisionEnable(bool NewBool)
{
    if(NewBool) // 콜리전 원상태
    {
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        SetActorTickEnabled(false);
    }
    else // 콜리전 무시
    {
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
        OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SetActorTickEnabled(true);
    }
}

void AAPSkillActorBase::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	AbilityNum--;
	auto RowName = RowDataTable->SilverRowName;
    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;
    AbilityData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[AbilityNum]), RowName[AbilityNum]);
}

void AAPSkillActorBase::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	AbilityNum--;
    auto RowName = RowDataTable->GoldRowName;
    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;
    AbilityData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[AbilityNum]), RowName[AbilityNum]);
}

void AAPSkillActorBase::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	AbilityNum--;
    auto RowName = RowDataTable->PlatinumRowName;
    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;
    AbilityData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[AbilityNum]), RowName[AbilityNum]);
}