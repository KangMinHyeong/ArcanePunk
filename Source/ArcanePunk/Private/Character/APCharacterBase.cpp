
#include "Character/APCharacterBase.h"

#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APHitPointComponent.h"
#include "Components/Character/APAnimHubComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "GameInstance/APGameInstance.h"
#include "Kismet/GameplayStatics.h"

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

	auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;
    auto DataTable = GI->GetStatusData()->FindRow<FStatusData>(CharacterName, CharacterName.ToString()); 
    if(DataTable) TotalStatus_Origin.StatusData = * DataTable;  
}

void AAPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	CrowdControlComp->BindCrowdComp();
	MoveComponent->SetBind();
	DefaultMaterial = GetMesh()->GetMaterials();
	DefaultSlip = GetCharacterMovement()->BrakingFrictionFactor;
}

void AAPCharacterBase::ResetDefaultMaterial()
{
	GetWorldTimerManager().ClearTimer(HitMaterialTimerHandle);
	
	int32 Index = 0;
	for(auto Mat : DefaultMaterial)
	{GetMesh()->SetMaterial(Index, Mat); Index++;}
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

	bHitting = true;

	if(DamageApplied > KINDA_SMALL_NUMBER)
	{
		int32 Index = 0;
		for(auto Mat : DefaultMaterial)
		{GetMesh()->SetMaterial(Index, HitMaterial); Index++;}

		if(IsDead()) {SpawnVoiceSound(DeadVoiceSound);}
		else {SpawnVoiceSound(HitVoiceSound);}
	}

	GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AAPCharacterBase::OnHittingEnd, HitMotionTime, false);
	GetWorldTimerManager().SetTimer(HitMaterialTimerHandle, this, &AAPCharacterBase::ResetDefaultMaterial, HitMaterailTime, false);

    return DamageAmount;
}

void AAPCharacterBase::OnHittingEnd()
{
	bHitting = false;
	GetCharacterMovement()->BrakingFrictionFactor = DefaultSlip;
	
	GetWorldTimerManager().ClearTimer(HitTimerHandle);
}

void AAPCharacterBase::SpawnVoiceSound(USoundBase *VoiceSound)
{
	if(!VoiceSound) return;

	auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;
    float Volume = 5.0f; Volume *= GI->GetGameSoundVolume().EffectVolume;
    
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), VoiceSound, GetActorLocation(), GetActorRotation(), Volume);
}

void AAPCharacterBase::SpawnAttackVoiceSound()
{
	if(!AttackVoiceSound) return;

	auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;
    float Volume = 5.0f; Volume *= GI->GetGameSoundVolume().EffectVolume;
    
	UGameplayStatics::SpawnSoundAttached(AttackVoiceSound, GetMesh(), TEXT("AttackVoiceSound"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false, Volume);
}

void AAPCharacterBase::SpawnAttackSound()
{
	if(!AttackVoiceSound) return;

	auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;
    float Volume = 5.0f; Volume *= GI->GetGameSoundVolume().EffectVolume;
    
	UGameplayStatics::SpawnSoundAttached(AttackSound, GetMesh(), TEXT("AttackVoiceSound"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false, Volume);
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
