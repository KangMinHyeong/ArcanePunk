// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Interfaces/InteractionInterface.h"
#include "ArcanePunkCharacter.generated.h"

//Minhyeong
class UDataTable;
class UAPAttackComponent;
class UParticleSystem;
class UAPMovementComponent;
class UAPSkillHubComponent;
class UAPSkillNumber;
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

enum class ECharacterState : uint8
{
    None        = 0,
    Stun        = 1,
    KnockBack 	= 2,
    Sleep       = 3,
	Slow		= 4,
};
//나중에 스킬도 uint8 또는 16으로 만들기

enum class ESkillState : uint8
{
	Base_None	= 0,
    Base_Q      = 1,
    Base_E      = 2,
    Base_Shift 	= 3,
    Base_Space	= 4,
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

	//플레이어 스테이터스 관련 함수
	FORCEINLINE FPlayerData GetPlayerStatus() const {return MyPlayerStatus;}; // PlayerStatus 반환
	FORCEINLINE void SetPlayerStatus(FPlayerData NewPlayerData) {MyPlayerStatus = NewPlayerData;}; // PlayerStatus 설정 

	UFUNCTION(BlueprintPure)
	FORCEINLINE uint8 returnState() const {return CurrentState;}; // CurrentCharacterState 반환

	void UpdateStatus();

	FORCEINLINE float GetFinalATK() const {return FinalATK;}; // FinalATK 반환

	// Attack 관련 함수
	FORCEINLINE UAPAttackComponent* GetAttackComponent() const {return AttackComp;};  // AttackComp 반환
	FORCEINLINE float GetMaxDistance() const {return MaxDistance;}; // 공격 사거리 반환
	FORCEINLINE float GetAttackRadius() const {return AttackRadius;}; // 공격 반경 반환
	FORCEINLINE UParticleSystem* GetComboHitEffect() const {return ComboHitEffect;}; // 콤보어택 히트 파티클 반환;
	FORCEINLINE FVector GetComboHitEffectScale() const {return HitEffectScale;}; // 콤보어택 히트 스케일 반환;
	FORCEINLINE void SetbMouseAttack(bool NewValue) {bMouseAttack = NewValue;}; // bMouseAttack 설정 

	// Move 관련 함수
	FORCEINLINE UAPMovementComponent* GetAPMoveComponent() const {return MoveComp;}; // MoveComp 반환
	FORCEINLINE bool GetCanMove() const {return bCanMove;}; // bCanMove 반환;
	FORCEINLINE void SetCanMove(bool NewValue) {bCanMove = NewValue;}; // bCanMove 설정;
	float GetAttackMoveSpeed(int32 Section);// 콤보 어택 Section Speed 반환
	FORCEINLINE float GetPushCoefficient() const {return AttackPushCoefficient;}; // 콤보 어택 시 Enemy Push 계수 반환
	
	// Skill 관련 함수
	UFUNCTION(BlueprintPure)
	FORCEINLINE UAPSkillHubComponent* GetAPSkillHubComponent() const {return SkillComp;}; // SkillComp 반환
	FORCEINLINE UAPSkillNumber* GetAPSkillNumberComponent() const {return SkillNumberComp;}; // SkillNumberComp 반환
	FORCEINLINE TSubclassOf<ASwordImpact> GetSwordImpactClass() const {return SwordImpactClass;}; //SwordImpactClass 반환
	FORCEINLINE TSubclassOf<ASwordThrowBase> GetSwordThrowClass() const {return SwordThrowClass;}; // SwordThrowClass 반환

	FORCEINLINE float GetSkill3_LimitDist() const {return Skill3_LimitDist;}; // Skill3_LimitDist 반환
	FORCEINLINE void SetSkill3_Location(FVector Vector) {Skill3_Location = Vector;};// Skill3 위치 설정

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

	//FootPrint 관련 함수
	FORCEINLINE UAPSpawnFootPrintComponent* GetSpawnFootPrintComponent() const {return SpawnFootPrintComp;}; // HitMotionTime 반환

	TSubclassOf<AActor> GetFootClass(bool Left);
	FTransform GetFootTransform(bool Left);

	// 장비 관련 함수
	USkeletalMeshComponent* GetPlayerEquipment(uint8 NewValue);
	UAPItemBase* GetEquipData(uint8 NewValue);
	void SetEquipData(uint8 NewValue, UAPItemBase* NewData);

	// 플레이어 상태이상 함수
	void StunState(float StunTime);//후에 인자 추가 (상태시간)
	void KnockBackState(FVector KnockBackPoint, float KnockBackTime);//후에 인자 추가 (상태시간)
	void SleepState();//후에 인자 추가 (상태시간)
	void SlowState(float SlowCoefficient, float SlowTime);

private:
	void InitPlayerStatus();
	
	void ZoomInOut(float AxisValue);
	
	void Skill_typeQ();
	void Skill_typeE();
	void Skill_typeSpace();
	
	void NormalState();
	void SwitchState(uint8 Current);
	
	void SaveStatus();
	void CurrentPlayerLocation();

	// Move 관련 함수
	void MoveForward(float AxisValue); // 위 아래 Move
	void MoveRight(float AxisValue); // 오른, 왼쪽 Move
	void Skill_typeShift(); // 빨리 달리기 시작
	void EndJog(); // 빨리 달리기 끝

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
	UAPSkillHubComponent* SkillComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSkillNumber* SkillNumberComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPAnimHubComponent* AnimHubComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPTakeDamageComponent* TakeDMComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSpawnFootPrintComponent* SpawnFootPrintComp;

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
	bool bCanMove = true;

	float DefaultSpeed = 400.0f;
	UPROPERTY()
	bool bCanJog = true;

	UPROPERTY(EditAnywhere, Category = "Move")
	float AttackPushCoefficient = 1.2f;

	// TimerHandle 변수
	FTimerHandle State_ETimerHandle;
	FTimerHandle State_TimerHandle;

	// 캐릭터 상태 관련 변수
	UPROPERTY(EditAnywhere)
	float HitMotionTime = 1.0f;
	bool bHitting = false;

	UPROPERTY(EditAnywhere)
	float State_Time = 3.0f;

	uint8 CurrentState = 0;

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
	FPlayerData MyPlayerStatus;

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

	bool bMouseAttack = true;

	// 머터리얼
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial;

	// 드랍
	UPROPERTY(EditAnywhere, Category = "Drop")
	TSubclassOf<APickup> DropClass;

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	UParticleSystem* Skill3_Effect;

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector Skill3_Location;

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector Skill3_Size = FVector(1,1,1);

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial_Test1; // Test용 후에 삭제

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial_Test2; // Test용 후에 삭제

	UPROPERTY()
	TArray<uint8> MouseArr;
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
};
