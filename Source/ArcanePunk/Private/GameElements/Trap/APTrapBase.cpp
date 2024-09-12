
#include "GameElements/Trap/APTrapBase.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Common/APDestructibleMeshComponent.h"
#include "AnimInstance/APTrapAnimInstance.h"
#include "Components/Common/APTransparentComponent.h"

AAPTrapBase::AAPTrapBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	RootMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RootMesh"));
	TrapCollision = CreateDefaultSubobject<USphereComponent>(TEXT("TrapCollision"));
	TopMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TopMesh"));
	DestructibleMesh = CreateDefaultSubobject<UAPDestructibleMeshComponent>(TEXT("DestructibleMesh"));
	FadeComp = CreateDefaultSubobject<UAPTransparentComponent>(TEXT("FadeComp"));

	SetRootComponent(RootMesh);
	TrapCollision->SetupAttachment(RootMesh);
	TopMesh->SetupAttachment(RootMesh);
	DestructibleMesh->SetupAttachment(RootMesh);
	DestructibleMesh->SetHiddenInGame(true);
}

void AAPTrapBase::BeginPlay()
{
	Super::BeginPlay();
	
	TrapCollision->OnComponentBeginOverlap.AddDynamic(this, &AAPTrapBase::OnOverlap);
	if(DestroyTime >= 1.0f)
	{
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAPTrapBase::AutoDestroy, DestroyTime, false);
		FadeComp->SetMeshMaterials(RootMesh);
	}

	auto Anim = Cast<UAPTrapAnimInstance>(RootMesh->GetAnimInstance());
	if(Anim) Anim->OnMontageEnded.AddDynamic(this, &AAPTrapBase::TrapMontageEnded);
}

void AAPTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAPTrapBase::TrapMontageEnded(UAnimMontage *Montage, bool bInterrupted)
{
	auto Anim = Cast<UAPTrapAnimInstance>(RootMesh->GetAnimInstance()); if(!Anim) return;
	if(Montage == Anim->TrapOperation_Montage) OnTrapOperation_MontageEnded();
}

void AAPTrapBase::OnTrapOperation_MontageEnded()
{
	FadeComp->FadeOut();
}

void AAPTrapBase::AutoRotating()
{
    FVector ToTarget = Player->GetActorLocation() - TopMesh->GetComponentLocation();
	FRotator TargetRotation = FRotator(0, ToTarget.Rotation().Yaw + RotatePlus, 0);

	TopMesh->SetWorldRotation(FMath::RInterpTo(TopMesh->GetComponentRotation(), TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), RotateSpeed));
}

void AAPTrapBase::AutoDestroy()
{
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	TrapCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AAPTrapBase::OnOverlap);
	RootMesh->SetHiddenInGame(true);
	DestructibleMesh->SetHiddenInGame(false);
	DestructibleMesh->Explode(DestructibleMesh->GetComponentLocation());

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestructibleMesh, &UAPDestructibleMeshComponent::StartFadeOut, 2.0f, false);
}

void AAPTrapBase::OnDamageTrigger()
{
	if(!Player.IsValid() || !bActivate) return;	

	UGameplayStatics::ApplyDamage(Player.Get(), TrapDamage, nullptr, this, UDamageType::StaticClass());
}

void AAPTrapBase::Deactivate()
{
	bActivate = false;
	TrapCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AAPTrapBase::OnOverlap);
}

void AAPTrapBase::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Player = Cast<AArcanePunkCharacter>(OtherActor); if(!Player.IsValid()) return;	

	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	TrapCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AAPTrapBase::OnOverlap);

	auto Anim = Cast<UAPTrapAnimInstance>(RootMesh->GetAnimInstance());
	if(Anim) OperationTime = Anim->PlayTrapOperation_Montage();

	bOperationEnd = true;
}
