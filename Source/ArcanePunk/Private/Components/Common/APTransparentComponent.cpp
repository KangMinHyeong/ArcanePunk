
#include "Components/Common/APTransparentComponent.h"

#include "Kismet/KismetMaterialLibrary.h"

UAPTransparentComponent::UAPTransparentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAPTransparentComponent::BeginPlay()
{
	Super::BeginPlay();

	if(bAuto)
	{
		auto Mesh = GetOwner()->GetComponentByClass<UMeshComponent>();
		SetMeshMaterials(Mesh);
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
		if(Opacity <= FadeOutLimit && bDestroy) GetOwner()->Destroy();
	}
	else
	{
        for(auto Mat : Materials)
        {
            Opacity = FMath::FInterpConstantTo(Opacity, 1.0f, DeltaTime, FadeSpeed);
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

void UAPTransparentComponent::SetMeshMaterials(UMeshComponent *Meshes)
{
	if(!Meshes) return;
	Materials.Empty();
	
	int32 Index = 0; 
	for(auto Mat : Meshes->GetMaterials())
	{
		auto CurrentMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Mat);
		Materials.Emplace(CurrentMat);
		Meshes->SetMaterial(Index , CurrentMat); Index++;
		CurrentMat->GetScalarParameterValue(TEXT("Apperence"), Opacity);
	} 
}
