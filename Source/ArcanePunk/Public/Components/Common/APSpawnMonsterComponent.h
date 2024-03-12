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
	void SpawnParamsSetting();
	void PlaySpawnEffect(FVector Location);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnLocation(TSubclassOf<AAPSpawnPointBase> SpawnPointClass, int32 SpawnMonsterNum, TArray<AActor*>& SpawnLocations, USceneComponent* SpawnRoot = nullptr);
	void SpawnMonsterFromLocation(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass, TArray<AActor*>& SpawnLocations, TArray<AEnemy_CharacterBase*>& MonsterArr);
	void SpawnLocationRandom(TSubclassOf<AAPSpawnPointBase> SpawnPointClass, int32 SpawnMonsterNum, TArray<AActor*>& SpawnLocations, FVector MinimumRange, FVector MaximumRange);
	void SpawnMonsterRandomWithoutLocationActor(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass,  uint8 SpawnMonsterNum, FVector MinimumRange, FVector MaximumRange);
	
	FActorSpawnParameters SpawnParams;
};
