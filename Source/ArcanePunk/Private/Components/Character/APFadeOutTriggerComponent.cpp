
#include "Components/Character/APFadeOutTriggerComponent.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Components/Common/APTransparentComponent.h"

UAPFadeOutTriggerComponent::UAPFadeOutTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPFadeOutTriggerComponent::BeginPlay()
{
    Super::BeginPlay();

    this->OnComponentBeginOverlap.AddDynamic(this, &UAPFadeOutTriggerComponent::OnOverlap);
    this->OnComponentEndOverlap.AddDynamic(this, &UAPFadeOutTriggerComponent::OnOverlapEnd);
}

void UAPFadeOutTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAPFadeOutTriggerComponent::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Mesh = OtherActor->GetComponentByClass<UAPTransparentComponent>();
    if(!Mesh) return;
    Mesh->FadeOut();
}

void UAPFadeOutTriggerComponent::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    auto Mesh = OtherActor->GetComponentByClass<UAPTransparentComponent>();
    if(!Mesh) return;
    Mesh->FadeIn();
}
