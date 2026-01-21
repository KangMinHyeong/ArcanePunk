#include "Character/APCharacterBase.h"

#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APHitPointComponent.h"
#include "Components/Character/APAnimHubComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "GameInstance/APGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMaterialLibrary.h"

AAPCharacterBase::AAPCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CrowdControlComp = CreateDefaultSubobject<UAPCrowdControlComponent>(TEXT("CrowdControlComp"));
	MoveComponent = CreateDefaultSubobject<UAPMovementComponent>(TEXT("MoveComponent"));
	AttackComponent = CreateDefaultSubobject<UAPAttackComponent>(TEXT("AttackComponent"));
	HitPointComponent = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComponent"));
	AnimHubComponent = CreateDefaultSubobject<UAPAnimHubComponent>(TEXT("AnimHubComponent"));
	StunEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StunEffectComp"));

	StunEffectComp->SetupAttachment(GetMesh());
}

void AAPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AAPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return; 
    auto DataTable = DataTableGI->GetRowByStruct<FStatusData>(CharacterName, CharacterName.ToString()); 
    if(DataTable) TotalStatus_Origin.StatusData = * DataTable;  

	CrowdControlComp->BindCrowdComp();
	MoveComponent->SetBind();
	DefaultMaterial = GetMesh()->GetMaterials();
	DefaultSlip = GetCharacterMovement()->BrakingFrictionFactor;

	// Mesh Init
	int32 i = 0;
	for (auto Mat : GetMesh()->GetMaterials())
	{
		auto NewMesh = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mat);
		SkinMesh.Emplace(NewMesh); GetMesh()->SetMaterial(i, NewMesh);
		i++;
	}
}

void AAPCharacterBase::ResetDefaultMaterial()
{
	GetWorldTimerManager().ClearTimer(HitMaterialTimerHandle);
	
	int32 Index = 0;
	for(auto Mat : DefaultMaterial)
	{GetMesh()->SetMaterial(Index, Mat); Index++;}
}

void AAPCharacterBase::DissolveCharacterMesh(float DeltaTime)
{
	Apperence = FMath::FInterpConstantTo(Apperence, ApperenceTarget, DeltaTime, FadeOutSpeed);
	int32 index = 0;
	for (auto Mat : SkinMesh)
	{
		if(!Mat) continue;
		
		Mat->SetScalarParameterValue(TEXT("Apperence"), Apperence);
		GetMesh()->SetMaterial(index, Mat);
		index++;
	}
}

void AAPCharacterBase::DissolveCharacterMesh_Immediate(bool bFadeOut)
{
	int32 index = 0;

	if(bFadeOut)
	{
		ApperenceTarget = 0.0f;
		Apperence = 0.0f;
	}
	else
	{
		ApperenceTarget = 1.0f;
		Apperence = 1.0f;	
	}

	for (auto Mat : SkinMesh)
	{
		if(!Mat) continue;

		Mat->SetScalarParameterValue(TEXT("Apperence"), Apperence);
		GetMesh()->SetMaterial(index, Mat);
		index++;
	}
}



bool AAPCharacterBase::IsDead()
{
	bool bIsDead = false;
	if(TotalStatus.StatusData.HP<=KINDA_SMALL_NUMBER)
	{
		bIsDead = true;
		CrowdControlComp->PlayStateEffect(ECharacterState::Frozen, false);
	}
    return bIsDead;
}

void AAPCharacterBase::UpdateStatus()
{
	
}

void AAPCharacterBase::SetAttackRotation(float AddSpeed)
{

}

float AAPCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(bBlockMode) return 0.0f;

	if(DamageApplied > KINDA_SMALL_NUMBER)
	{
		bHitting = true;

		int32 Index = 0;
		for(auto Mat : DefaultMaterial)
		{GetMesh()->SetMaterial(Index, HitMaterial); Index++;}

		if(IsDead()) {SpawnVoiceSound(DeadVoiceSound);}
		else {SpawnVoiceSound(HitVoiceSound);}
		
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AAPCharacterBase::OnHittingEnd, HitMotionTime, false);
		GetWorldTimerManager().SetTimer(HitMaterialTimerHandle, this, &AAPCharacterBase::ResetDefaultMaterial, HitMaterailTime, false);
	}
	
    return DamageAmount;
}

void AAPCharacterBase::OnHittingEnd()
{
	bHitting = false;
	GetCharacterMovement()->BrakingFrictionFactor = DefaultSlip;
	
	GetWorldTimerManager().ClearTimer(HitTimerHandle);
}

float AAPCharacterBase::DamageMath(float Damage)
{
    return Damage * Defense_constant * (1/(Defense_constant + TotalStatus.StatusData.DEF));
}

void AAPCharacterBase::SpawnVoiceSound(USoundBase *VoiceSound)
{
	if(!VoiceSound) return;

	auto SoundGI = Cast<UAPSoundSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSoundSubsystem::StaticClass())); if(!SoundGI) return;
    float Volume = 5.0f; Volume *= SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().EffectVolume;
    
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), VoiceSound, GetActorLocation(), GetActorRotation(), Volume);
}

void AAPCharacterBase::SpawnAttackVoiceSound()
{
	if(!AttackVoiceSound) return;

	auto SoundGI = Cast<UAPSoundSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSoundSubsystem::StaticClass())); if(!SoundGI) return;
    float Volume = 5.0f; Volume *= SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().EffectVolume;
        
	UGameplayStatics::SpawnSoundAttached(AttackVoiceSound, GetMesh(), TEXT("AttackVoiceSound"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false, Volume);
}

void AAPCharacterBase::SpawnAttackSound(uint8 AttackNum)
{
	if(!AttackVoiceSound) return;

	auto SoundGI = Cast<UAPSoundSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSoundSubsystem::StaticClass())); if(!SoundGI) return;
    float Volume = 5.0f; Volume *= SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().EffectVolume;

	if(AttackNum > AttackSound.Num()) return;
	UGameplayStatics::SpawnSoundAttached(AttackSound[AttackNum-1], GetMesh(), TEXT("AttackSound"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false, Volume);
}

bool AAPCharacterBase::CheckShieldHP(float & DamageApplied, FDamageEvent const &DamageEvent) // if true, DamageApplied < ShieldHP
{
	float& ShieldHP = TotalStatus.StatusData.ShieldHP;
	float OriginShieldHP = ShieldHP;
	DamageApplied -= ShieldHP;

	bool bResult = false;
	
	ShieldHP = FMath::Max(-DamageApplied, 0.0f);
	DamageApplied = FMath::Max(DamageApplied, 0.0f);

	if(ShieldHP > KINDA_SMALL_NUMBER)
	{
		bResult = true;
		if(DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*) &DamageEvent;
			FVector HitLoc = PointDamageEvent->HitInfo.ImpactPoint - GetActorLocation(); HitLoc.Z = 0.0f;
			FRotator HitRot = FRotationMatrix::MakeFromX(HitLoc).Rotator();  HitRot.Yaw -= 90.0f;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShieldHitEffect, PointDamageEvent->HitInfo.ImpactPoint, HitRot);
		}
	}

	OnCheckingShield.Broadcast(this, OriginShieldHP);

    return bResult;	
}


float AAPCharacterBase::CriticalCalculate()
{
	float Percent = FMath::RandRange(0.0f, 100.0f);
	if(Percent <= TotalStatus.StatusData.CriticalPercent)
	{
		bCriticalAttack = true;
		return TotalStatus.StatusData.CriticalStep * (TotalStatus.StatusData.CriticalDamageCoefficient);
	}
	else {bCriticalAttack= false;}
    return 1.0f;
}

void AAPCharacterBase::SetHitPoint(float Forward, float Right)
{
	HitForward = Forward;
	HitRight = Right;
}

void AAPCharacterBase::SetHitEffect(FVector HitLocation)
{
	if(HitForward > 0)
	{
		if(HitRight > 0)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect_R, GetMesh(), TEXT("HitLocation"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		}
		else
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect_L, GetMesh(), TEXT("HitLocation"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		}
	}
	else
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect_B, GetMesh(), TEXT("HitLocation"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	}
}


