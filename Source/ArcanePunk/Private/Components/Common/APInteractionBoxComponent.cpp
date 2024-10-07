
#include "Components/Common/APInteractionBoxComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/WidgetComponent.h"
#include "UserInterface/Interaction/InteractionWidget.h"

UAPInteractionBoxComponent::UAPInteractionBoxComponent()
{
    InteractWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidgetComp"));
	InteractWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);


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
    auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;
    Character->OnBeginInteract.Clear();

    auto Interface = Cast<IInteractionInterface>(GetOwner());
    if(Interface)
    {
        if(!Interface->BeginFocus())
        Character->OnBeginInteract.AddDynamic(this, &UAPInteractionBoxComponent::BeginInteract);
    }

    if(bFirstInteract) InitInteractUI();
    if(InteractUI.IsValid()) InteractUI->OnInteractUI_FadeIn();    
}

void UAPInteractionBoxComponent::OnInteractionTriggerEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;

    Character->OnBeginInteract.RemoveDynamic(this, &UAPInteractionBoxComponent::BeginInteract);

    if(InteractUI.IsValid()) InteractUI->OnInteractUI_FadeOut();
}

void UAPInteractionBoxComponent::BeginInteract(AArcanePunkCharacter *PlayerCharacter)
{
    auto Interface = Cast<IInteractionInterface>(GetOwner());
    if(Interface) Interface->Interact(PlayerCharacter);
    if(InteractUI.IsValid()) InteractUI->OnInteractUI_FadeOut();   

    PlayerCharacter->OnEndInteract.Clear();
    PlayerCharacter->OnEndInteract.AddDynamic(this, &UAPInteractionBoxComponent::EndInteract);
}

void UAPInteractionBoxComponent::EndInteract(AArcanePunkCharacter *PlayerCharacter)
{
    PlayerCharacter->OnEndInteract.RemoveDynamic(this, &UAPInteractionBoxComponent::EndInteract);
    if(InteractUI.IsValid()) InteractUI->OnInteractUI_FadeIn();
}

void UAPInteractionBoxComponent::InitInteractUI()
{
    InteractWidgetComp->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    InteractUI = Cast<UInteractionWidget>(InteractWidgetComp->GetUserWidgetObject());
	if(InteractUI.IsValid()) 
    {
        InteractUI->UpdateWidget(InteractionData);
        InteractUI->SetVisibility(ESlateVisibility::Collapsed);
    }

    bFirstInteract = false;
}

