
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/LevelStreamingDynamic.h"
#include "APLevelStreamTrigger.generated.h"

class UBoxComponent;

UCLASS()
class ARCANEPUNK_API AAPLevelStreamTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPLevelStreamTrigger();

protected:
	virtual void BeginPlay() override;

#pragma region Set StreamingLevel
public:
	void SetStreamingLevel(ULevelStreamingDynamic* Level);
#pragma endregion

#pragma region Level Streaming Trigger Func
private:
	UFUNCTION()
	void LevelLoad(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void LeveUnlLoad(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Memory profiling - Get current memory usage in MB */
	float GetCurrentMemoryUsageMB() const;

	/** Log loading state for debugging */
	void LogLoadingState(const FString& Action) const;
#pragma endregion

private:

#pragma region BoxComponent
	UPROPERTY(EditAnywhere)
	UBoxComponent* LoadTriggerVolume;
#pragma endregion

#pragma region StreamingLevel
	TWeakObjectPtr<ULevelStreamingDynamic> StreamingLevel;
#pragma endregion

#pragma region Variable
	bool IsLevelLoaded = false;

	/** Memory before loading (for profiling) */
	UPROPERTY()
	float MemoryBeforeLoad = 0.0f;

	/** Priority for loading (higher = load first) */
	UPROPERTY(EditAnywhere, Category = "Streaming")
	int32 LoadingPriority = 0;
#pragma endregion
};
