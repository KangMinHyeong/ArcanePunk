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
	void PlaySpawnEffect(const FVector & Location);
	FVector GetRandomLocation(const FVector & V_1, const FVector & V_2, const FVector & V_3) const;
	bool IsPossibleLocation(const FVector & Location);
	void SpawnParamsSetting();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnLocation(TSubclassOf<AAPSpawnPointBase> SpawnPointClass, int32 SpawnMonsterNum, TArray<AActor*>& SpawnLocations, USceneComponent* SpawnRoot = nullptr);
	void SpawnMonsterFromLocation(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass, TArray<AActor*>& SpawnLocations, TArray<AEnemy_CharacterBase*>& MonsterArr);
	void SpawnLocationRandom(TSubclassOf<AAPSpawnPointBase> SpawnPointClass, int32 SpawnMonsterNum, TArray<AActor*>& SpawnLocations, const FVector & MinimumRange, const FVector & MaximumRange);
	void SpawnMonsterRandomWithTriangle(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass,  uint8 SpawnMonsterNum, const FVector & V_1, const FVector & V_2, const FVector & V_3);
	TArray<AEnemy_CharacterBase*> SpawnMonsterRandomWithSquare(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass,  uint8 SpawnMonsterNum, const FVector & V_1, const FVector & V_2, const FVector & V_3, const FVector & V_4);
	TArray<AEnemy_CharacterBase*> SpawnMonsterRandomWithLocation(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass,  uint8 SpawnMonsterNum, const FVector & SpawnLocation);
	FActorSpawnParameters SpawnParams;

private:
	UPROPERTY(EditAnywhere)
	float Sink = 0.4f;

	FCollisionQueryParams Params;
};
