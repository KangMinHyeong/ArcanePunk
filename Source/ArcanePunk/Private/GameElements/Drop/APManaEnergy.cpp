
#include "GameElements/Drop/APManaEnergy.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "UserInterface/HUD/APHUD.h"
#include "Components/Character/APSkillHubComponent.h"
#include "GameInstance/APGameInstance.h"

AAPManaEnergy::AAPManaEnergy()
{
 	PrimaryActorTick.bCanEverTick = true;

	EnergyTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("EnergyTrigger"));
	EnergyEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EnergyEffect"));

	SetRootComponent(EnergyTrigger);
	EnergyEffect->SetupAttachment(EnergyTrigger);
}

void AAPManaEnergy::BeginPlay()
{
	Super::BeginPlay();
	
	PlayEnergySound(true);
	SetStartImpulse();
	SetActorTickEnabled(false);
	EnergyTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPManaEnergy::OnOverlap);
}

void AAPManaEnergy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimeLine.TickTimeline(DeltaTime);

	if(bHoming)
	{
		TimeLine.TickTimeline(DeltaTime);
		StartTracePlayer(DeltaTime);
	}
}

void AAPManaEnergy::SetStartImpulse()
{
	SpawnImpulse.X = FMath::RandRange(-1.0f, 1.0f);
	if(SpawnImpulse.X > 0) {SpawnImpulse.X = FMath::RandRange(0.4f, 1.0f);}
	else {SpawnImpulse.X = FMath::RandRange(-0.4f, -1.0f);}

	SpawnImpulse.Y = FMath::RandRange(-1.0f, 1.0f);
	if(SpawnImpulse.Y > 0) {SpawnImpulse.Y = FMath::RandRange(0.4f, 1.0f);}
	else {SpawnImpulse.Y = FMath::RandRange(-0.4f, -1.0f);}

	SpawnImpulse.Z = FMath::RandRange(0.5f, 1.0f);
}

void AAPManaEnergy::StartTracePlayer(float DeltaTime)
{
	if(!TraceActor.IsValid()) return;
	CurrentHomingSpeed = FMath::FInterpConstantTo(CurrentHomingSpeed, HomingSpeed, DeltaTime, SpeedUp);

	FVector Current = GetActorLocation();
	Current = FMath::VInterpConstantTo(Current, TraceActor->GetActorLocation(), DeltaTime, CurrentHomingSpeed);
	SetActorLocation(Current);
}

void AAPManaEnergy::PlayEnergySound(bool Start)
{
	auto SoundGI = Cast<UAPSoundSubsystem>(GetGameInstance()->GetSubsystemBase(UAPSoundSubsystem::StaticClass())); if(!SoundGI) return;
    float Multiple = SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().EffectVolume;

	if(Start)
	{
		UGameplayStatics::SpawnSoundAttached(ManaSound_Start, GetRootComponent(), TEXT("ManaSound_Start"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false, ManaSoundVolume*Multiple);
	}
	else
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), ManaSound_Overlap, ManaSoundVolume*Multiple);
	}
}

void AAPManaEnergy::SetEnergyMoveComp(AActor* ManaOwner)
{
	TraceActor = ManaOwner;

	if(TimelineCurve)
	{
		TimeLineUpdateDelegate.BindUFunction(this, FName("TimeLineUpdateFunc"));
		TimeLine.AddInterpVector(TimelineCurve, TimeLineUpdateDelegate);

		TimeLine.SetTimelineLength(MaxLength);
		TimeLine.SetLooping(true);
	}
	TimeLine.PlayFromStart();

	if(SpawnTimeLineCurve)
	{
		SpawnTimeLineUpdateDelegate.BindUFunction(this, FName("SpawnTimeLineUpdateFunc"));
		SpawnTimeLine.AddInterpVector(SpawnTimeLineCurve, SpawnTimeLineUpdateDelegate);

		SpawnTimeLine.SetTimelineLength(MaxLength);
	}
	SpawnTimeLine.PlayFromStart();

	SetActorTickEnabled(true);
	StartHoming();
	// GetWorldTimerManager().SetTimer(TimerHandle, this, &AAPManaEnergy::StartHoming, MaxLength - 0.2f, false);
}

void AAPManaEnergy::TimeLineUpdateFunc(FVector & Output)
{
	Output.Y = Output.Y * FMath::RandRange(0.5f, 1.5f);
	SetActorLocation(GetActorLocation() + Output);
}

void AAPManaEnergy::SpawnTimeLineUpdateFunc(FVector & Output)
{
	Output.X *= SpawnImpulse.X;
	Output.Y *= SpawnImpulse.Y;
	Output.Z *= SpawnImpulse.Z;
	SetActorLocation(GetActorLocation() + Output);
}

void AAPManaEnergy::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EnergyOverlapEffect, Character->GetActorLocation(), Character->GetActorRotation());
	Destroy();

	Character->GetAPSkillHubComponent()->RecoveryMP();
		
	PlayEnergySound(false);
	// Mana 회복
}
