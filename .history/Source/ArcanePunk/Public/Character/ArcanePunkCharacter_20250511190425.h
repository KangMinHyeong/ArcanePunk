
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
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboAttackStart, uint8, ComboStack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteract, AArcanePunkCharacter*, PlayerCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteract, AArcanePunkCharacter*, PlayerCharacter);

//Minhyeong
class UParticleSystem;
class UAPSkillHubComponent;
class UAPSpawnFootPrintComponent;
class APickup;
class UArcanePunkCharacterAnimInstance;
class UAPSpringArmComponent;
class UAPCameraComponent;
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
class ASkillController;

#define Defense_constant 1000

UENUM()
enum class EPlayerState : uint8 // Player 상태태
{
    None      			    = 0,
    Idle	 			    = 1, // 플레이어 입력을 받을 수 있는 일반 상태
    Spawn 					= 2, // 스폰된 직 후 상태
	EntryMode				= 3, // 플레이어 스왑 대쉬 상태
    EntrySkillPlaying       = 4, // 출격 스킬 플레이 상태
	ExitSkillPlaying 		= 5, // 퇴각 스킬 플레이 상태
	ActiveSkillPlaying 		= 6, // 일반 액티브 스킬 플레이 상태
	Inactive				= 7, // 비활성화 상태
};

UCLASS()
class ARCANEPUNK_API AArcanePunkCharacter : public AAPCharacterBase
{
	GENERATED_BODY()

public:
	AArcanePunkCharacter();

	FORCEINLINE UAPSpringArmComponent* GetAPSpringArm() {return APSpringArm;};
	FORCEINLINE UAPSpringArmComponent* GetAPSpringArm_Back() {return APSpringArm_Back;};
	FORCEINLINE UAPCameraComponent* GetAPCameraComponent() {return APCamera;};
	FORCEINLINE UAPCameraComponent* GetAPCameraComponent_Back() {return APCamera_Back;};
	FORCEINLINE UAPFadeOutTriggerComponent* GetFadeOutTrigger() {return FadeOutTigger;};

	FORCEINLINE AAPHUD* GetAPHUD() const {return HUD;};

	FORCEINLINE UAPSkillAbility* GetAPSkillAbility() const {return SkillAbility;}; // SkillComp 반환
	FORCEINLINE UAPPassiveComponent* GetAPPassiveComp() const {return PassiveComp;}; // PassiveComp 반환

	FORCEINLINE bool IsSkilling() const {return bSkilling;};

	FORCEINLINE float GetSkillCancelTime() const {return SkillCancelTime;}; // SkillCancleTime 반환
	FORCEINLINE ESkillNumber GetQSkill() const {return QSkill;}; // QSkill 반환
	FORCEINLINE void SetQSkill(ESkillNumber NewSkill) {QSkill = NewSkill;}; // QSkill 설정
	FORCEINLINE ESkillNumber GetESkill() const {return ESkill;}; // ESkill 반환
	FORCEINLINE void SetESkill(ESkillNumber NewSkill) {ESkill = NewSkill;}; // ESkill 설정
	FORCEINLINE EUltSkillNumber GetRSkill() const {return RSkill;}; // RSkill 반환	
	void SetRSkill(EUltSkillNumber NewSkill); // ESkill 설정
	FORCEINLINE TMap<uint8, FSkillAbilityNestingData> GetPassiveSkills() const {return PassiveSkills;}; // PassiveSkills 반환	

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAPSkillHubComponent* GetAPSkillHubComponent() const {return SkillHubComponent;}; // SkillComp 반환

	FORCEINLINE UAPGhostTrailSpawnComponent* GetGhostTrailSpawnComp() const {return GhostTrailSpawnComp;}; // APGhostTrailSpawnComp 반환

	FORCEINLINE bool GetOnQSkill() const {return OnQSkill;};
	FORCEINLINE bool GetOnESkill() const {return OnESkill;};
	FORCEINLINE bool GetOnRSkill() const {return OnRSkill;};

	FORCEINLINE FVector GetHomingPoint() const {return HomingPoint;};
	FORCEINLINE void SetHomingPoint(const FVector & NewHomingPoint) {HomingPoint = NewHomingPoint;};

	FORCEINLINE USceneComponent* GetLeftBeamPoint() const {return LeftBeamPoint;}; 
	
	FORCEINLINE bool GetRageMode() const {return bRageMode;};
	FORCEINLINE void SetRageMode(bool NewBool) {bRageMode = NewBool;};

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
	
	// Dash
	FORCEINLINE void SetbCanDash(bool NewBool) {bCanDash = NewBool;};	
	FORCEINLINE void SetAttackCancelTime(float NewValue) {AttackCancelTime = NewValue;};	
	
	// Parrying
	FORCEINLINE void SetbCanParrying(bool NewBool) {bCanParrying = NewBool;};	
	FORCEINLINE float GetParryingCoolTime() const {return ParryingCoolTime;};
	
	//FootPrint 관련 함수
	FORCEINLINE UAPSpawnFootPrintComponent* GetSpawnFootPrintComponent() const {return APSpawnStepComp;}; // HitMotionTime 반환	
	FORCEINLINE UNiagaraComponent* GetPlayerPanelAura() {return CharacterAura->GetAuraEffect();}; // PlayerPanel 반환

	// Buff 관련 함수
	FORCEINLINE UAPBuffComponent* GetBuffComp() const {return BuffComp;}; // BuffComp 반환
	
	FORCEINLINE float GetSwapGauge() const {return SwapGauge;};
	FORCEINLINE void SetSwapGauge(float NewValue) {SwapGauge = NewValue;};

protected:
	virtual void BeginPlay() override;

	virtual void DissolveCharacterMesh(float DeltaTime) override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	void InitPlayer();
	void InitSkill();
	void InitComponent();

	void ChangePlayer(AArcanePunkPlayerController* NewController);

	bool CheckSwapPlayerCondition();
	void SwapMainPlayer(bool NewBool, bool bEnhanceSwap, AActor* SwapEnemy, bool bPrioritySkilling = false);
	void SwitchPlayerState();
	
	virtual void UpdateSwapGauge(float Add = 10.0f) override;

	const FTransform & ReturnCameraTransform();

	void UpdateInventoryWidgetPosition(int32 Numbers);

	bool PMCheck(FHitResult& HitResult, const FVector & OverlapStart, const FVector & OverlapEnd); // 발 밑 메쉬의 피지컬 머터리얼 체크 // 그외에 캐릭터 근처 히트 체크
	
	virtual float GetCurrentATK() const override; // DefaultATK 반환
	virtual void UpdateStatus() override;

	void BeginInteract();

	// Attack 관련 함수
	virtual void SetAttackRotation(float AddSpeed = 0.0f) override; // 마우스 커서 방향으로 플레이어 회전
	
	void AddPassive(const EPassiveNumber & PassiveNum);
	void AddPassive_Enhance(uint8 PassiveNum, const EEnHanceType & EnHanceType, uint8 AbilityNum, uint16 AbilityNestingNum);

	void SetSkillAbility(EEnhanceCategory EnhanceCategory, EEnHanceType EnHanceType);
	void SetHavingSkills(); 
	

	// Hit, Dead 관련 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool IsDead() override; // HP가 0이하인지 반환

	void DeadPenalty(float DeathTime); // DeadPenalty 발동

	FTransform GetFootTransform(bool Left);	
	
	// 장비 관련 함수
	USkeletalMeshComponent* GetPlayerEquipment(uint8 NewValue);
	UAPItemBase* GetEquipData(uint8 NewValue);
	void SetEquipData(uint8 NewValue, UAPItemBase* NewData);

	// Save 관련 함수
	void SaveStatus(const FString & PlayerSlotName);
	void SavePlayerData(const FString & PlayerSlotName);
	void SaveGameData(const FString & PlayerSlotName);

	// Dash
	void PressedDash();
	void ReleasedDash();

	void PlaySwapDash();
	void EndSwapDash();
		
	// Skill 관련 함수
	void RestoreSkills();
	
private:
	void InitPlayerStatus();
	
	void ZoomInOut(float AxisValue);

	void CurrentPlayerLocation();

	void WorldMap();

	void Alt_RightClick();

	// Move 관련 함수
	void MoveForward(float AxisValue); // 위 아래 Move
	void MoveRight(float AxisValue); // 오른, 왼쪽 Move

	void OnBlockMode();
	void ClearBlockMode();

	// 공격 관련 함수
	void ComboAttack(); // 마우스 오른쪽 공격
	void Parrying(); // 패링

	//캐릭터장비 데이터 초기화
	void InitEquipData(TArray<UAPItemBase*> & EquipArr, FName EquipID);
	void ChangeEquipData(TArray<UAPItemBase*> & EquipArr, UAPItemBase* NewData);

	// 무기 위치 설정
	void SetWeaponPosition();

	// 데미지 몇번 받았는지
	bool CheckingDamaged();

	void UseSwapSkill_Retreat(); // 퇴격
	void UseSwapSkill_Sally(); // 출격

private:
	bool bMainPlayer = false;
	// 부착 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSkillHubComponent* SkillHubComponent;

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
	UAPSpringArmComponent* APSpringArm;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UAPCameraComponent* APCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UAPSpringArmComponent* APSpringArm_Back;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UAPCameraComponent* APCamera_Back;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UAPFadeOutTriggerComponent* FadeOutTigger;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float WheelZoomCoefficient = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float WheelZoomTime = 0.5f;

	bool bCanParrying = true;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ParryingCoolTime = 5.0f;

	bool bCanDash = true;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float DashCoolTime = 5.0f;
	
	// State 관련 변수
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AArcanePunkPlayerState* MyPlayerState;

	float AttackCancelTime = 0.0f;
	float FinalATK = 0.0f;

	float SwapGauge = 0.0f;

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

	FVector HomingPoint = FVector(0,0,0);

	bool bRageMode = false;

	bool bSkilling = false;

	bool OnQSkill = false;
	bool OnESkill = false;
	bool OnRSkill = false;

	bool bCanSkill_Q = true;  // Q쿨타임 체크
	bool bCanSkill_E = true;  // E쿨타임 체크
	bool bCanSkill_R = true;  // R쿨타임 체크

	bool EnhanceSwap = false;

	uint8 ReflectingModeGauge = 0;

	// Foot Print 변수
	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UStaticMeshComponent* FootPrint_L;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UStaticMeshComponent* FootPrint_R;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UAPCharacterAuraComponent* CharacterAura;

	// 드랍
	UPROPERTY(EditAnywhere, Category = "Drop")
	TSubclassOf<APickup> DropClass;

	// FTimerHandle
	FTimerHandle SwapDashTimerHandle;

public:
	TArray<ESkillNumber> HavingSkill;

	FOnAutoRecoveryMPDelegate OnAutoRecoveryMPDelegate;

	FOnLeftMouseClick OnLeftMouseClick;

	FOnAltRightMouseClick OnAltRightMouseClick;
	
	// FOnComboAttackStart OnComboAttackStart;

	FOnBeginInteract OnBeginInteract;
	FOnEndInteract OnEndInteract;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UParticleSystem* Skill3_Effect;

	UPROPERTY(EditAnywhere, Category = "Character UI ID")
	uint8 CharacterUIID = 0;

	uint8 PlayerIndex = 0;

	EPlayerState PlayerState = EPlayerState::Inactive;
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

	void ToggleMenu();

public :
	FORCEINLINE UAPInventoryComponent* GetInventory() const { return PlayerInventory; };

	void DropItems(UAPItemBase* ItemToDrop, const int32 QuantityToDrop);

	UFUNCTION()
	void InventorySort();
	void SetSkill(ESkillKey SkillKey, FName SkillName);
	void UseSkill(ESkillKey SkillKey);
	void EnhanceSkill(ESkillKey SkillKey, int32 EnhanceNumber);
	void PlaySkillAction(UAnimMontage* Montage);
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	TMap<ESkillKey, ASkillController*> SkillControllers;
	ESkillKey LastSkillKey;
};
