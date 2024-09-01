
#include "GameElements/Trap/Field/APLavaField.h"

#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	OnFieldDamage();
	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AAPLavaField::OnFieldDamage, DamageFrequency, true);
}

void AAPLavaField::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
}

void AAPLavaField::OnFieldDamage()
{
	TArray<AActor*> OverlappedActors;
	LavaTrigger->GetOverlappingActors(OverlappedActors);

	for(auto Actor : OverlappedActors)
	UGameplayStatics::ApplyDamage(Actor, FieldDamage, nullptr, this, UDamageType::StaticClass());
}