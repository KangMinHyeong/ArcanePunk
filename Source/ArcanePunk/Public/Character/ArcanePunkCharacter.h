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

	UFUNCTION(BlueprintPure)
	uint8 returnState();

	FTransform ReturnCameraTransform();

	void SetCanMove(bool NewValue);

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
	FPlayerData GetPlayerStatus();

	void MarkingOn(FVector Location, AActor* OtherActor);

	void AnimMovement();
	void AnimMoveStop();

	void SpawnFootPrint(bool LeftFoot);

	class UAPAttackComponent* GetAttackComponent(); // AttackComp 반환

	struct FPlayerData GetMyPlayerStatus(); // PlayerStatus 반환

	uint8 GetCurrentCharacterState(); // CurrentCharacterState 반환

	class UArcanePunkCharacterAnimInstance* GetAnim(); // AnimInstance 반환
private:
	float DamageMath(float Damage);
	void InitPlayerStatus();
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void ZoomInOut(float AxisValue);
	void Attack_typeA();
	void Attack_typeB();
	void Skill_typeQ();
	void Skill_typeE();
	void Skill_typeR();
	void StartJog();
	void EndJog();
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

	void OnAttack_A_MontageEnded();
	void OnAttack_B_MontageEnded();
	void OnSkill_Q_MontageEnded();
	void OnSkill_E_MontageEnded();
	void OnSkill_R_MontageEnded();

	void MarkErase();

	bool PMCheck(FHitResult& HitResult, FVector OverlapStart, FVector OverlapEnd);

	void AttackMoving(float DeltaTime);
private:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* MySpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* MyCamera;

	UPROPERTY(EditAnywhere)
	class UAPAttackComponent* AttackComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FootPrint_L;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FootPrint_R;

	float MaximumSpringArmLength = 0.0f;

	float CurrentArmLength = 0.0f;

	UPROPERTY(EditAnywhere)
	float MinimumSpringArmLength = 150.0f;

	UPROPERTY(EditAnywhere)
	float ZoomCoefficient = 20.0f;

	FVector PlayerVec = FVector::ZeroVector;

	bool bCanMove = true;

	bool bSkill_Q = false;
	bool bSkill_E = false;
	bool bSkill_R = false;

	FTimerHandle State_ETimerHandle;
	FTimerHandle State_TimerHandle;
	FTimerHandle HitTimerHandle;
	FTimerHandle DeActivate_Q_TimerHandle;
	FTimerHandle MarkTimerHandle;

	UPROPERTY(EditAnywhere)
	float HitMotionTime = 1.0f;

	UPROPERTY(EditAnywhere)
	float State_Time = 3.0f;

	UPROPERTY(EditAnywhere)
	float Q_ActiveTime = 7.0f;

	UPROPERTY(EditAnywhere)

	float JoggingSpeed = 700.0f;

	float DefaultSpeed = 400.0f;

	uint8 CurrentState = 0;

	float DefaultSlip = 0.0f;

	uint8 CurrentCharacterState = 0;

	float StateTime = 3.0f;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AArcanePunkPlayerState* MyPlayerState;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Weapon;

	bool bHitting = false;

	UPROPERTY()
	class UArcanePunkCharacterAnimInstance* Anim;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* Q_Effect;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* E_Effect;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* R_Effect;

	UPROPERTY(EditAnywhere)
	FVector E_Size = FVector(1,1,1);

	UPROPERTY(EditAnywhere)
	FVector R_Location;

	UPROPERTY(EditAnywhere)
	FVector R_Size = FVector(1,1,1);
	
	UPROPERTY(EditAnywhere)
	class USoundBase* E_Sound_first;

	UPROPERTY(EditAnywhere)
	class USoundBase* E_Sound;

	UPROPERTY(EditAnywhere)
	class USoundBase* Q_Sound;

	UPROPERTY(EditAnywhere)
	class USoundBase* Attack_Sound;

	UPROPERTY(EditAnywhere)
	class USoundBase* R_Sound_Cast;

	UPROPERTY(EditAnywhere)
	float E_SoundScale = 1.0f;

	UPROPERTY(EditAnywhere)
	float Q_SoundScale = 1.0f;

	UPROPERTY(EditAnywhere)
	float R_LimitDist = 1500.0f;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* R_Range_Decal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	struct FPlayerData MyPlayerStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	struct FGameData MyGameStatus;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<class ASwordImpact> SwordImpactClass;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<class ASwordThrowBase> SwordThrowClass;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<AActor> LeftFootClass;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<AActor> RightFootClass;

	bool bMarking = false;

	FVector MarkingLocation;

	AActor* MarkingActor;

	UPROPERTY(EditAnyWhere)
	float Attack1_MoveSpeed = 500.0f;

	UPROPERTY(EditAnyWhere)
	float Attack2_MoveSpeed = 500.0f;

	UPROPERTY(EditAnyWhere)
	float Attack3_MoveSpeed = 500.0f;

	bool AttackMove = false;

	UPROPERTY(EditAnyWhere)
	float Customfloat = 75.0f;

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
