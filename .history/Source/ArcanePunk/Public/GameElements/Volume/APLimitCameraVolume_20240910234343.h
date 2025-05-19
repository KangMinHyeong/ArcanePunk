// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APLimitCameraArea.generated.h"

class UBoxComponent;
class AArcanePunkCharacter;
class UAPFadeOutTriggerComponent;

UENUM(BlueprintType)
enum class ECurrentArea : uint8 // E스킬
{
	None 			 = 0 UMETA(DisplayName = "None"),
	Area_1			 = 1 UMETA(DisplayName = "제 1사분면"),
	Area_2			 = 2 UMETA(DisplayName = "제 2사분면"),
	Area_3 			 = 3 UMETA(DisplayName = "제 3사분면"),
	Area_4			 = 4 UMETA(DisplayName = "제 4사분면"),

};

UCLASS()
class ARCANEPUNK_API AAPLimitCameraArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPLimitCameraArea();

protected:
	virtual void BeginPlay() override;

	bool CheckLimit(float X, float Y);
	void CheckPlayerLocation(float X, float Y, bool first = false);
	void LineTrace(float & X, float & Y);
	void InitArea();

public:	
	virtual void Tick(float DeltaTime) override;

	void InitCondition();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
	

	FORCEINLINE void ResetInit() {X_Init = 0.0f; Y_Init = 0.0f;};
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* AreaTrigger;

private:
	float X_Init = 0.0f;
	float Y_Init = 0.0f;

	UPROPERTY(EditAnywhere)
	float InitSpeed = 200.0f;

	FVector FixedSpringArmLocation;

	TWeakObjectPtr<AArcanePunkCharacter> Player;

	float AreaAngle = 0.0f;
	float AreaDegree = 0.0f;
	float AreaWidth = 0.0f;
	float AreaHeight = 0.0f;
	FVector AreaCenter;

	FVector AreaPoint_1;
	FVector AreaPoint_2;
	FVector AreaPoint_3;
	FVector AreaPoint_4;

	FVector FinalPoint_1;
	FVector FinalPoint_2;

	ECurrentArea CurrentArea = ECurrentArea::None;

	bool bEnd = true;
};
