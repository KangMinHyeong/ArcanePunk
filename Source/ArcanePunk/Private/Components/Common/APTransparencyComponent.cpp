
#include "Components/Common/APTransparencyComponent.h"

AAPTransparencyComponent::AAPTransparencyComponent()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAPTransparencyComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AAPTransparencyComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bFadeOut)
	{

	}
	else
	{
		// SetComponentTickEnabled(false);
	}
}

void AAPTransparencyComponent::FadeOut()
{
	bFadeOut = true;
	// SetComponentTickEnabled(true);
}

void AAPTransparencyComponent::FadeIn()
{
	bFadeOut = false;
}
