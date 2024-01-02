#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "APDron.generated.h"

UCLASS()
class ARCANEPUNK_API AAPDron : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPDron();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Dron")
		float MovementSpeed;

	// Bandwidth parameter for Mean Shift
	UPROPERTY(EditAnywhere, Category = "Dron")
		float InfluenceRadius;

};
