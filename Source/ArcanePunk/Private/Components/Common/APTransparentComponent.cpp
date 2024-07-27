
#include "Components/Common/APTransparentComponent.h"

#include "Kismet/KismetMaterialLibrary.h"

UAPTransparentComponent::UAPTransparentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAPTransparentComponent::BeginPlay()
{
	Super::BeginPlay();

	auto Mesh = GetOwner()->GetComponentByClass<UMeshComponent>();
    if(!Mesh) return;
	int32 Index = 0; 
    for(auto Mat : Mesh->GetMaterials())
    {
        auto CurrentMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mat);
        Materials.Emplace(CurrentMat);
		Mesh->SetMaterial(Index , CurrentMat); Index++;
		CurrentMat->GetScalarParameterValue(TEXT("Apperence"), Opacity);
    } 
	
	SetComponentTickEnabled(false);
}

void UAPTransparentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bFadeOut)
	{
        for(auto Mat : Materials)
        {
            Opacity = FMath::FInterpConstantTo(Opacity, FadeOutLimit, DeltaTime, FadeSpeed);
            Mat->SetScalarParameterValue(TEXT("Apperence"), Opacity);
        } 
	}
	else
	{
        for(auto Mat : Materials)
        {
            Opacity = FMath::FInterpConstantTo(Opacity, 1.00f, DeltaTime, FadeSpeed);
            Mat->SetScalarParameterValue(TEXT("Apperence"), Opacity);
        }
		if(Opacity >= 1.0f) SetComponentTickEnabled(false);
	}
}

void UAPTransparentComponent::FadeOut()
{
	bFadeOut = true;
	SetComponentTickEnabled(true);
}

void UAPTransparentComponent::FadeIn()
{
	bFadeOut = false;
}


