
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameState/APGameState.h"
#include "PlayerState/APPlayerData.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Interfaces/InteractionInterface.h"
#include "ArcanePunkCharacter.generated.h"

//Minhyeong
class UDataTable;
class UAPAttackComponent;
class UParticleSystem;
class UAPMovementComponent;
class UAPSkillHubComponent;
class ASwordImpact;
class ASwordThrowBase;
class UAPTakeDamageComponent;
class UAPSpawnFootPrintComponent;
class UAPAnimHubComponent;
class APickup;
class UArcanePunkCharacterAnimInstance;
class USpringArmComponent;
class UCameraComponent;
class AArcanePunkPlayerController;
class UNiagaraComponent;
class AArcanePunkPlayerState;
class AEnemy_DropUnlock;
class AArcaneBomb;
class AArcaneBeam;
class AShouting;
class AAPSkillRange;
class AAPSkillRange_Target;
class AAPSkillRange_Arrow;
class AAPSkillRange_Circle;
class AArcaneCutter;
class AArcaneRain;
class AArcaneBall;
class AArcaneMine;
class AArcaneTurret;
class AArcaneField;
class AArcaneRage;
class AArcaneTent;
class ASwordClutch;

// prodo

class UAPItemBase;
class UAPInventoryComponent;
class AAPHUD;
class ATextRenderActor;

#define Defense_constant 1000

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f) {};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UENUM(BlueprintType)
enum class EEnHanceType : uint8
{
	Silver		 = 0,
    Gold		 = 1,
    Platinum     = 2,
};

UCLASS()
class ARCANEPUNK_API AArcanePunkCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AArcanePunkCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	FTransform ReturnCameraTransform();

	FORCEINLINE UMaterialInterface* GetHitMaterial() const {return HitMaterial;}; // HitMaterial 반환
	FORCEINLINE UMaterialInterface* GetDefaultMaterial() const {return DefaultMaterial;}; // DefaultMaterial 반환

	bool PMCheck(FHitResult& HitResult, FVector OverlapStart, FVector OverlapEnd); // 발 밑 메쉬의 피지컬 머터리얼 체크 // 그외에 캐릭터 근처 히트 체크
	
	FORCEINLINE void SetDoing(bool NewBool) {bDoing = NewBool;}; // bDoing 설정
	FORCEINLINE bool GetDoing() const {return bDoing;}; // bDoing 반환

	//Effect
	FORCEINLINE UNiagaraComponent* GetStunEffect() {return StunEffect;}; // StunEffect 반환

	//플레이어 스테이터스 관련 함수
	FORCEINLINE FPlayerTotalData GetPlayerStatus() const {return MyPlayerTotalStatus;}; // MyPlayerTotalStatus 반환
	FORCEINLINE void SetPlayerStatus(FPlayerTotalData NewPlayerData) {MyPlayerTotalStatus = NewPlayerData;}; // MyPlayerTotalStatus 설정 

	UFUNCTION(BlueprintPure)
	FORCEINLINE ECharacterState returnState() const {return CurrentState;}; // CurrentState 반환
	FORCEINLINE void SetState(ECharacterState UpdateState) { CurrentState = UpdateState;}; // CurrentState 설정

	void UpdateStatus();

	float GetFinalATK(float Multiple = 2.0f);// FinalATK 반환

	// Attack 관련 함수
	FORCEINLINE UAPAttackComponent* GetAttackComponent() const {return AttackComp;};  // AttackComp 반환
	FORCEINLINE float GetMaxDistance() const {return MaxDistance;}; // 공격 사거리 반환
	FORCEINLINE float GetAttackRadius() const {return AttackRadius;}; // 공격 반경 반환
	FORCEINLINE UParticleSystem* GetComboHitEffect() const {return ComboHitEffect;}; // 콤보어택 히트 파티클 반환;
	FORCEINLINE FVector GetComboHitEffectScale() const {return HitEffectScale;}; // 콤보어택 히트 스케일 반환;

	// Move 관련 함수
	FORCEINLINE UAPMovementComponent* GetAPMoveComponent() const {return MoveComp;}; // MoveComp 반환
	FORCEINLINE bool GetCanMove() const {return bCanMove;}; // bCanMove 반환;
	FORCEINLINE void SetCanMove(bool NewValue) {bCanMove = NewValue;}; // bCanMove 설정;
	float GetAttackMoveSpeed(int32 Section);// 콤보 어택 Section Speed 반환
	FORCEINLINE float GetPushCoefficient() const {return AttackPushCoefficient;}; // 콤보 어택 시 Enemy Push 계수 반환
	FORCEINLINE void SetCanJog(bool NewValue) {bCanJog = NewValue;}; // bCanJog 설정;
	FORCEINLINE float GetDefaultSpeed() const {return DefaultSpeed;}; // DefaultSpeed 반환;
	
	// Skill 관련 함수
	UFUNCTION(BlueprintPure)
	FORCEINLINE UAPSkillHubComponent* GetAPSkillHubComponent() const {return APSkillHubComp;}; // SkillComp 반환
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
	FORCEINLINE TSubclassOf<AArcaneRain> GetArcaneRainClass() const {return ArcaneRainClass;}; // ArcaneRainClass 반환

	FORCEINLINE float GetSkillCancelTime() const {return SkillCancelTime;}; // SkillCancleTime 반환
	FORCEINLINE ESkillNumber GetQSkill() const {return QSkill;}; // QSkill 반환
	FORCEINLINE void SetQSkill(ESkillNumber NewSkill) {QSkill = NewSkill;}; // QSkill 설정
	FORCEINLINE ESkillNumber GetESkill() const {return ESkill;}; // ESkill 반환
	FORCEINLINE void SetESkill(ESkillNumber NewSkill) {ESkill = NewSkill;}; // ESkill 설정
	FORCEINLINE EUltSkillNumber GetRSkill() const {return RSkill;}; // RSkill 반환
	void SetRSkill(); // RSkill 설정

	FORCEINLINE TArray<ESkillAbility> GetAbilitySkillQ() const {return SkillAbilityQ;}; // SkillAbilityQ 반환
	FORCEINLINE void SetAbilitySkillQ(TArray<ESkillAbility> NewSkillAbility) {SkillAbilityQ = NewSkillAbility;}; // SkillAbilityQ 설정
	FORCEINLINE TArray<ESkillAbility> GetAbilitySkillE() const {return SkillAbilityE;}; // SkillAbilityE 반환
	FORCEINLINE void SetAbilitySkillE(TArray<ESkillAbility> NewSkillAbility) {SkillAbilityE = NewSkillAbility;}; // SkillAbilityE 설정
	FORCEINLINE TArray<ESkillAbility> GetAbilitySkillR() const {return SkillAbilityR;}; // SkillAbilityR 반환
	FORCEINLINE void SetAbilitySkillR(TArray<ESkillAbility> NewSkillAbility) {SkillAbilityR = NewSkillAbility;}; // SkillAbilityR 설정

	FORCEINLINE bool GetOnQSkill() const {return OnQSkill;};
	FORCEINLINE bool GetOnESkill() const {return OnESkill;};
	FORCEINLINE bool GetOnRSkill() const {return OnRSkill;};

	FORCEINLINE TSubclassOf<AAPSkillRange> GetAPSkillRange() const {return SkillRange;};
	FORCEINLINE TSubclassOf<AAPSkillRange_Target> GetAPSkillRange_Target() const {return SkillRange_Target;};
	FORCEINLINE TSubclassOf<AAPSkillRange_Arrow> GetAPSkillRange_Arrow() const {return SkillRange_Arrow;};
	FORCEINLINE TSubclassOf<AAPSkillRange_Circle> GetAPSkillRange_Circle() const {return SkillRange_Circle;};

	void SetSkillAbility(EEnhanceCategory EnhanceCategory, EEnHanceType EnHanceType);
	
	FORCEINLINE float GetSkill3_LimitDist() const {return Skill3_LimitDist;}; // Skill3_LimitDist 반환
	
	FORCEINLINE FVector GetHomingPoint() const {return HomingPoint;};
	FORCEINLINE void SetHomingPoint(FVector NewHomingPoint) {HomingPoint = NewHomingPoint;};

	FORCEINLINE USceneComponent* GetLeftBeamPoint() const {return LeftBeamPoint;}; 

	void SetHavingSkills(); 

	FORCEINLINE bool GetRageMode() const {return bRageMode;};
	FORCEINLINE void SetRageMode(bool NewBool) {bRageMode = NewBool;};

	FORCEINLINE void SetReturnToHideTime(float NewTime) {ReturnToHideTime = NewTime;};
	void SetInArcaneTent(bool NewBool);
	void SetHideMode(bool NewBool);
	FORCEINLINE bool GetInArcaneTent() const {return InArcaneTent;};
	FORCEINLINE bool GetHideMode() const {return bHideMode;};
	FORCEINLINE UMaterialInterface* GetHideMaterial() const {return HideMaterial;};
	void HideClear();

	// 무적 관련 함수
	FORCEINLINE bool IsBlockMode() const {return bBlockMode;};

	// Hit, Dead 관련 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAPTakeDamageComponent* GetTakeDamageComponent() const {return TakeDMComp;}; // DamageComp 반환

	UFUNCTION(BlueprintPure)
	bool IsDead(); // HP가 0이하인지 반환

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsHitting() const {return bHitting;};  // bHitting 반환
	FORCEINLINE void SetHitting(bool NewBool) {bHitting = NewBool;}; // bHitting 설정
	FORCEINLINE float GetHitMotionTime() const {return HitMotionTime;}; // HitMotionTime 반환

	FORCEINLINE void DeadPenalty(); // DeadPenalty 발동

	//FootPrint 관련 함수
	FORCEINLINE UAPSpawnFootPrintComponent* GetSpawnFootPrintComponent() const {return SpawnFootPrintComp;}; // HitMotionTime 반환

	TSubclassOf<AActor> GetFootClass(bool Left);
	FTransform GetFootTransform(bool Left);

	// 장비 관련 함수
	USkeletalMeshComponent* GetPlayerEquipment(uint8 NewValue);
	UAPItemBase* GetEquipData(uint8 NewValue);
	void SetEquipData(uint8 NewValue, UAPItemBase* NewData);

	// CC 관련 함수
	FORCEINLINE UAPCrowdControlComponent* GetCrowdControlComponent() const {return CrowdControlComp;}; // CrowdControlComp 반환

	// Save 관련 함수
	void SaveStatus(FString PlayerSlotName, FString GameSlotName);

private:
	void InitPlayerStatus();
	
	void ZoomInOut(float AxisValue);
	
	void SkillBase_Q();
	void Release_Q() {OnQSkill = false;};

	void SkillBase_E();
	void Release_E() {OnESkill = false;};

	void SkillBase_R();
	void Release_R() {OnRSkill = false;};

	void Save();
	void CurrentPlayerLocation();

	// Skill 관련 함수
	void HideCheck();

	// 크리티컬 데미지 계산
	float CriticalCalculate(float Multiple);

	// Move 관련 함수
	void MoveForward(float AxisValue); // 위 아래 Move
	void MoveRight(float AxisValue); // 오른, 왼쪽 Move

	void StartJog(); // 빨리 달리기 시작

	void EndJog(); // 빨리 달리기 끝

	virtual void Jump() override;

	void Dash();

	void OnBlockMode();
	void ClearBlockMode();

	// 공격 관련 함수
	void Attack_typeA(); // 마우스 오른쪽 공격
	void Attack_typeB(); // 마우스 왼쪽 공격

	//캐릭터장비 데이터 초기화
	void InitEquipData(TArray<UAPItemBase*> & EquipArr, FName EquipID);
	void ChangeEquipData(TArray<UAPItemBase*> & EquipArr, UAPItemBase* NewData);

private:
	// 부착 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Component")
	UAPAttackComponent* AttackComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPMovementComponent* MoveComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSkillHubComponent* APSkillHubComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPAnimHubComponent* AnimHubComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPTakeDamageComponent* TakeDMComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSpawnFootPrintComponent* SpawnFootPrintComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPCrowdControlComponent* CrowdControlComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	USceneComponent* LeftBeamPoint;

	//PlayerController 변수
	AArcanePunkPlayerController* PC;
	
	// 장비, 무기 변수
	UPROPERTY(EditAnywhere, Category = "Equipment")
	USkeletalMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	UDataTable* EquipDataTable;

	UPROPERTY(EditAnywhere, Category = "Equipment") // 1개만 채우기
	TArray<UAPItemBase*> WeaponReference;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FName DesiredWeaponID;

	// 카메라, 스프링암 변수
	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* MySpringArm;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* MyCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinimumSpringArmLength = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomCoefficient = 20.0f;

	float MaximumSpringArmLength = 0.0f;

	float CurrentArmLength = 0.0f;

	// 이동 관련 변수
	UPROPERTY()
	bool bCanMove = true;

	float DefaultSpeed = 400.0f;

	UPROPERTY()
	bool bCanJog = true;

	UPROPERTY(EditAnywhere, Category = "Move")
	float AttackPushCoefficient = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Move")
	float DashSpeed = 650.0f;

	// 캐릭터 상태 관련 변수
	UPROPERTY(EditAnywhere)
	float HitMotionTime = 1.0f;
	bool bHitting = false;

	UPROPERTY(EditAnywhere)
	float State_Time = 3.0f;

	ECharacterState CurrentState = ECharacterState::None;

	float DefaultSlip = 0.0f;

	float StateTime = 3.0f;

	bool bDoing = false; // 공격 , 스킬 사용중인지 체크하는 변수 / true면 다른 행동 제약 

	UPROPERTY(EditAnywhere, Category = "CC State")
	UNiagaraComponent* StunEffect;

	UPROPERTY()
	UArcanePunkCharacterAnimInstance* Anim;

	// State 관련 변수
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AArcanePunkPlayerState* MyPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerTotalData MyPlayerTotalStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameData MyGameStatus;

	UPROPERTY()
	float FinalATK = 0.0f;

	// 스킬 관련 변수
	UPROPERTY(EditAnywhere, Category = "Skill")
	float Skill3_LimitDist = 1500.0f;

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
	TSubclassOf<AArcaneRain> ArcaneRainClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float SkillCancelTime = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	ESkillNumber QSkill = ESkillNumber::Skill_1;
	UPROPERTY()
	TArray<ESkillAbility> SkillAbilityQ;

	UPROPERTY(EditAnywhere, Category = "Skill")
	ESkillNumber ESkill = ESkillNumber::Skill_2;
	UPROPERTY()
	TArray<ESkillAbility> SkillAbilityE;

	UPROPERTY(EditAnywhere, Category = "Skill")
	EUltSkillNumber RSkill = EUltSkillNumber::UltSkill_1;
	UPROPERTY()
	TArray<ESkillAbility> SkillAbilityR;

	UPROPERTY()
	FVector HomingPoint = FVector(0,0,0);

	UPROPERTY()
	bool bRageMode = false;

	UPROPERTY()
	bool InArcaneTent = false;
	UPROPERTY()
	bool bHideMode = false;
	UPROPERTY()
	float ReturnToHideTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UMaterialInterface* HideMaterial;

	FTimerHandle HideTimerHandle;

	UPROPERTY()
	bool OnQSkill = false;
	UPROPERTY()
	bool OnESkill = false;
	UPROPERTY()
	bool OnRSkill = false;

	UPROPERTY()
	bool bCanSkill = true;

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	TSubclassOf<AAPSkillRange> SkillRange;

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	TSubclassOf<AAPSkillRange_Target> SkillRange_Target;

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	TSubclassOf<AAPSkillRange_Arrow> SkillRange_Arrow;

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	TSubclassOf<AAPSkillRange_Circle> SkillRange_Circle;

	// 무적기 관련 변수
	FTimerHandle BlockTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Super Stance")
	float BlockTime = 1.5f;

	UPROPERTY()
	bool bBlockMode = false;

	// Foot Print 변수
	UPROPERTY(EditAnywhere, Category = "Foot Print")
	TSubclassOf<AActor> LeftFootClass;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	TSubclassOf<AActor> RightFootClass;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UStaticMeshComponent* FootPrint_L;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UStaticMeshComponent* FootPrint_R;

	//Attack Variable
	UPROPERTY(EditAnywhere, Category = "Attack")
	float Attack1_MoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float Attack2_MoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float Attack3_MoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* ComboHitEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float MaxDistance = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRadius = 40.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FVector HitEffectScale = FVector(1,1,1);

	// 머터리얼
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial;

	// 드랍
	UPROPERTY(EditAnywhere, Category = "Drop")
	TSubclassOf<APickup> DropClass;



public:
	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial_Test1; // Test용 후에 삭제

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial_Test2; // Test용 후에 삭제

	UPROPERTY()
	TArray<uint8> MouseArr;

	UPROPERTY()
	TArray<bool> StopState;

	UPROPERTY()
	TArray<ESkillNumber> HavingSkill;

	// UPROPERTY()
	// TArray<ESkillTypeState> SkillTypeState; // 나중에 쓸수도?

	UPROPERTY(EditAnywhere, Category = "Skill")
	UParticleSystem* Skill3_Effect;

	void UpdateInventoryWidgetPosition(int32 Numbers);

// prodo

protected:
	UPROPERTY()
	AAPHUD* HUD;

	UPROPERTY()
	ATextRenderActor* TextRenderActor;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UAPInventoryComponent* PlayerInventory;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	void ToggleMenu();

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

public :

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UAPInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	void DropItems(UAPItemBase* ItemToDrop, const int32 QuantityToDrop);

	UFUNCTION()
	void InventorySort();
};
