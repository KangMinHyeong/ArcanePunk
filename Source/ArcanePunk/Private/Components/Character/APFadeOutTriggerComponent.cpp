
#include "Components/Character/APFadeOutTriggerComponent.h"

#include "Kismet/KismetMaterialLibrary.h"

UAPFadeOutTriggerComponent::UAPFadeOutTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAPFadeOutTriggerComponent::BeginPlay()
{
    Super::BeginPlay();

    if(IsComponentTickEnabled()) {UE_LOG(LogTemp, Display, TEXT("TickComponent")); }
    else UE_LOG(LogTemp, Display, TEXT("No")); 
    PrimaryComponentTick.SetTickFunctionEnable(true);
    SetComponentTickEnabled(true);
    this->OnComponentBeginOverlap.AddDynamic(this, &UAPFadeOutTriggerComponent::OnOverlap);
    this->OnComponentEndOverlap.AddDynamic(this, &UAPFadeOutTriggerComponent::OnOverlapEnd);
}

void UAPFadeOutTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    

    for(auto Actor : FadeOutActors)
    {
        UE_LOG(LogTemp, Display, TEXT("Opacity : "));  
        auto Mesh = Actor->GetComponentByClass<UMeshComponent>();
        if(!Mesh) continue;

        int32 Index = 0; float Opacity = 0.0f;
        for(auto Mat : Mesh->GetMaterials())
        {
            auto CurrentMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mat);
            CurrentMat->GetScalarParameterValue(TEXT("Apperence"), Opacity);
            UE_LOG(LogTemp, Display, TEXT("Opacity : %f"), Opacity);  
            Opacity = FMath::FInterpConstantTo(Opacity, FadeOutLimit, DeltaTime, FadeSpeed);
            CurrentMat->SetScalarParameterValue(TEXT("Apperence"), Opacity);
            Mesh->SetMaterial(Index , CurrentMat); Index++;
        }    
          
    }

    for(auto Actor : FadeInActors)
    {
        auto Mesh = Actor->GetComponentByClass<UMeshComponent>();
        if(!Mesh) continue;

        int32 Index = 0; float Opacity = 0.0f;
        for(auto Mat : Mesh->GetMaterials())
        {
            auto CurrentMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mat);
            CurrentMat->GetScalarParameterValue(TEXT("Apperence"), Opacity);
            Opacity = FMath::FInterpConstantTo(Opacity, 1.0f, DeltaTime, FadeSpeed);
            CurrentMat->SetScalarParameterValue(TEXT("Apperence"), Opacity);
            Mesh->SetMaterial(Index , CurrentMat); Index++;
        }  

        if(Opacity >= 1.0f) {FadeInActors.Remove(Actor);}
    }
}

void UAPFadeOutTriggerComponent::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	FadeOutActors.Emplace(OtherActor);

    
    auto Mesh = OtherActor->GetComponentByClass<UMeshComponent>();
    if(!Mesh) return;

    int32 Index = 0; float Opacity = 0.0f;
    for(auto Mat : Mesh->GetMaterials())
    {
        auto CurrentMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mat);
        // CurrentMat->GetScalarParameterValue(TEXT("Apperence"), Opacity);
        UE_LOG(LogTemp, Display, TEXT("Opacity : %f"), Opacity);  
        // Opacity = FMath::FInterpConstantTo(Opacity, FadeOutLimit, DeltaTime, FadeSpeed);
        CurrentMat->SetScalarParameterValue(TEXT("Apperence"), Opacity);
        Mesh->SetMaterial(Index , CurrentMat); Index++;
    } 
}

void UAPFadeOutTriggerComponent::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    FadeOutActors.Remove(OtherActor);
    FadeInActors.Emplace(OtherActor);
}
