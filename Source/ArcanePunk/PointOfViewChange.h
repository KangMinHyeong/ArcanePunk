#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointOfViewChange.generated.h"

UCLASS()
class ARCANEPUNK_API APointOfViewChange : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
		class AActor* ChangeViewCamera;

	UPROPERTY(EditAnywhere)
		class AActor* OriginalCamera;

public:	
	APointOfViewChange();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnBoxComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnBoxComponentOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
