
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "APDronEnemyCharacter.generated.h"

UCLASS()
class ARCANEPUNK_API AAPDronEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAPDronEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditDefaultsOnly, Category = "Dron")
        int32 MaxDronCount = 20;

    UPROPERTY(EditDefaultsOnly, Category = "Dron")
        int32 DronCount = 0;

    UFUNCTION()
        void SpawnDron();


    FTimerHandle SpawnTimerHandle;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
