#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCameraShake.generated.h"

UCLASS()
class ARCANEPUNK_API ACCameraShake : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
		TSubclassOf<UCameraShakeBase> CameraShakeClass;

public:	
	ACCameraShake();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnBoxComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
