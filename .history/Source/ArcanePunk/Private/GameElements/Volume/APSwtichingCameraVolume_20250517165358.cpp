
#include "GameElements/Trigger/APSwtichingCameraArea.h"

#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APCameraComponent.h"

AAPSwtichingCameraVolume::AAPSwtichingCameraVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	AreaTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaTrigger"));
	
	SetRootComponent(AreaTrigger);
}

void AAPSwtichingCameraVolume::BeginPlay()
{
	Super::BeginPlay();
	
	AreaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPSwtichingCameraVolume::OnOverlap);
	AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPSwtichingCameraVolume::OnOverlapEnd);
}

void AAPSwtichingCameraVolume::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	SwitchingCamera(Character, true);
}

void AAPSwtichingCameraVolume::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	SwitchingCamera(Character, false);
}

void AAPSwtichingCameraVolume::SwitchingCamera(AArcanePunkCharacter* Character, bool bBackView)
{
	auto PC = Cast<APlayerController>(Character->GetController());
	if(bBackView)
	{
		Character->GetAPCameraComponent()->Deactivate();
        Character->GetAPCameraComponent_Back()->Activate();
    }
    else
    {
        Character->GetAPCameraComponent()->Activate();
        Character->GetAPCameraComponent_Back()->Deactivate();
	}
}