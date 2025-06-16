
#include "GameElements/Trigger/APSwtichingCameraArea.h"

AAPSwtichingCameraArea::AAPSwtichingCameraArea()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AAPSwtichingCameraArea::BeginPlay()
{
	Super::BeginPlay();
	
	AreaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPSwtichingCameraArea::OnOverlap);
	AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPSwtichingCameraArea::OnOverlapEnd);
}

void AAPSwtichingCameraArea::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	
}

void AAPLimitCameraArea::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{

}



