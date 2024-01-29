// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/SkillInterface.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "APSkillRange.generated.h"

class UMaterialInterface;
class AArcanePunkPlayerController;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AAPSkillRange : public AActor, public ISkillInterface
{
	GENERATED_BODY()
	
public:	
	AAPSkillRange();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UDecalComponent* GetDecalComponent() {return RangeDecal;};
	
	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;


	FORCEINLINE float GetMaxDist() const {return MaxDistance;};
	FORCEINLINE void SetMaxDist(float MaxDist) {MaxDistance = MaxDist;};
	FORCEINLINE float GetTargetDistance() const {return TargetDistance;};
	FORCEINLINE float GetTargetWide() {return RangeDecal->DecalSize.Y * (GetActorScale3D().Z / DefaultSize);};
	FORCEINLINE bool GetbStun() const {return bStun;};

protected:
	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	class UDecalComponent* RangeDecal;

	UPROPERTY()
	float DefaultSize;

	UPROPERTY()
	AArcanePunkCharacter* OwnerCharacter;
	UPROPERTY()
	AArcanePunkPlayerController* OwnerPC;
	UPROPERTY()
	bool Targeting = false;
	UPROPERTY()
	float MaxDistance = 550.0f;
	UPROPERTY()
	bool bStun = false;
	UPROPERTY()
	float TargetDistance = 0.0f;

public:
	UPROPERTY()
	UMaterialInterface* DefaultDecal;

};
