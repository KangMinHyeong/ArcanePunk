
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameState/APGameState.h"
#include "PlayerState/APPlayerData.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "Components/Common/APBuffComponent.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Interfaces/InteractionInterface.h"
#include "ArcanePunkCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAutoRecoveryMPDelegate);

//Minhyeong
class UDataTable;
class UAPAttackComponent;
class UParticleSystem;
class UAPMovementComponent;
class UAPSkillHubComponent;
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
class USkillNumberBase;
class UAPHitPointComponent;
class AAPGameModeBase;
class UAPGhostTrailSpawnComponent;
class UAPSkillAbility;

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
	void InitComponent();
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	FTransform ReturnCameraTransform();
	FORCEINLINE USpringArmComponent* GetMySpringArm() {return MySpringArm;};

	FORCEINLINE AAPHUD* GetAPHUD() const {return HUD;};

	FORCEINLINE UMaterialInterface* GetDefaultMaterial() const {return DefaultMaterial;}; // DefaultMaterial 반환

	bool PMCheck(FHitResult& HitResult, FVector OverlapStart, FVector OverlapEnd); // 발 밑 메쉬의 피지컬 머터리얼 체크 // 그외에 캐릭터 근처 히트 체크
	
	FORCEINLINE void SetDoing(bool NewBool) {bDoing = NewBool;}; // bDoing 설정
	FORCEINLINE bool GetDoing() const {return bDoing;}; // bDoing 반환

	//Effect
	FORCEINLINE UNiagaraComponent* GetStunEffect() {return StunEffect;}; // StunEffect 반환

	//플레이어 스테이터스 관련 함수
	FORCEINLINE FPlayerTotalData GetPlayerStatus_Origin() const {return MyPlayerTotalStatus_Origin;}; // MyPlayerTotalStatus 반환
	FORCEINLINE void SetPlayerStatus_Origin(FPlayerTotalData NewPlayerData) {MyPlayerTotalStatus_Origin = NewPlayerData; UpdateStatus();}; // MyPlayerTotalStatus 설정 

	FORCEINLINE FPlayerTotalData GetPlayerStatus() const {return MyPlayerTotalStatus;}; // MyPlayerTotalStatus 반환
	FORCEINLINE void SetPlayerStatus(FPlayerTotalData NewPlayerData) {MyPlayerTotalStatus = NewPlayerData; UpdateStatus();}; // MyPlayerTotalStatus 설정 

	FORCEINLINE float GetDefaultSpeed() const {return MyPlayerTotalStatus_Origin.PlayerDynamicData.MoveSpeed;}; // DefaultSpeed 반환;
	FORCEINLINE float GetCurrentATK()  const {return FinalATK;}; // DefaultATK 반환
	FORCEINLINE float GetDefaultATK()  const {return MyPlayerTotalStatus.PlayerDynamicData.ATK;}; // DefaultATK 반환

	FORCEINLINE void SetDefaultATK(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.ATK = NewValue; UpdateStatus();}; // DefaultATK 설정
	FORCEINLINE void SetDefaultSpeed(float Speed) {MyPlayerTotalStatus.PlayerDynamicData.MoveSpeed = Speed; UpdateStatus();}; // DefaultSpeed 설정;
	FORCEINLINE void SetDefaultATKSpeed(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.ATKSpeed = NewValue; UpdateStatus();}; // DefaultATKSpeed 설정
	FORCEINLINE void SetDefaultHP(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.HP = NewValue; UpdateStatus();}; // DefaultHP 설정
	FORCEINLINE void SetDefaultMP(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.MP = NewValue; UpdateStatus();}; // DefaultHP 설정

	UFUNCTION(BlueprintPure)
	FORCEINLINE ECharacterState returnState() const {return CurrentState;}; // CurrentState 반환
	FORCEINLINE void SetState(ECharacterState UpdateState) { CurrentState = UpdateState;}; // CurrentState 설정

	void UpdateStatus();


	FORCEINLINE bool IsCriticalAttack() const {return bCriticalAttack;};
	// 크리티컬 데미지 계산
	float CriticalCalculate(float Multiple = 2.0f);

	// Attack 관련 함수
	FORCEINLINE UAPAttackComponent* GetAttackComponent() const {return AttackComp;};  // AttackComp 반환
	void SetAttackRotation(); // 마우스 커서 방향으로 플레이어 회전

	// Move 관련 함수
	FORCEINLINE UAPMovementComponent* GetAPMoveComponent() const {return MoveComp;}; // MoveComp 반환
	FORCEINLINE bool GetCanMove() const {return bCanMove;}; // bCanMove 반환;
	FORCEINLINE void SetCanMove(bool NewValue) {bCanMove = NewValue;}; // bCanMove 설정;
	FORCEINLINE float GetPushCoefficient() const {return AttackPushCoefficient;}; // 콤보 어택 시 Enemy Push 계수 반환
	FORCEINLINE void SetCanJog(bool NewValue) {bCanJog = NewValue;}; // bCanJog 설정;


	FORCEINLINE UAPSkillAbility* GetAPSkillAbility() const {return SkillAbility;}; // SkillComp 반환

	FORCEINLINE float GetSkillCancelTime() const {return SkillCancelTime;}; // SkillCancleTime 반환
	FORCEINLINE ESkillNumber GetQSkill() const {return QSkill;}; // QSkill 반환
	FORCEINLINE void SetQSkill(ESkillNumber NewSkill) {QSkill = NewSkill;}; // QSkill 설정
	FORCEINLINE ESkillNumber GetESkill() const {return ESkill;}; // ESkill 반환
	FORCEINLINE void SetESkill(ESkillNumber NewSkill) {ESkill = NewSkill;}; // ESkill 설정
	FORCEINLINE EUltSkillNumber GetRSkill() const {return RSkill;}; // RSkill 반환
	void SetRSkill(); // RSkill 설정

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAPSkillHubComponent* GetAPSkillHubComponent() const {return SkillHubComp;}; // SkillComp 반환

	FORCEINLINE UAPGhostTrailSpawnComponent* GetGhostTrailSpawnComp() const {return GhostTrailSpawnComp;}; // APGhostTrailSpawnComp 반환

	FORCEINLINE bool GetOnQSkill() const {return OnQSkill;};
	FORCEINLINE bool GetOnESkill() const {return OnESkill;};
	FORCEINLINE bool GetOnRSkill() const {return OnRSkill;};

	FORCEINLINE USkillNumberBase* GetQSkillNumber() const {return Skill_Q;}; // SkillNumComp 반환
	FORCEINLINE USkillNumberBase* GetESkillNumber() const {return Skill_E;}; // SkillNumComp 반환
	FORCEINLINE USkillNumberBase* GetRSkillNumber() const {return Skill_R;}; // SkillNumComp 반환

	FORCEINLINE void SetQSkillNumber(USkillNumberBase* NewObject) {Skill_Q = NewObject;}; // SkillNumComp 반환
	FORCEINLINE void SetESkillNumber(USkillNumberBase* NewObject) {Skill_E = NewObject;}; // SkillNumComp 반환
	FORCEINLINE void SetRSkillNumber(USkillNumberBase* NewObject) {Skill_R = NewObject;}; // SkillNumComp 반환

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
	FORCEINLINE UMaterialInterface* GetHideMaterial() const {return HideMaterial;};
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
	FORCEINLINE bool IsBlockMode() const {return bBlockMode;};
	FORCEINLINE uint8 GetReflectMode() const {return ReflectingModeGauge;};
	FORCEINLINE void SetReflectMode(uint8 NewValue) {ReflectingModeGauge = NewValue;};

	// Hit, Dead 관련 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAPTakeDamageComponent* GetTakeDamageComponent() const {return TakeDamageComp;}; // DamageComp 반환
	FORCEINLINE UAPHitPointComponent* GetHitPointComponent() const {return HitPointComp;}; // HitPointComp 반환
	
	UFUNCTION(BlueprintPure)
	bool IsDead(); // HP가 0이하인지 반환

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsHitting() const {return bHitting;};  // bHitting 반환
	FORCEINLINE void SetHitting(bool NewBool) {bHitting = NewBool;}; // bHitting 설정

	void DeadPenalty(float DeathTime); // DeadPenalty 발동

	//FootPrint 관련 함수
	FORCEINLINE UAPSpawnFootPrintComponent* GetSpawnFootPrintComponent() const {return APSpawnStepComp;}; // HitMotionTime 반환	
	FORCEINLINE UStaticMeshComponent* GetPlayerPanel() {return PlayerPanel;}; // PlayerPanel 반환
	FTransform GetFootTransform(bool Left);	
	
	// 장비 관련 함수
	USkeletalMeshComponent* GetPlayerEquipment(uint8 NewValue);
	UAPItemBase* GetEquipData(uint8 NewValue);
	void SetEquipData(uint8 NewValue, UAPItemBase* NewData);

	// CC 관련 함수
	FORCEINLINE UAPCrowdControlComponent* GetCrowdControlComponent() const {return CrowdControlComp;}; // CrowdControlComp 반환

	// Buff 관련 함수
	FORCEINLINE UAPBuffComponent* GetBuffComp() const {return BuffComp;}; // BuffComp 반환

	// Save 관련 함수
	void SaveStatus(FString PlayerSlotName, FString GameSlotName);

	// Interaction 관련 함수
	bool IsInteraction() const {return bInteract;};
	void SetInteraction(bool NewBool) {bInteract = NewBool;};

	void ActivateInteractionSweep();
	void InteractionActorRemove(AActor* InteractionActor);

	// DataTable
	FORCEINLINE UDataTable* GetSkillAbilityDataTable() const {return SkillAbilityDataTable;};

	FORCEINLINE UDataTable* GetSkillAbilityRowData() const {return SkillAbilityRowData;};
	FORCEINLINE UDataTable* GetSilverAbilityDataTable() const {return SilverAbilityDataTable;};
	FORCEINLINE UDataTable* GetGoldAbilityDataTable() const {return GoldAbilityDataTable;};
	FORCEINLINE UDataTable* GetPlatinumAbilityDataTable() const {return PlatinumAbilityDataTable;};
	
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

	// Skill 관련 함수
	void HideCheck();

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

	// 무기 위치 설정
	void SetWeaponPosition();
	
private:
	// 부착 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Component")
	UAPAttackComponent* AttackComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPMovementComponent* MoveComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSkillHubComponent* SkillHubComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPAnimHubComponent* AnimHubComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPTakeDamageComponent* TakeDamageComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSpawnFootPrintComponent* APSpawnStepComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPCrowdControlComponent* CrowdControlComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPBuffComponent* BuffComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPHitPointComponent* HitPointComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	USceneComponent* LeftBeamPoint;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPGhostTrailSpawnComponent* GhostTrailSpawnComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSkillAbility* SkillAbility;
	
	UPROPERTY(EditAnywhere, Category = "CC State")
	UNiagaraComponent* StunEffect;

	//PlayerController 변수
	TWeakObjectPtr<AArcanePunkPlayerController> PC;
	//GameMode 변수
	TWeakObjectPtr<AAPGameModeBase> GM;

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
	bool bCanMove = true;

	bool bCanJog = true;

	UPROPERTY(EditAnywhere, Category = "Move")
	float AttackPushCoefficient = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Move")
	float DashSpeed = 650.0f;

	// 캐릭터 상태 관련 변수
	bool bHitting = false;

	UPROPERTY(EditAnywhere)
	float State_Time = 3.0f;

	ECharacterState CurrentState = ECharacterState::None;

	float DefaultSlip = 0.0f;

	float StateTime = 3.0f;

	bool bDoing = false; // 공격 , 스킬 사용중인지 체크하는 변수 / true면 다른 행동 제약 


	// State 관련 변수
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AArcanePunkPlayerState* MyPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerTotalData MyPlayerTotalStatus_Origin; // 오리지널 스테이터스 (영구적 증가)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerTotalData MyPlayerTotalStatus; // 현재 스테이터스 (일시적 증가) [ex - 장비, 버프 등]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameData MyGameStatus;

	float FinalATK = 0.0f;

	bool bCriticalAttack = false;

	UPROPERTY()
	USkillNumberBase* Skill_Q;

	UPROPERTY()
	USkillNumberBase* Skill_E;

	UPROPERTY()
	USkillNumberBase* Skill_R;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float SkillCancelTime = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	ESkillNumber QSkill = ESkillNumber::Skill_1;

	UPROPERTY(EditAnywhere, Category = "Skill")
	ESkillNumber ESkill = ESkillNumber::Skill_2;

	UPROPERTY(EditAnywhere, Category = "Skill")
	EUltSkillNumber RSkill = EUltSkillNumber::UltSkill_1;

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
	UPROPERTY(EditAnywhere, Category = "Skill")
	UMaterialInterface* HideMaterial;
	
	bool OnQSkill = false;

	bool OnESkill = false;

	bool OnRSkill = false;

	bool bCanSkill_Q = true;  // Q쿨타임 체크
	bool bCanSkill_E = true;  // E쿨타임 체크
	bool bCanSkill_R = true;  // R쿨타임 체크

	// 무적기 관련 변수
	FTimerHandle BlockTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Super Stance")
	float BlockTime = 1.5f;

	bool bBlockMode = false;

	uint8 ReflectingModeGauge = 0;

	// Foot Print 변수
	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UStaticMeshComponent* FootPrint_L;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UStaticMeshComponent* FootPrint_R;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	UStaticMeshComponent* PlayerPanel;

	// 머터리얼
	UMaterialInterface* DefaultMaterial;

	// 드랍
	UPROPERTY(EditAnywhere, Category = "Drop")
	TSubclassOf<APickup> DropClass;

	// Interaction
	bool bInteract = false;

	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractionRadius = 60.0f;
	
	UPROPERTY()
	TArray<AActor*> InteractionActors;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbilityDataTable;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbilityRowData;
	UPROPERTY(EditAnywhere)
	UDataTable* SilverAbilityDataTable;
	UPROPERTY(EditAnywhere)
	UDataTable* GoldAbilityDataTable;
	UPROPERTY(EditAnywhere)
	UDataTable* PlatinumAbilityDataTable;


public:
	TArray<uint8> MouseArr;

	TArray<bool> StopState;

	TArray<ESkillNumber> HavingSkill;

	FOnAutoRecoveryMPDelegate OnAutoRecoveryMPDelegate;

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
