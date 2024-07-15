// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Interfaces/SkillInterface.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "APSkillRange.generated.h"

class UMaterialInterface;
class AArcanePunkPlayerController;
class AArcanePunkCharacter;
class UDecalComponent;
class UNiagaraComponent;

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
	USceneComponent* GetRootComponent() const {return RootComp;};

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	void SetAngle(float AngleSize);
	void SetScale(float Range);
	void SetScale_Arrow(float Width, float Length);
	
	FORCEINLINE float GetMaxDist() const {return MaxDistance;};
	FORCEINLINE void SetMaxDist(float MaxDist) {MaxDistance = MaxDist;};
	FORCEINLINE float GetTargetDistance() const {return TargetDistance;};
	FORCEINLINE float GetTargetWide() {return RangeDecal->DecalSize.Y * (GetActorScale3D().Z / DefaultSize);};
	FORCEINLINE bool GetbStun() const {return bStun;};
	FORCEINLINE void SetWide(float NewValue) {Wide = NewValue;};
	FORCEINLINE void SetSkillRangeType(ESkillRangeType NewValue) {SkillRangeType = NewValue;};
	FORCEINLINE void SetMouseControll(bool NewBool) {Targeting = NewBool;};
	FORCEINLINE FVector GetRotVector() const {return RotVector;};
	// FORCEINLINE void SetHideAndTargeting(bool NewBool) {Targeting = !NewBool; }; // SetHideAndTargeting

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	UDecalComponent* RangeDecal;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* RangeEffect;

	float DefaultSize;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	TWeakObjectPtr<AArcanePunkPlayerController> OwnerPC;

	bool Targeting = false;

	float MaxDistance = 550.0f;

	bool bStun = false;

	float TargetDistance = 0.0f;

	float Wide = 0.0f;	

	ESkillRangeType SkillRangeType = ESkillRangeType::None;

	FVector RotVector;

public:
	TWeakObjectPtr<UMaterialInterface> DefaultDecal;

};
