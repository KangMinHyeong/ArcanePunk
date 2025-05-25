// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSkillControllerData.h"
#include "FSkillActorData.h"
#include "Character/SkillDataTable/SkillDataTable.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "SkillActor.generated.h"

class AAPSkillRange;
class ASkillController;
class AArcanePunkCharacter;
class UNiagaraComponent;
class UProjectileMovementComponent;

UCLASS()
class ARCANEPUNK_API ASkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillActor();
	FORCEINLINE FName GetSkillName() const {return SkillId;};
	FORCEINLINE TSubclassOf<AAPSkillRange> GetSkillTargetRangeClass() const {return SkillTargetRangeClass;};
	FORCEINLINE TSubclassOf<AAPSkillRange> GetSkillGroundRangeClass() const {return SkillGroundRangeClass;};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	virtual void UseSkill();
	UFUNCTION()
	virtual void InitSkill(FName SkillName, TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter, TWeakObjectPtr<ASkillController> OwnerController);
	void PlaySkillAction();
	void UpdateSkillSlotImage();
		
private:
	void DrawDebugForShapeComponent() const;
	
	float CalculateDamage() const;

	FActorSpawnParameters InitSpawnParams();

private:
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<ASkillController> OwnerController;
	
	FName SkillId;
	FSkillControllerData SkillControllerData;
	FSkillActorData SkillActorData;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* SkillEffectComponent;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SkillOffEffect;
	UPROPERTY(EditAnywhere)
	UShapeComponent* CollisionShape;
	UPROPERTY()
	TSubclassOf<AAPSkillRange> SkillTargetRangeClass;
	UPROPERTY()
	TSubclassOf<AAPSkillRange> SkillGroundRangeClass;
	UPROPERTY()
	TSubclassOf<AActor> InstallActorClass;
	UPROPERTY()
	UTexture2D* SkillSlotImage;
	
	FTimerHandle DestroyTimerHandle;

public:
	float RangeCoefficient = 1.0f; // Test용
};
