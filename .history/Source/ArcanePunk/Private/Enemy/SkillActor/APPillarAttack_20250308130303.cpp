
#include "Enemy/SkillActor/APPillarAttack.h"

#include "Enemy/Enemy_BossBase.h"
#include "GameElements/DamageText/DamageText.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/SkeletalMeshComponent.h"

AAPPillarAttack::AAPPillarAttack()
{
	PrimaryActorTick.bCanEverTick = true;

	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	ThrownTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ThrownTrigger"));
	PillarSkin = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PillarSkin"));

	SetRootComponent(PillarMesh);
	ThrownTrigger->SetupAttachment(PillarMesh);
	PillarSkin->SetupAttachment(PillarMesh);
}

void AAPPillarAttack::BeginPlay()
{
	Super::BeginPlay();
	
	OriginHP = PillarHP;
	InitPillar(false);
	PillarMesh->OnComponentBeginOverlap.AddDynamic(this, &AAPPillarAttack::OnOverlaping);

	SetPillarMaterial();
}

void AAPPillarAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bHidden)
	{
		Opacity = FMath::FInterpConstantTo(Opacity, 0.0f, DeltaTime, FadeOutSpeed);
	}
	else
	{
		Opacity = FMath::FInterpConstantTo(Opacity, 1.0f, DeltaTime, FadeOutSpeed);
	}

	for(auto Mat : Materials)
	{
		Mat->SetScalarParameterValue(TEXT("Opacity"), Opacity);
	}
}

void AAPPillarAttack::InitPillar(bool bEnd)
{
	bHidden = bEnd;
	// 
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);

	if(!bEnd)
	{
		PillarMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetWorld()->GetTimerManager().ClearTimer(AppearTimerHandle);
		PillarHP = OriginHP;
	}
	else
	{
		PillarMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AAPPillarAttack::InitPillar, !bEnd );
		GetWorld()->GetTimerManager().SetTimer(AppearTimerHandle, TimerDelegate, AppearTime, false);
	}
}

void AAPPillarAttack::SetPillarMaterial()
{
	TArray<UMaterialInterface*> Mats = PillarMesh->GetMaterials();
	for (int i = 0; i < Mats.Num(); i++)
	{
		Materials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mats[i]));
		PillarMesh->SetMaterial(i, Materials[i]);
		PillarSkin->SetMaterial(i, Materials[i]);
	}
	SetActorTickEnabled(false);
	SkinChange(false);
}

void AAPPillarAttack::OnOverlaping(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || bThrowed) return;
	auto Boss = Cast<AEnemy_BossBase>(OtherActor); if(!Boss) return;

	if(!Boss->IsPillarAttack()) return;
	Boss->OnPillarWeapon();
	InitPillar(true);
	SetActorHiddenInGame(true);
	// AttachToComponent(Boss->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bip001-Prop1"));
}

void AAPPillarAttack::OnHitting(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!OtherActor || OtherActor == this ) return;
	auto Boss = Cast<AEnemy_BossBase>(GetOwner()); if(!Boss) return;

	auto Player = Cast<AArcanePunkCharacter>(OtherActor); if(!Player) return;

	Boss->DistinctHitPoint(Player->GetActorLocation(), Player);
	UGameplayStatics::ApplyDamage(Player, Boss->GetDefaultATK() * PillarCoefficient, GetInstigatorController(), this, UDamageType::StaticClass()); 
	ThrownTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
}

float AAPPillarAttack::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(PillarHP <= KINDA_SMALL_NUMBER || bThrowed) return 0.0f;
	DamageApplied = FMath::Min(PillarHP, DamageApplied);
	
	PillarHP = PillarHP - DamageApplied;

	SpawnDamageText(EventInstigator, DamageAmount, DamageTextAddLocation);

	if(PillarHP <= KINDA_SMALL_NUMBER)
	{
	// 	UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
		InitPillar(true);
	}
	else
	{
		SkinChange(true);
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AAPPillarAttack::HitEnd, 0.8f, false);
	}

    return DamageApplied;
}

void AAPPillarAttack::SkinChange(bool Hitting)
{
	PillarMesh->SetHiddenInGame(Hitting);
	PillarSkin->SetHiddenInGame(!Hitting);
	bHitting = Hitting;
}

void AAPPillarAttack::ThrowedPillar(FVector & Target)
{
	bThrowed = true;
	// auto Impulse = (Target - GetActorLocation()) * ThrowedSpeed;
	Target.Z = UpCoeff;
	PillarMesh->SetSimulatePhysics(true);
	PillarMesh->AddImpulse(Target * ThrowedSpeed, TEXT("Impulse"), true);

	ThrownTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	ThrownTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPPillarAttack::OnHitting);

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AAPPillarAttack::DestroyPillar, DestroyTime, false);
}

void AAPPillarAttack::SpawnDamageText(AController* EventInstigator, float Damage, FVector AddLocation)
{
	ADamageText* DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass, GetActorLocation() + AddLocation, FRotator(0.0f, 180.0f, 0.0f));
	if(!DamageText) return;

	bool Check = false; 
	auto Character = Cast<AArcanePunkCharacter>(EventInstigator->GetPawn()); 
	if(Character) Check = Character->IsCriticalAttack();

	DamageText->SetOwner(this);
	DamageText->SetDamageText(Damage, Check);
}

void AAPPillarAttack::DestroyPillar()
{
	Destroy();
}
