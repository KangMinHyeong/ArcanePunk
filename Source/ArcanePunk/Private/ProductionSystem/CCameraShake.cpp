#include "ArcanePunk/Public/ProductionSystem/CCameraShake.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"

ACCameraShake::ACCameraShake()
{
    PrimaryActorTick.bCanEverTick = true;
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));


    static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeClassFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/ProductionSystem/BP_Camera.BP_Camera_C'"));
    if (CameraShakeClassFinder.Succeeded())
    {
        CameraShakeClass = CameraShakeClassFinder.Class;
    }
}

void ACCameraShake::BeginPlay()
{
	Super::BeginPlay();
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACCameraShake::OnBoxComponentHit);
}

void ACCameraShake::OnBoxComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (CameraShakeClass)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (PlayerController)
        {
            PlayerController->ClientStartCameraShake(CameraShakeClass, 1.0f);
        }
    }
}
