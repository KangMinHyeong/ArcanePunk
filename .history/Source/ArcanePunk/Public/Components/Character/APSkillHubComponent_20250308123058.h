// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillDataTable/SkillDataTable.h"
#include "Components/ActorComponent.h"
#include "APSkillHubComponent.generated.h"

class USkillNumberBase;
class AArcanePunkCharacter;
class AArcanePunkPlayerController;
class AArcaneCutter;
class AArcaneRain;
class AArcaneBall;
class AArcaneMine;
class AArcaneTurret;
class AArcaneField;
class AArcaneRage;
class AArcaneTent;
class ASwordClutch;
class UNiagaraSystem;
class ATimeRewinder;
class AArcaneExecution;
class ABlackHole;
class AShadowShuriken;
class AWindRush;
class AArcaneMeteor;
class ACarpetBoom;
class ATerminator;
class AOverload;
class AArcaneReflector;
class AArcaneBomb;
class AArcaneBeam;
class AShouting;
class ASwordImpact;
class ASwordThrowBase;
class ASpinSlash;
class AArcaneWave;
class AWindCutter;
class ABomberMan;
class ASuperiorMode;
class AArcaneArea;
class AMoonSlash;
class AArcaneBlast;
class AArcaneCannon;
class AArcaneAlterEgo;
class AImitator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillHubComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSkillHubComponent();

	// Skill 관련 함수
	FORCEINLINE TSubclassOf<ASwordImpact> GetSwordImpactClass() const {return SwordImpactClass;}; //SwordImpactClass 반환
	FORCEINLINE TSubclassOf<ASwordThrowBase> GetSwordThrowClass() const {return SwordThrowClass;}; // SwordThrowClass 반환
	FORCEINLINE TSubclassOf<AArcaneBomb> GetArcaneBombClass() const {return ArcaneBombClass;}; // ArcaneBombClass 반환
	FORCEINLINE TSubclassOf<AArcaneBeam> GetArcaneBeamClass() const {return ArcaneBeamClass;}; // ArcaneBeamClass 반환
	FORCEINLINE TSubclassOf<AShouting> GetShoutingClass() const {return ShoutingClass;}; // ShoutingClass 반환
	FORCEINLINE TSubclassOf<AArcaneCutter> GetArcaneCutterClass() const {return ArcaneCutterClass;}; // ArcaneCutterClass 반환
	FORCEINLINE TSubclassOf<AArcaneBall> GetArcaneBallClass() const {return ArcaneBallClass;}; // ArcaneBallClass 반환
	FORCEINLINE TSubclassOf<AArcaneMine> GetArcaneMineClass() const {return ArcaneMineClass;}; // ArcaneMineClass 반환
	FORCEINLINE TSubclassOf<AArcaneTurret> GetArcaneTurretClass() const {return ArcaneTurretClass;}; // ArcaneTurretClass 반환
	FORCEINLINE TSubclassOf<AArcaneField> GetArcaneFieldClass() const {return ArcaneFieldClass;}; // ArcaneFieldClass 반환
	FORCEINLINE TSubclassOf<AArcaneRage> GetArcaneRageClass() const {return ArcaneRageClass;}; // ArcaneRageClass 반환
	FORCEINLINE TSubclassOf<AArcaneTent> GetArcaneTentClass() const {return ArcaneTentClass;}; // ArcaneTentClass 반환
	FORCEINLINE TSubclassOf<ASwordClutch> GetSwordClutchClass() const {return SwordClutchClass;}; // SwordClutchClass 반환
	FORCEINLINE TSubclassOf<ASpinSlash> GetSpinSlash() const {return SpinSlashClass;}; // SpinSlashClass 반환
	FORCEINLINE TSubclassOf<AMoonSlash> GetMoonSlash() const {return MoonSlashClass;}; // MoonSlashClass 반환
	FORCEINLINE TSubclassOf<AArcaneCannon> GetArcaneCannon() const {return ArcaneCannonClass;}; // ArcaneCannonClass 반환

	FORCEINLINE TSubclassOf<AArcaneRain> GetArcaneRainClass() const {return ArcaneRainClasss;}; // ArcaneRainClass 반환
	FORCEINLINE TSubclassOf<ATimeRewinder> GetTimeRewinderClass() const {return TimeRewinderClass;}; // TimeRewinderClass 반환
	FORCEINLINE TSubclassOf<AArcaneExecution> GetArcaneExecutionClass() const {return ArcaneExecutionClass;}; // ArcaneExecutionClass 반환
	FORCEINLINE TSubclassOf<ABlackHole> GetBlackHoleClass() const {return BlackHoleClass;}; // BlackHoleClass 반환
	FORCEINLINE TSubclassOf<AShadowShuriken> GetShadowShuriken() const {return ShadowShurikenClass;}; // ShadowShurikenClass 반환
	FORCEINLINE TSubclassOf<AWindRush> GetWindRush() const {return WindRushClass;}; // WindRushClass 반환
	FORCEINLINE TSubclassOf<AArcaneMeteor> GetArcaneMeteor() const {return ArcaneMeteorClass;}; // ArcaneMeteorClass 반환
	FORCEINLINE TSubclassOf<ACarpetBoom> GetCarpetBoom() const {return CarpetBoomClass;}; // ACarpetBoomClass 반환
	FORCEINLINE TSubclassOf<ATerminator> GetTerminator() const {return TerminatorClass;}; // TerminatorClass 반환
	FORCEINLINE TSubclassOf<AOverload> GetOverload() const {return OverloadClass;}; // OverloadClass 반환
	FORCEINLINE TSubclassOf<AArcaneReflector> GetArcaneReflector() const {return ArcaneReflectorClass;}; // ArcaneReflectorClass 반환
	FORCEINLINE TSubclassOf<AArcaneWave> GetArcaneWave() const {return ArcaneWaveClass;}; // ArcaneWaveClass 반환
	FORCEINLINE TSubclassOf<AWindCutter> GetWindCutter() const {return WindCutterClass;}; // WindCutterClass 반환
	FORCEINLINE TSubclassOf<ABomberMan> GetBomberMan() const {return BomberManClass;}; // BomberManClass 반환
	FORCEINLINE TSubclassOf<ASuperiorMode> GetSuperiorMode() const {return SuperiorModeClass;}; // SuperiorModeClass 반환
	FORCEINLINE TSubclassOf<AArcaneArea> GetArcaneArea() const {return ArcaneAreaClass;}; // ArcaneAreaClass 반환
	FORCEINLINE TSubclassOf<AArcaneBlast> GetArcaneBlast() const {return ArcaneBlastClass;}; // ArcaneBlastClass 반환
	FORCEINLINE TSubclassOf<AArcaneAlterEgo> GetArcaneAlterEgo() const {return ArcaneAlterEgoClass;}; // ArcaneAlterEgoClass 반환
	FORCEINLINE TSubclassOf<AImitator> GetImitator() const {return ImitatorClass;}; // ImitatorClass 반환

	FORCEINLINE UNiagaraSystem* GetArcaneTeleportEffect() const {return ArcaneTeleportEffect;}; // ArcaneTeleportEffect 반환
	FORCEINLINE UNiagaraSystem* GetHealingEffect() const {return HealingEffect;}; // HealingEffect 반환
	FORCEINLINE UNiagaraSystem* GetSkill16Effect() const {return Skill16Effect;}; // Skill16Effect 반환
	FORCEINLINE UNiagaraSystem* GetSkill17Effect() const {return Skill17Effect;}; // Skill17Effect 반환
	FORCEINLINE UNiagaraSystem* GetSkill20Effect() const {return Skill20Effect;}; // Skill20Effect 반환
	
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PressQ(); // Q Press 시 호출
	void PressE(); // E Press 시 호출
	void PressR(); // SpaceBar Press 시 호출

	UFUNCTION(BlueprintPure)
	FORCEINLINE ESkillKey GetSkillState() {return SkillState;};

	FORCEINLINE void SetSkillState(ESkillKey NewSkillKey) {SkillState = NewSkillKey;};

	void UpdatingSkill_Q();
	void UpdatingSkill_E();
	void UpdatingSkill_R();

	//  USkillNumberBase* CreateSkillNumber(ESkillNumber CreatedNumber);

	// USkillNumberBase* GetSKillNumberComp(ESkillNumber SkillNumber);

	void AutoRecoveryMP();
	void RecoveryMP();

	FORCEINLINE float GetRecoveryTime_MP() const {return RecoveryTime_MP;};
	FORCEINLINE void SetRecoveryTime_MP(float NewValue) {RecoveryTime_MP = NewValue;};

private:
	void PlayBasicSkill();
	
private:
	// 스킬 Base State
	ESkillKey SkillState = ESkillKey::None;

	TArray<ESkillKey>SkillStateArr;

	//TimerHandle
	FTimerHandle SkillCancleTimerHandle;
	FTimerHandle RecoveryMPTimerHandle;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<AArcanePunkPlayerController> OwnerCharacterPC;
	// UPROPERTY(EditAnywhere, Category = "Component")
	// UAPSkillNumber* SkillNumComp;
	bool Proceeding = false;

	UPROPERTY()
	float RecoveryTime_MP = 10.0f;

	// 스킬 관련 변수
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ASwordImpact> SwordImpactClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ASwordThrowBase> SwordThrowClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneBomb> ArcaneBombClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneBeam> ArcaneBeamClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AShouting> ShoutingClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneCutter> ArcaneCutterClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneBall> ArcaneBallClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneMine> ArcaneMineClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneTurret> ArcaneTurretClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneField> ArcaneFieldClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneRage> ArcaneRageClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneTent> ArcaneTentClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ASwordClutch> SwordClutchClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneRain> ArcaneRainClasss;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ATimeRewinder> TimeRewinderClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneExecution> ArcaneExecutionClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ABlackHole> BlackHoleClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AShadowShuriken> ShadowShurikenClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AWindRush> WindRushClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneMeteor> ArcaneMeteorClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ACarpetBoom> CarpetBoomClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ATerminator> TerminatorClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AOverload> OverloadClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneReflector> ArcaneReflectorClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ASpinSlash> SpinSlashClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneWave> ArcaneWaveClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AWindCutter> WindCutterClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ABomberMan> BomberManClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ASuperiorMode> SuperiorModeClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneArea> ArcaneAreaClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneBlast> ArcaneBlastClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneAlterEgo> ArcaneAlterEgoClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AArcaneCannon> ArcaneCannonClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AMoonSlash> MoonSlashClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AImitator> ImitatorClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UNiagaraSystem*  ArcaneTeleportEffect;	
	UPROPERTY(EditAnywhere, Category = "Skill")
	UNiagaraSystem* HealingEffect;
	UPROPERTY(EditAnywhere, Category = "Skill")
	UNiagaraSystem* Skill16Effect;
	UPROPERTY(EditAnywhere, Category = "Skill")
	UNiagaraSystem* Skill17Effect;
	UPROPERTY(EditAnywhere, Category = "Skill")
	UNiagaraSystem* Skill20Effect;

public:	
	ESkillKey LastSkill = ESkillKey::None;
};
