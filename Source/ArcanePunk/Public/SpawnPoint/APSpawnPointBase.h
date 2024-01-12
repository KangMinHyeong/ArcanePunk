// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APSpawnPointBase.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API AAPSpawnPointBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPSpawnPointBase();

	void SetAttackMouse(bool NewBool);
	void SetRangeLocation();

	FORCEINLINE bool GetCanCast() const {return CanCast;};
	FORCEINLINE bool GetbStun() const {return bStun;};

	void SetSkillType(uint8 SkillType);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UCapsuleComponent* GetBlockingArea() {return BlockingArea;}; // BlockingArea 반환

private:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* BlockingArea;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* SpawnPointEffect;

	bool AttackMouse = false;
	bool RangeSpawnPoint = false;

	FVector DefaultVector = FVector(0,0,0);

	UPROPERTY()
	class AArcanePunkCharacter* OwnerCharacter;

	UPROPERTY()
	class AArcanePunkPlayerController* OwnerCharacterPC;
	
	bool CanCast = false;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PanelComp;

	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* ProhibitMaterial;

	UPROPERTY()
	class UAPSkillType* SkillTypeComp;

	bool bStun = false;

public:
	UPROPERTY()
	float DefaultSize = 1.0f;
};
