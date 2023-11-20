#include "ArcanePunk/Public/ProductionSystem/PointOfViewChange.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"

APointOfViewChange::APointOfViewChange()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));


}

void APointOfViewChange::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APointOfViewChange::OnBoxComponentHit);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APointOfViewChange::OnBoxComponentOut);


	if (ChangeViewCamera)
	{
		UCameraComponent* CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
		ChangeViewCamera->AddInstanceComponent(CameraComponent);
		CameraComponent->SetupAttachment(ChangeViewCamera->GetRootComponent());

		FVector CameraLocation = FVector(0.0f, 0.0f, 100.0f);
		FRotator CameraRotation = FRotator(-30.0f, 0.0f, 0.0f);
		CameraComponent->SetRelativeLocationAndRotation(CameraLocation, CameraRotation);
	}
}

void APointOfViewChange::OnBoxComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		OriginalCamera = PlayerController->GetViewTarget();
		PlayerController->SetViewTargetWithBlend(ChangeViewCamera, 5.0f);
	}
}

void APointOfViewChange::OnBoxComponentOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(OriginalCamera, 5.0f);
	}
}

