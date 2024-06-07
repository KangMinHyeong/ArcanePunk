
#pragma once

#include "CoreMinimal.h"
#include "GameInstance/APGameInstance.h"
#include "Components/SkillActor/APSkillType.h"
#include "Components/SkillActor/APSkillAbility.h"
#include "Interfaces/SkillInterface.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APHitPointComponent.h"
#include "GameFramework/Actor.h"
#include "APSkillActorBase.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	None		 = 0,
    Speed		 = 1,
    ATKSpeed     = 2,
	ATK			 = 3,
	Damage		 = 4,
};

UENUM(BlueprintType)
enum class EPenetrateType : uint8 // 관통 종류
{
	None 	= 0 UMETA(DisplayName = "None"),
	Enemy 	= 1 UMETA(DisplayName = "Enemy 관통"),
	Object 	= 2 UMETA(DisplayName = "Object 관통"),
	Both 	= 3	UMETA(DisplayName = "모두 관통")
};

class AEnemy_CharacterBase;
class USkillNumberBase;

UCLASS()
class ARCANEPUNK_API AAPSkillActorBase : public AActor, public ISkillInterface
{
	GENERATED_BODY()
	
public:	
	AAPSkillActorBase();

protected:
	virtual void BeginPlay() override;
	virtual void DestroySKill();
		
	void DeActivate(ESkillNumber SkillNumber);
	void DeActivate_Ult();

	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum);
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum);
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum);
	
	void HitDelay(AActor* DamagedActor, float Damage, uint8 HitNums, float DelayTime, bool bCriticalApply);

	virtual void HomingOrderSet();

	void CheckBuff(bool NewBool);
	bool CheckSkillKey(USkillNumberBase* SkillNum);
	void SetPlayerCollisionEnable(bool NewBool);
	
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	virtual void OnCharging();

	FORCEINLINE void SetbStun(bool NewBool) {bStun = NewBool;};
	FORCEINLINE bool IsSkillTypeMaterial(ESkillTypeState SkillType) const {return SkillTypeMaterial.Contains(SkillType);};
	FORCEINLINE UMaterial* GetSkillTypeMaterial(ESkillTypeState SkillType) const {return SkillTypeMaterial[SkillType];};
	FORCEINLINE bool IsEffectColor(ESkillTypeState SkillType) const {return SkillTypeColor.Contains(SkillType);};
	FORCEINLINE FLinearColor GetEffectColor(ESkillTypeState SkillType) const {return SkillTypeColor[SkillType];};
	FORCEINLINE void SetEffectColor(FLinearColor NewColor) {EffectColor = NewColor;};
	FORCEINLINE float GetStateTime() const {return StateTime;};
	FORCEINLINE float GetDamage() const {return CurrentDamage;};
	FORCEINLINE bool IsSlow() const {return bSlow;};
	FORCEINLINE void SetbCharging(bool NewBool) {bCharging = NewBool;};
	virtual void SetDeadTime(float DeadTime);

protected:
	FTimerHandle DestroyTimerHandle;
	
	UPROPERTY(EditAnywhere)
	float DestroyTime = 5.0f;

	// UPROPERTY()
	// UAPSkillType* SkillTypeComp;

	UPROPERTY(EditAnywhere)
	UAPSkillAbility* SkillAbilityComponent;

	UPROPERTY()
	UAPHitPointComponent* HitPointComp;

	// UPROPERTY()
	// UAPGameInstance* APGI;
	UPROPERTY(EditAnywhere)
	int32 SlowPercent = 0;
	
	bool bSlow = false;
	bool bStun = false;
	bool bWeak = false;
	bool bHoming = false;
	bool bCharging = false;

	EBuffType BuffType = EBuffType::None;

	float WeakCoefficient = 0.0f;

	UPROPERTY(EditAnywhere)
	float StateTime = 3.0f;

	UPROPERTY(EditAnywhere)
	float DamageCoefficient = 1.0f;

	FLinearColor EffectColor;

	UPROPERTY(EditAnywhere)
	ESkillCategory SkillCategory = ESkillCategory::None;

	UPROPERTY(EditAnywhere)
	TMap<ESkillTypeState, UMaterial*> SkillTypeMaterial;

	UPROPERTY(EditAnywhere)
	TMap<ESkillTypeState, FLinearColor> SkillTypeColor;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;

	FSkillAbilityNestingData SkillAbilityData;

	FSkillAbilityRowNameData* RowDataTable;

	FSkillAbilityDataSheet* AbilityData;

	FTimerHandle BuffTimerHandle;
	float DamageBuff = 1.0f;
	UPROPERTY(EditAnywhere)
	float FastCoefficient = 0.0f;
	UPROPERTY(EditAnywhere)
	float ATKCoefficient = 0.0f;
	UPROPERTY(EditAnywhere)
	float ATKSpeedCoefficient = 0.0f;

	UPROPERTY()
	TArray<AActor*> HomingActors;

	UPROPERTY(EditAnywhere)
	float HomingSpeed = 1000.0f;

	float InstantDeathPercent = 0.0f;

	float CurrentDamage = 0.0f;

	float ChargeCurrent = 0.0f;
	float ChargeMax = 1.0f;

	UPROPERTY(EditAnywhere)
	uint8 HitNumbers = 2;
	
	UPROPERTY(EditAnywhere)
	float HitDelayTime = 0.25f;

	TWeakObjectPtr<USkillNumberBase> SkillComp;

	// 관통 관련
	int32 PenetrateCount = 1;
	bool bPenetrate = false;
	EPenetrateType PenetrateType = EPenetrateType::None;
	
public:
	float DefaultSize = 1.0f;
};
