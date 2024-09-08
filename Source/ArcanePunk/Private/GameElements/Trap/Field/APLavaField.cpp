
#include "GameElements/Trap/Field/APLavaField.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"

AAPLavaField::AAPLavaField()
{
	PrimaryActorTick.bCanEverTick = false;

    LavaTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LavaTrigger"));
    SetRootComponent(LavaTrigger);

	LavaEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LavaEffectComp"));
	LavaEffectComp->SetupAttachment(LavaTrigger);
	LavaEffectComp->DeactivateImmediate();
}

void AAPLavaField::BeginPlay()
{
	Super::BeginPlay();
	
	LavaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPLavaField::OnOverlap);
	LavaTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPLavaField::OnOverlapEnd);

	LavaEffectComp->SetVariableFloat(TEXT("Size"), LavaTrigger->GetScaledBoxExtent().X / 200.0f);
	LavaEffectComp->Activate();
}

void AAPLavaField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPLavaField::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(bOverlap) return;
	auto Player = Cast<AArcanePunkCharacter>(OtherActor); if(!Player) return;
	bOverlap = true;
	
	float FirstDelay = 0.0001f;
	TArray<AActor*> Lavas;
	Player->GetCapsuleComponent()->GetOverlappingActors(Lavas, AAPLavaField::StaticClass());
	
	float time = DamageFrequency;
	for(auto Lava : Lavas) 
	{
		auto Lava_cur = Cast<AAPLavaField>(Lava);
		if(Lava_cur != this)
		{
			time = FMath::Min(time, Lava_cur->ClearTrap());
			FirstDelay = time;
		}		
	}

	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AAPLavaField::OnFieldDamage, FirstDelay, false);
}

void AAPLavaField::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	auto Player = Cast<AArcanePunkCharacter>(OtherActor); if(!Player) return;
	bOverlap = false;
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
}

float AAPLavaField::ClearTrap()
{
	float Time = 0.00001f;
	if (!GetWorldTimerManager().IsTimerActive(DamageTimerHandle)) return Time;

	Time = (GetWorld()->GetTimerManager().GetTimerRate(DamageTimerHandle) - GetWorld()->GetTimerManager().GetTimerElapsed(DamageTimerHandle));
	Time = FMath::Min(DamageFrequency - Time, Time);
    GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	return Time;
}

void AAPLavaField::OnFieldDamage()
{
	TArray<AActor*> Players;
	LavaTrigger->GetOverlappingActors(Players, AArcanePunkCharacter::StaticClass());

	for(auto Player : Players)
	UGameplayStatics::ApplyDamage(Player, FieldDamage, nullptr, this, UDamageType::StaticClass());
	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AAPLavaField::OnFieldDamage, DamageFrequency, false);
}