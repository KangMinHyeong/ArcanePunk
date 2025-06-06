
#include "GameElements/Trigger/APSwtichingCameraArea.h"

#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APCameraComponent.h"

AAPSwtichingCameraArea::AAPSwtichingCameraArea()
{
	PrimaryActorTick.bCanEverTick = false;

	AreaTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaTrigger"));
	
	SetRootComponent(AreaTrigger);
}

void AAPSwtichingCameraArea::BeginPlay()
{
	Super::BeginPlay();
	
	AreaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPSwtichingCameraArea::OnOverlap);
	AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPSwtichingCameraArea::OnOverlapEnd);
}

void AAPSwtichingCameraArea::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	SwitchingCamera(Character, true);
}

void AAPSwtichingCameraArea::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

	SwitchingCamera(Character, false);
}

void AAPSwtichingCameraArea::SwitchingCamera(AArcanePunkCharacter* Character, bool bBackView)
{
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