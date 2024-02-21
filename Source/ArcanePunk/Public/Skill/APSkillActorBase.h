
#pragma once

#include "CoreMinimal.h"
#include "Components/SkillActor/APSkillType.h"
#include "Components/SkillActor/APSkillAbility.h"
#include "Interfaces/SkillInterface.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APHitPointComponent.h"
#include "GameFramework/Actor.h"
#include "APSkillActorBase.generated.h"

UCLASS()
class ARCANEPUNK_API AAPSkillActorBase : public AActor, public ISkillInterface
{
	GENERATED_BODY()
	
public:	
	AAPSkillActorBase();

protected:
	virtual void BeginPlay() override;
	virtual void DestroySKill();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;

	FORCEINLINE void SetbStun(bool NewBool) {bStun = NewBool;};
	FORCEINLINE bool IsSkillTypeMaterial(ESkillTypeState SkillType) const {return SkillTypeMaterial.Contains(SkillType);};
	FORCEINLINE UMaterial* GetSkillTypeMaterial(ESkillTypeState SkillType) const {return SkillTypeMaterial[SkillType];};
	FORCEINLINE bool IsEffectColor(ESkillTypeState SkillType) const {return SkillTypeColor.Contains(SkillType);};
	FORCEINLINE FLinearColor GetEffectColor(ESkillTypeState SkillType) const {return SkillTypeColor[SkillType];};
	FORCEINLINE void SetEffectColor(FLinearColor NewColor) {EffectColor = NewColor;};

protected:
	FTimerHandle DestroyTimerHandle;
	
	UPROPERTY(EditAnywhere)
	float DestroyTime = 5.0f;

	UPROPERTY()
	UAPSkillType* SkillTypeComp;

	UPROPERTY()
	UAPSkillAbility* SkillAbilityComp;

	UPROPERTY()
	UAPHitPointComponent* HitPointComp;
	
	bool bStun = false;

	UPROPERTY(EditAnywhere)
	float StateTime = 3.0f;

	UPROPERTY(EditAnywhere)
	float DamageCoefficient = 1.0f;

	UPROPERTY()
	FLinearColor EffectColor;

	UPROPERTY(EditAnywhere)
	ESkillCategory SkillCategory = ESkillCategory::None;

	UPROPERTY(EditAnywhere)
	TMap<ESkillTypeState, UMaterial*> SkillTypeMaterial;

	UPROPERTY(EditAnywhere)
	TMap<ESkillTypeState, FLinearColor> SkillTypeColor;

public:
	UPROPERTY()
	float DefaultSize = 1.0f;
};
