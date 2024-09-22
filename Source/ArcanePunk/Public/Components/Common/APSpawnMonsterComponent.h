// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APSpawnMonsterComponent.generated.h"

class AAPSpawnPointBase;
class AEnemy_CharacterBase;
struct FActorSpawnParameters;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSpawnMonsterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSpawnMonsterComponent();

protected:
	virtual void BeginPlay() override;
	void PlaySpawnEffect(FVector Location);
	FVector GetRandomLocation(FVector V_1, FVector V_2, FVector V_3);
	bool IsPossibleLocation(FVector Location);
	void SpawnParamsSetting();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnLocation(TSubclassOf<AAPSpawnPointBase> SpawnPointClass, int32 SpawnMonsterNum, TArray<AActor*>& SpawnLocations, USceneComponent* SpawnRoot = nullptr);
	void SpawnMonsterFromLocation(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass, TArray<AActor*>& SpawnLocations, TArray<AEnemy_CharacterBase*>& MonsterArr);
	void SpawnLocationRandom(TSubclassOf<AAPSpawnPointBase> SpawnPointClass, int32 SpawnMonsterNum, TArray<AActor*>& SpawnLocations, FVector MinimumRange, FVector MaximumRange);
	void SpawnMonsterRandomWithTriangle(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass,  uint8 SpawnMonsterNum, FVector V_1, FVector V_2, FVector V_3);
	void SpawnMonsterRandomWithSquare(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass,  uint8 SpawnMonsterNum, FVector V_1, FVector V_2, FVector V_3, FVector V_4);
	void SpawnMonsterRandomWithLocation(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass,  uint8 SpawnMonsterNum, FVector SpawnLocation);
	FActorSpawnParameters SpawnParams;

private:
	UPROPERTY(EditAnywhere)
	float Sink = 0.4f;

	FCollisionQueryParams Params;
};
