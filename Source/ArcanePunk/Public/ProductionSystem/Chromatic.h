#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chromatic.generated.h"

UCLASS()
class ARCANEPUNK_API AChromatic : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditDefaultsOnly)
		class UMaterialInterface* YourDesiredMaterial;

	UPROPERTY(EditDefaultsOnly)
		class UMaterialInterface* OriginalMaterial;

public:	
	AChromatic();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
