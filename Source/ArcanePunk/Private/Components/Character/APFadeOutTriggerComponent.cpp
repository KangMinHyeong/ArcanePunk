
#include "Components/Character/APFadeOutTriggerComponent.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Components/Common/APTransparentComponent.h"

UAPFadeOutTriggerComponent::UAPFadeOutTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAPFadeOutTriggerComponent::BeginPlay()
{
    Super::BeginPlay();

    // SetComponentTickEnabled(false);
    this->OnComponentBeginOverlap.AddDynamic(this, &UAPFadeOutTriggerComponent::OnOverlap);
    this->OnComponentEndOverlap.AddDynamic(this, &UAPFadeOutTriggerComponent::OnOverlapEnd);
}

void UAPFadeOutTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Location = GetComponentLocation();
	Location = Location - GetOwner()->GetActorLocation(); 
    float Length = Location.Size();
    Location = Location/Length;  
    SetWorldRotation(FRotationMatrix::MakeFromX(Location).Rotator());
    SetBoxExtent(FVector(Length, 32.0f, 32.0f));
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

void UAPFadeOutTriggerComponent::FocusToPlayer()
{
    SetComponentTickEnabled(false);
}
