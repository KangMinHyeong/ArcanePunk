
#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterBase.h"
#include "Components/Character/APCharacterAuraComponent.h"
#include "Components/Common/APBuffComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "ArcanePunkCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAutoRecoveryMPDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftMouseClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAltRightMouseClick, FVector, ClickPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboAttackStart, uint8, ComboStack);

//Minhyeong
class UParticleSystem;
class UAPSkillHubComponent;
class UAPTakeDamageComponent;
class UAPSpawnFootPrintComponent;
class APickup;
class UArcanePunkCharacterAnimInstance;
class USpringArmComponent;
class UCameraComponent;
class AArcanePunkPlayerController;
class AArcanePunkPlayerState;
class AEnemy_DropUnlock;
class USkillNumberBase;
class AAPGameModeBase;
class UAPGhostTrailSpawnComponent;
class UAPSkillAbility;
class UAPPassiveComponent;
class UAPFadeOutTriggerComponent;
class UAPCharacterAuraComponent;

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

UCLASS()
class ARCANEPUNK_API AArcanePunkCharacter : public AAPCharacterBase
{
	GENERATED_BODY()

public:
	AArcanePunkCharacter();

protected:
	virtual void BeginPlay() override;
	void InitComponent();
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	FTransform ReturnCameraTransform();
	FORCEINLINE USpringArmComponent* GetMySpringArm() {return MySpringArm;};
	FORCEINLINE UAPFadeOutTriggerComponent* GetFadeOutTrigger() {return FadeOutTigger;};

	FORCEINLINE AAPHUD* GetAPHUD() const {return HUD;};

	void UpdateInventoryWidgetPosition(int32 Numbers);

	bool PMCheck(FHitResult& HitResult, FVector OverlapStart, FVector OverlapEnd); // 발 밑 메쉬의 피지컬 머터리얼 체크 // 그외에 캐릭터 근처 히트 체크
	
	virtual float GetCurrentATK() const override; // DefaultATK 반환
	virtual void UpdateStatus() override;

	// Attack 관련 함수
	virtual void SetAttackRotation() override; // 마우스 커서 방향으로 플레이어 회전

	FORCEINLINE UAPSkillAbility* GetAPSkillAbility() const {return SkillAbility;}; // SkillComp 반환
	FORCEINLINE UAPPassiveComponent* GetAPPassiveComp() const {return PassiveComp;}; // PassiveComp 반환

	FORCEINLINE float GetSkillCancelTime() const {return SkillCancelTime;}; // SkillCancleTime 반환
	FORCEINLINE ESkillNumber GetQSkill() const {return QSkill;}; // QSkill 반환
	FORCEINLINE void SetQSkill(ESkillNumber NewSkill) {QSkill = NewSkill;}; // QSkill 설정
	FORCEINLINE ESkillNumber GetESkill() const {return ESkill;}; // ESkill 반환
	FORCEINLINE void SetESkill(ESkillNumber NewSkill) {ESkill = NewSkill;}; // ESkill 설정
	FORCEINLINE EUltSkillNumber GetRSkill() const {return RSkill;}; // RSkill 반환	
	void SetRSkill(EUltSkillNumber NewSkill); // ESkill 설정
	FORCEINLINE TMap<uint8, FSkillAbilityNestingData> GetPassiveSkills() const {return PassiveSkills;}; // PassiveSkills 반환	
	
	void AddPassive(EPassiveNumber PassiveNum);
	void AddPassive_Enhance(uint8 PassiveNum, EEnHanceType EnHanceType, uint8 AbilityNum, uint16 AbilityNestingNum);

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAPSkillHubComponent* GetAPSkillHubComponent() const {return SkillHubComponent;}; // SkillComp 반환

	FORCEINLINE UAPGhostTrailSpawnComponent* GetGhostTrailSpawnComp() const {return GhostTrailSpawnComp;}; // APGhostTrailSpawnComp 반환

	FORCEINLINE bool GetOnQSkill() const {return OnQSkill;};
	FORCEINLINE bool GetOnESkill() const {return OnESkill;};
	FORCEINLINE bool GetOnRSkill() const {return OnRSkill;};

	FORCEINLINE USkillNumberBase* GetQSkillNumber() const {return Skill_Q;}; // SkillNumComp 반환
	FORCEINLINE USkillNumberBase* GetESkillNumber() const {return Skill_E;}; // SkillNumComp 반환
	FORCEINLINE USkillNumberBase* GetRSkillNumber() const {return Skill_R;}; // SkillNumComp 반환
	FORCEINLINE USkillNumberBase* GetSpareSkillNumber() const {return Skil_Spare;}; // SkillNumComp 반환

	FORCEINLINE void SetQSkillNumber(USkillNumberBase* NewObject) {Skill_Q = NewObject;}; // SkillNumComp 반환
	FORCEINLINE void SetESkillNumber(USkillNumberBase* NewObject) {Skill_E = NewObject;}; // SkillNumComp 반환
	FORCEINLINE void SetRSkillNumber(USkillNumberBase* NewObject) {Skill_R = NewObject;}; // SkillNumComp 반환
	FORCEINLINE void SetSpareSkillNumber(USkillNumberBase* NewObject) {Skil_Spare = NewObject;}; // SkillNumComp 반환

	void SetSkillAbility(EEnhanceCategory EnhanceCategory, EEnHanceType EnHanceType);
		
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
	FORCEINLINE void SetHideModeOnlyBool(bool NewBool) {bHideMode = NewBool;};

	void HideClear();
	FORCEINLINE bool IsEnhanceTent() const {return bEnhanceTent;};
	FORCEINLINE void SetEnhanceTent(bool NewBool) {bEnhanceTent = NewBool;};

	// 쿨타임 Bool
	FORCEINLINE bool GetbCanSkill_Q() const {return bCanSkill_Q;};
	FORCEINLINE void SetbCanSkill_Q(bool NewBool) {bCanSkill_Q = NewBool;};
	FORCEINLINE bool GetbCanSkill_E() const {return bCanSkill_E;};
	FORCEINLINE void SetbCanSkill_E(bool NewBool) {bCanSkill_E = NewBool;};
	FORCEINLINE bool GetbCanSkill_R() const {return bCanSkill_R;};
	FORCEINLINE void SetbCanSkill_R(bool NewBool) {bCanSkill_R = NewBool;};

	// 무적 관련 함수
	FORCEINLINE uint8 GetReflectMode() const {return ReflectingModeGauge;};
	FORCEINLINE void SetReflectMode(uint8 NewValue) {ReflectingModeGauge = NewValue;};

	// Hit, Dead 관련 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAPTakeDamageComponent* GetTakeDamageComponent() const {return TakeDamageComp;}; // DamageComp 반환
	
	virtual bool IsDead() override; // HP가 0이하인지 반환

	void DeadPenalty(float DeathTime); // DeadPenalty 발동

	//FootPrint 관련 함수
	FORCEINLINE UAPSpawnFootPrintComponent* GetSpawnFootPrintComponent() const {return APSpawnStepComp;}; // HitMotionTime 반환	
	FORCEINLINE UNiagaraComponent* GetPlayerPanelAura() {return CharacterAura->GetAuraEffect();}; // PlayerPanel 반환
	FTransform GetFootTransform(bool Left);	
	
	// 장비 관련 함수
	USkeletalMeshComponent* GetPlayerEquipment(uint8 NewValue);
	UAPItemBase* GetEquipData(uint8 NewValue);
	void SetEquipData(uint8 NewValue, UAPItemBase* NewData);

	// Buff 관련 함수
	FORCEINLINE UAPBuffComponent* GetBuffComp() const {return BuffComp;}; // BuffComp 반환

	// Save 관련 함수
	void SaveStatus(FString PlayerSlotName);

	// Interaction 관련 함수
	bool IsInteraction() const {return bInteract;};
	void SetInteraction(bool NewBool) {bInteract = NewBool;};

	void ActivateInteractionSweep();
	void InteractionActorRemove(AActor* InteractionActor);
	
	// 달리기
	FORCEINLINE bool GetbJogging() const {return bJogging;};
	void EndJog(); // 빨리 달리기 끝
	
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

	void WorldMap();

	void Alt_RightClick();

	// Skill 관련 함수
	void HideCheck();

	// Move 관련 함수
	void MoveForward(float AxisValue); // 위 아래 Move
	void MoveRight(float AxisValue); // 오른, 왼쪽 Move

	void StartJog(); // 빨리 달리기 시작

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

	// 무기 위치 설정
	void SetWeaponPosition();

	
private:
	// 부착 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSkillHubComponent* SkillHubComponent;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPTakeDamageComponent* TakeDamageComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSpawnFootPrintComponent* APSpawnStepComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPBuffComponent* BuffComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	USceneComponent* LeftBeamPoint;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPGhostTrailSpawnComponent* GhostTrailSpawnComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSkillAbility* SkillAbility;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPPassiveComponent* PassiveComp;
	
	//PlayerController 변수
	TWeakObjectPtr<AArcanePunkPlayerController> PC;
	//GameMode 변수
	TWeakObjectPtr<AAPGameModeBase> GM;

	// 장비, 무기 변수
	UPROPERTY(EditAnywhere, Category = "Equipment")
	USkeletalMeshComponent* Weapon;

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
	UAPFadeOutTriggerComponent* FadeOutTigger;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinimumSpringArmLength = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomCoefficient = 20.0f;

	float MaximumSpringArmLength = 0.0f;

	float CurrentArmLength = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Move")
	float DashSpeed = 650.0f;

	// State 관련 변수
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AArcanePunkPlayerState* MyPlayerState;

	float FinalATK = 0.0f;

	UPROPERTY()
	USkillNumberBase* Skill_Q;

	UPROPERTY()
	USkillNumberBase* Skill_E;

	UPROPERTY()
	USkillNumberBase* Skill_R;

	UPROPERTY()
	USkillNumberBase* Skil_Spare;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float SkillCancelTime = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	ESkillNumber QSkill = ESkillNumber::Skill_1;

	UPROPERTY(EditAnywhere, Category = "Skill")
	ESkillNumber ESkill = ESkillNumber::Skill_2;

	UPROPERTY(EditAnywhere, Category = "Skill")
	EUltSkillNumber RSkill = EUltSkillNumber::UltSkill_1;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TMap<uint8, FSkillAbilityNestingData> PassiveSkills;

	// TArray<ESkillAbility> SkillAbilityQ;
	// TArray<ESkillAbility> SkillAbilityE;
	// TArray<ESkillAbility> SkillAbilityR;

	FVector HomingPoint = FVector(0,0,0);

	bool bRageMode = false;

	// 은신 관련 변수
	FTimerHandle HideTimerHandle;
	bool bEnhanceTent = false;
	bool InArcaneTent = false;
	bool bHideMode = false;
	float ReturnToHideTime = 1.0f;

	
	bool OnQSkill = false;

	bool OnESkill = false;

	bool OnRSkill = false;

	bool bCanSkill_Q = true;  // Q쿨타임 체크
	bool bCanSkill_E = true;  // E쿨타임 체크
	bool bCanSkill_R = true;  // R쿨타임 체크

	uint8 ReflectingModeGauge = 0;

	// Foot Print 변수
	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UStaticMeshComponent* FootPrint_L;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UStaticMeshComponent* FootPrint_R;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UAPCharacterAuraComponent* CharacterAura;

	bool bJogging = false;

	// 드랍
	UPROPERTY(EditAnywhere, Category = "Drop")
	TSubclassOf<APickup> DropClass;

	// Interaction
	bool bInteract = false;

	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractionRadius = 60.0f;
	
	UPROPERTY()
	TArray<AActor*> InteractionActors;

public:
	TArray<ESkillNumber> HavingSkill;

	FOnAutoRecoveryMPDelegate OnAutoRecoveryMPDelegate;

	FOnLeftMouseClick OnLeftMouseClick;

	FOnAltRightMouseClick OnAltRightMouseClick;
	
	FOnComboAttackStart OnComboAttackStart;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UParticleSystem* Skill3_Effect;

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

	// void PerformInteractionCheck();
	// void FoundInteractable(AActor* NewInteractable);
	// void NoInteractableFound();
	// void Interact();
	void BeginInteract();
	void EndInteract();

public :

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UAPInventoryComponent* GetInventory() const { return PlayerInventory; };

	// void UpdateInteractionWidget() const;

	void DropItems(UAPItemBase* ItemToDrop, const int32 QuantityToDrop);

	UFUNCTION()
	void InventorySort();
};
