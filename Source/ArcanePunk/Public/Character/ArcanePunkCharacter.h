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

	UMaterialInterface* GetHitMaterial(); // HitMaterial 반환
	UMaterialInterface* GetDefaultMaterial(); // DefaultMaterial 반환

	bool PMCheck(FHitResult& HitResult, FVector OverlapStart, FVector OverlapEnd); // 발 밑 메쉬의 피지컬 머터리얼 체크 // 그외에 캐릭터 근처 히트 체크
	
	void SetDoing(bool NewBool); // bDoing 설정
	bool GetDoing(); // bDoing 반환

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
	void SetbMouseAttack(bool NewValue); // bMouseAttack 설정 

	// Move 관련 함수
	class UAPMovementComponent* GetAPMoveComponent(); // MoveComp 반환
	bool GetCanMove(); // bCanMove 반환;
	void SetCanMove(bool NewValue); // bCanMove 설정;
	float GetAttackMoveSpeed(int32 Section); // 콤보 어택 Section Speed 반환
	float GetPushCoefficient(); // 콤보 어택 시 Enemy Push 계수 반환
	
	// Skill 관련 함수
	UFUNCTION(BlueprintPure)
	class UAPSkillHubComponent* GetAPSkillHubComponent(); // SkillComp 반환
	
	class UAPSkillNumber* GetAPSkillNumberComponent(); // SkillNumberComp 반환
	TSubclassOf<class ASwordImpact> GetSwordImpactClass(); //SwordImpactClass 반환
	TSubclassOf<class ASwordThrowBase> GetSwordThrowClass(); // SwordThrowClass 반환

	float GetSkill3_LimitDist(); // Skill3_LimitDist 반환
	void SetSkill3_Location(FVector Vector); // Skill3 위치 설정

	// Hit, Dead 관련 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintPure)
	bool IsDead(); // HP가 0이하인지 반환

	UFUNCTION(BlueprintPure)
	bool IsHitting(); // bHitting 반환
	void SetHitting(bool NewBool); // bHitting 설정
	float GetHitMotionTime(); // HitMotionTime 반환

	//FootPrint 관련 함수
	class UAPSpawnFootPrintComponent* GetSpawnFootPrintComponent();

	TSubclassOf<AActor> GetFootClass(bool Left);
	FTransform GetFootTransform(bool Left);

private:
	void InitPlayerStatus();
	
	void ZoomInOut(float AxisValue);
	
	void Skill_typeQ();
	void Skill_typeE();
	void Skill_typeSpace();
	
	void NormalState();
	void StunState();//후에 인자 추가 (상태시간)
	void KnockBackState();//후에 인자 추가 (상태시간)
	void SleepState();//후에 인자 추가 (상태시간)
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

private:
	// 부착 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Component")
	class UAPAttackComponent* AttackComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	class UAPMovementComponent* MoveComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	class UAPSkillHubComponent* SkillComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	class UAPSkillNumber* SkillNumberComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	class UAPAnimHubComponent* AnimHubComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	class UAPTakeDamageComponent* TakeDamageComp;

	UPROPERTY(EditAnywhere, Category = "Component")
	class UAPSpawnFootPrintComponent* SpawnFootPrintComp;
	
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
	float Skill3_LimitDist = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASwordImpact> SwordImpactClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASwordThrowBase> SwordThrowClass;

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

	bool bMouseAttack = true;

	// 머터리얼
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial;

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	class UParticleSystem* Skill3_Effect;

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector Skill3_Location;

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector Skill3_Size = FVector(1,1,1);

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
