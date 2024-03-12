
#include "Components/Common/APInteractionBoxComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/InteractionInterface.h"

UAPInteractionBoxComponent::UAPInteractionBoxComponent()
{
    SetCollisionObjectType(ECC_GameTraceChannel4);
    SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap); 
    SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UAPInteractionBoxComponent::BeginPlay()
{
    Super::BeginPlay();
    this->OnComponentBeginOverlap.AddDynamic(this, &UAPInteractionBoxComponent::OnInteractionTrigger);
    this->OnComponentEndOverlap.AddDynamic(this, &UAPInteractionBoxComponent::OnInteractionTriggerEnd);
}

void UAPInteractionBoxComponent::OnInteractionTrigger(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character.IsValid()) return;
    
    auto Interface = Cast<IInteractionInterface>(GetOwner());
    if(Interface)
    {
        Interface->BeginFocus();
    }
}

void UAPInteractionBoxComponent::OnInteractionTriggerEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character.IsValid()) return;

    auto PC = Cast<AArcanePunkPlayerController>(Character->GetController()); if(!PC) return;
    PC->CloseInteraction(GetOwner());

    auto Interface = Cast<IInteractionInterface>(GetOwner());
    if(Interface)
    {
        Interface->EndFocus();
        Character->InteractionActorRemove(GetOwner());
    }
    
    Character = nullptr;
}
