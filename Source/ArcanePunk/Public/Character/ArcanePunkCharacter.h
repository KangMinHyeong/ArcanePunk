// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameState/APGameState.h"
#include "Interfaces/InteractionInterface.h"
#include "ArcanePunkCharacter.generated.h"

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
};
//나중에 스킬도 uint8 또는 16으로 만들기


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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintPure)
	bool IsDead();

	UFUNCTION(BlueprintPure)
	bool IsHitting();

	UFUNCTION()
	void PlayerMontageEnded(UAnimMontage *Montage, bool bInterrupted);

	void Activate_Q();
	void Activate_E();
	void Cast_R(FVector HitLocation);

	void SetSkill_R(bool bValue);
	float GetR_LimitDist();

	void SetR_Location(FVector Vector);

	void MarkingOn(FVector Location, AActor* OtherActor);

	void SpawnFootPrint(bool LeftFoot); // 풋프린트 생성

	bool PMCheck(FHitResult& HitResult, FVector OverlapStart, FVector OverlapEnd); // 발 밑 메쉬의 피지컬 머터리얼 체크 // 그외에 캐릭터 근처 히트 체크

	class UArcanePunkCharacterAnimInstance* GetAnim(); // AnimInstance 반환
	
	//플레이어 스테이터스 관련 함수
	FPlayerData GetPlayerStatus(); // PlayerStatus 반환
	void SetPlayerStatus(FPlayerData NewPlayerData);

	UFUNCTION(BlueprintPure)
	uint8 returnState(); // CurrentCharacterState 반환

	// Attack 관련 함수
	class UAPAttackComponent* GetAttackComponent(); // AttackComp 반환
	float GetMaxDistance(); // 공격 사거리 반환
	float GetAttackRadius(); // 공격 반경 반환
	class UParticleSystem* GetComboHitEffect(); // 콤보어택 히트 파티클 반환;
	FVector GetComboHitEffectScale(); // 콤보어택 히트 스케일 반환;

	// Move 관련 함수
	class UAPMovementComponent* GetAPMoveComponent(); // APMoveComp 반환
	bool GetCanMove(); // bCanMove 반환;
	void SetCanMove(bool NewValue); // bCanMove 설정;
	float GetAttackMoveSpeed(int32 Section); // 콤보 어택 Section Speed 반환
	float GetPushCoefficient(); // 콤보 어택 시 Enemy Push 계수 반환
	
private:
	float DamageMath(float Damage);
	void InitPlayerStatus();
	
	void ZoomInOut(float AxisValue);
	
	void Skill_typeQ();
	void Skill_typeE();
	void Skill_typeR();
	
	void NormalState();
	void StunState();//후에 인자 추가 (상태시간)
	void KnockBackState();//후에 인자 추가 (상태시간)
	void SleepState();//후에 인자 추가 (상태시간)
	void SwitchState(uint8 Current);
	
	void OnHitting();
	void BindAttackCheck();
	void BindComboCheck();
	void DeActivate_Q();
	void SaveStatus();
	void CurrentPlayerLocation();

	// 몽타주 엔드 함수
	void OnAttack_A_MontageEnded();
	void OnAttack_B_MontageEnded();
	void OnSkill_Q_MontageEnded();
	void OnSkill_E_MontageEnded();
	void OnSkill_R_MontageEnded();

	void MarkErase();

	// Move 관련 함수
	void MoveForward(float AxisValue); // 위 아래 Move
	void MoveRight(float AxisValue); // 오른, 왼쪽 Move
	void StartJog(); // 빨리 달리기 시작
	void EndJog(); // 빨리 달리기 끝

	// 공격 관련 함수
	void Attack_typeA(); // 마우스 오른쪽 공격
	void Attack_typeB(); // 마우스 왼쪽 공격

private:
	// 부착 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Component")
	class UAPAttackComponent* AttackComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	class UAPMovementComponent* MoveComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	USkeletalMeshComponent* Weapon;

	// 카메라, 스프링암 변수
	UPROPERTY(EditAnywhere, Category = "Camera")
	class USpringArmComponent* MySpringArm;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* MyCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinimumSpringArmLength = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomCoefficient = 20.0f;

	float MaximumSpringArmLength = 0.0f;

	float CurrentArmLength = 0.0f;

	// 이동 관련 변수
	bool bCanMove = true;

	float DefaultSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Move")
	float AttackPushCoefficient = 1.2f;

	// TimerHandle 변수
	FTimerHandle State_ETimerHandle;
	FTimerHandle State_TimerHandle;
	FTimerHandle HitTimerHandle;
	FTimerHandle DeActivate_Q_TimerHandle;
	FTimerHandle MarkTimerHandle;

	// 캐릭터 상태 관련 변수
	UPROPERTY(EditAnywhere)
	float HitMotionTime = 1.0f;

	UPROPERTY(EditAnywhere)
	float State_Time = 3.0f;

	uint8 CurrentState = 0;

	float DefaultSlip = 0.0f;

	float StateTime = 3.0f;

	bool bHitting = false;

	UPROPERTY()
	class UArcanePunkCharacterAnimInstance* Anim;

	// State 관련 변수
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AArcanePunkPlayerState* MyPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	struct FPlayerData MyPlayerStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	struct FGameData MyGameStatus;

	// 스킬 관련 변수
	UPROPERTY(EditAnywhere, Category = "Skill")
	class UParticleSystemComponent* Q_Effect;

	UPROPERTY(EditAnywhere, Category = "Skill")
	class UParticleSystem* R_Effect;

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector R_Location;

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector R_Size = FVector(1,1,1);
	
	UPROPERTY(EditAnywhere, Category = "Skill")
	float R_LimitDist = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	class UMaterialInterface* R_Range_Decal;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASwordImpact> SwordImpactClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASwordThrowBase> SwordThrowClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float Q_ActiveTime = 7.0f;

	FVector MarkingLocation;

	AActor* MarkingActor;

	bool bMarking = false;

	bool bSkill_Q = false;
	bool bSkill_E = false;
	bool bSkill_R = false;

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
	class UParticleSystem* ComboHitEffect;

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

public:
	UPROPERTY(EditAnyWhere)
	float SwordTeleportTime = 5.0f;

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

	void DropItem(UAPItemBase* ItemToDrop, const int32 QuantityToDrop);
};
