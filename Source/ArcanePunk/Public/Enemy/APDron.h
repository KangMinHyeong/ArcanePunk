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

    int32 GroupID;
    bool IsClustered;

protected:
    virtual void BeginPlay() override;

private:
    void SetGroupID();
    FVector CalculateCohesion();
    FVector CalculateSeparation();
    FVector SetDronDirection();

    UPROPERTY(EditAnywhere)
        float NeighborRadius;

    UPROPERTY(EditAnywhere)
        float AlignmentWeight;

    UPROPERTY(EditAnywhere)
        float CohesionWeight;

    UPROPERTY(EditAnywhere)
        float SeparationWeight;

    UPROPERTY(EditAnywhere)
        float MoveSpeed;

    UPROPERTY(EditAnywhere)
        float SeparationScale;

    FVector InitialLocation;

public:
    virtual void Tick(float DeltaTime) override;
};
