
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void LevelLoad(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void LeveUnlLoad(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* LoadTrigger;

	UPROPERTY(EditAnywhere)
	FName LoadLevelName; // 후에 데이터 테이블 만들어서 관리해도 좋을듯?

	UPROPERTY()
	bool IsLevelLoad = false;
};
