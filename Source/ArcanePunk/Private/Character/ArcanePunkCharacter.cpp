// Fill out your copyright notice in the Description page of Project Settings.

// Minhyeong
#include "Character/ArcanePunkCharacter.h"

#include "Components/Character/APSpringArmComponent.h"
#include "Components/Character/APCameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Controller.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Save/APSaveGame.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APAnimHubComponent.h"
#include "Components/Character/APSpawnFootPrintComponent.h"
#include "PlayerState/ArcanePunkPlayerState.h"
#include "GameMode/APGameModeBase.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Components/CapsuleComponent.h"
#include "Components/Common/APGhostTrailSpawnComponent.h"
#include "Components/SkillActor/APSkillAbility.h"
#include "Components/Character/APPassiveComponent.h"
#include "GameInstance/APGameInstance.h"
#include "Components/Character/APFadeOutTriggerComponent.h"
#include "Components/Character/APCharacterAuraComponent.h"

// prodo
#include "DrawDebugHelpers.h"
#include "kismet/KismetSystemLibrary.h"
#include "Items/APItemBase.h"
#include "UserInterface/HUD/APHUD.h"
#include "Components/Character/APInventoryComponent.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Test/Pickup.h"
#include "UserInterface/Tutorial/APTuTorialUserWidget.h"
#include "Skill/SkillController.h"

//YS
#include "Logging/StructuredLog.h"
#include "MotionWarpingComponent.h"
#include "GameInstance/Subsystem/APTimeManipulationSubsystem.h"

DEFINE_LOG_CATEGORY(LogCharacter)

// Minhyeong
AArcanePunkCharacter::AArcanePunkCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	APSpringArm = CreateDefaultSubobject<UAPSpringArmComponent>(TEXT("APSpringArm"));
	APCamera = CreateDefaultSubobject<UAPCameraComponent>(TEXT("APCamera"));
	APSpringArm_Back = CreateDefaultSubobject<UAPSpringArmComponent>(TEXT("APSpringArm_Back"));
	APCamera_Back = CreateDefaultSubobject<UAPCameraComponent>(TEXT("APCamera_Back"));

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	FootPrint_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootPrint_L"));
	FootPrint_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootPrint_R"));
	LeftBeamPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftBeamPoint"));
	FadeOutTigger = CreateDefaultSubobject<UAPFadeOutTriggerComponent>(TEXT("FadeOutTigger"));

	SkillHubComponent = CreateDefaultSubobject<UAPSkillHubComponent>(TEXT("SkillHubComponent"));
	APSpawnStepComp = CreateDefaultSubobject<UAPSpawnFootPrintComponent>(TEXT("APSpawnStepComp"));
	BuffComp = CreateDefaultSubobject<UAPBuffComponent>(TEXT("BuffComp"));
	SkillAbility = CreateDefaultSubobject<UAPSkillAbility>(TEXT("SkillAbility"));
	GhostTrailSpawnComp = CreateDefaultSubobject<UAPGhostTrailSpawnComponent>(TEXT("GhostTrailSpawnComp"));
	PassiveComp = CreateDefaultSubobject<UAPPassiveComponent>(TEXT("PassiveComp"));
	CharacterAura = CreateDefaultSubobject<UAPCharacterAuraComponent>(TEXT("CharacterAura"));

	APSpringArm->SetupAttachment(GetRootComponent());
	APCamera->SetupAttachment(APSpringArm);
	APSpringArm_Back->SetupAttachment(GetRootComponent());
	APCamera_Back->SetupAttachment(APSpringArm_Back);

	FadeOutTigger->SetupAttachment(APSpringArm);
	Weapon->SetupAttachment(GetMesh(),FName("Bip001-Prop1"));
	FootPrint_L->SetupAttachment(GetMesh(), FName("FootPrint_L"));
	FootPrint_R->SetupAttachment(GetMesh(), FName("FootPrint_R"));
	LeftBeamPoint->SetupAttachment(GetMesh(), FName("BeamPoint"));

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// prodo
	PlayerInventory = CreateDefaultSubobject<UAPInventoryComponent>(TEXT("PlayerInventory"));

	// set capacity of inventory
	PlayerInventory->SetSlotsCapacity(15);
	PlayerInventory->SetWeightCapacity(50.0f);

	// ys
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Component"));
}

void AArcanePunkCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitPlayer();
}

void AArcanePunkCharacter::InitPlayer()
{
	PC = Cast<AArcanePunkPlayerController>(GetController());
	if(PC.IsValid()) HUD = Cast<AAPHUD>(PC->GetHUD());
	
	GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	InitComponent();
	BuffComp->BindBuffComp();
	
	InitPlayerStatus();
	InitEquipData(WeaponReference, DesiredWeaponID);
	UpdateStatus();

	InitSkill();

	SetHavingSkills();

	PassiveComp->InitPassive();

	SetWeaponPosition();
}

void AArcanePunkCharacter::InitSkill()
{
	// TODO: 임시 초기 스킬 설정(테이블의 첫 내용들로 사용) => 추후 설정 방식 지정 필요
	auto SkillDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/Skill/SkillDataTable.SkillDataTable"));
	if (SkillDataTable)
	{
		// 데이터 테이블에서 행의 키 값(스킬 이름)들을 가져옴
		TArray<FName> RowNames = SkillDataTable->GetRowNames();

		int numToIterate = std::min(RowNames.Num(), 3);
		for(int i = 0; i < numToIterate; i++)
		{
			int enumSkillKey = static_cast<int>(ESkillKey::Q) + i;
			SetSkill(static_cast<ESkillKey>(enumSkillKey), RowNames[i]);
		}
	}
}

void AArcanePunkCharacter::ChangePlayer(AArcanePunkPlayerController* NewController)
{
	PlayerState = EPlayerState::Spawn;
	ApperenceTarget = 1.0f;
	
	PC = Cast<AArcanePunkPlayerController>(NewController);
	if(PC.IsValid()) HUD = Cast<AAPHUD>(PC->GetHUD());
	
	for(auto SkillController : SkillControllers)
	{
		SkillController.Value->UpdateSkillSlotImage();
	}
	
	if(HUD)
	{
		HUD->GetStatusWidget(this)->SetMaxHP(TotalStatus_Origin.StatusData.MaxHP);
		HUD->GetStatusWidget(this)->SetHPPercent(this, TotalStatus_Origin.StatusData.HP);
	}	
}

void AArcanePunkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bMainPlayer && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) 
	{
		DissolveCharacterMesh(DeltaTime);
	}
}

void AArcanePunkCharacter::DissolveCharacterMesh(float DeltaTime)
{
	Super::DissolveCharacterMesh(DeltaTime);

	if(Apperence <= 0.2f)
	{
		SetActorTickEnabled(false);
		SetActorHiddenInGame(true);  // 화면에서 숨김
	}
}

void AArcanePunkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindAxis(TEXT("MoveForward"), this, &AArcanePunkCharacter::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AArcanePunkCharacter::MoveRight);

	InputComponent->BindAxis(TEXT("ZoomInOut"), this, &AArcanePunkCharacter::ZoomInOut);

	InputComponent->BindAction(TEXT("ComboAttack"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::ComboAttack); // 연속 공격
	InputComponent->BindAction(TEXT("Parrying"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Parrying); // 차징 공격

	DECLARE_DELEGATE_OneParam(FSkillInputDelegate, const ESkillKey);
	InputComponent->BindAction<FSkillInputDelegate>(TEXT("Skill_Q"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::UseSkill, ESkillKey::Q);
	InputComponent->BindAction<FSkillInputDelegate>(TEXT("Skill_E"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::UseSkill, ESkillKey::E);
	InputComponent->BindAction<FSkillInputDelegate>(TEXT("Skill_R"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::UseSkill, ESkillKey::R);

	InputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::PressedDash);

	InputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::OnBlockMode);
	
	InputComponent->BindAction(TEXT("WorldMap"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::WorldMap);

	InputComponent->BindAction(TEXT("Alt_RightClick"), EInputEvent::IE_Pressed, this, &AArcanePunkCharacter::Alt_RightClick);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AArcanePunkCharacter::BeginInteract);
	// PlayerInputComponent->BindAction("Interact", IE_Released, this, &AArcanePunkCharacter::EndInteract);
	InputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AArcanePunkCharacter::ToggleMenu);
}

void AArcanePunkCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimHubComponent->BindAttackCheck();
	AnimHubComponent->BindComboCheck();
}

void AArcanePunkCharacter::InitComponent()
{
	// 컴포넌트 초기화
	AttackComponent->InitAttackComp();
}

bool AArcanePunkCharacter::CheckSwapPlayerCondition()
{
	if(bDoing) return false;

    return true;
}

void AArcanePunkCharacter::SwapMainPlayer(bool NewBool, bool bEnhanceSwap, bool bPrioritySkilling)
{
	bMainPlayer = NewBool;
	CharacterAura->SetAuraActive(bMainPlayer);

	if(bMainPlayer)
	{		
		if(bPrioritySkilling)
		{
			PlayerState = EPlayerState::EntryMode_Only;
		}
		if(bEnhanceSwap)
		{
			PlayerState = EPlayerState::EntryMode;
		}
	}
	else
	{
		if(bEnhanceSwap)
		{
			PlayerState = EPlayerState::ExitSkillPlaying;			
		}
		else
		{
			PlayerState = EPlayerState::Inactive;
		}
	}

	SwitchPlayerState();
}

void AArcanePunkCharacter::SwitchPlayerState()
{
	switch (PlayerState)
	{
	case EPlayerState::Idle:
		DissolveCharacterMesh_Immediate(false);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		break;

	case EPlayerState::Spawn:
		PlayerState = EPlayerState::Idle;
		SwitchPlayerState();
		break;
	
	case EPlayerState::EntryMode_Only:
		PlaySwapDash();
		MoveComponent->StartLookAtEnemy();
		break;	

	case EPlayerState::EntryMode:
		PlaySwapDash();
		MoveComponent->StartLookAtEnemy();
		break;

	case EPlayerState::ExitSkillPlaying:
		UseSwapSkill_Exit();	
		break;
	
	case EPlayerState::EntrySkillPlaying:
		UseSwapSkill_Entry();	
		break;

	case EPlayerState::Inactive:
		ApperenceTarget = 0.0f;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		break;	
	}
}

void AArcanePunkCharacter::UpdateSwapGauge(float Add)
{
	if(HUD)
	{
		SwapGauge = HUD->UpdateSwapGauge(PlayerIndex, Add);
	}
}

void AArcanePunkCharacter::MoveForward(float AxisValue)
{
	if(!bMainPlayer) return;

	FVector2D PlayerVec;
	PlayerVec.X = AxisValue;

	if(StopState.IsEmpty()) MoveComponent->PlayerMoveForward(AxisValue);
}

void AArcanePunkCharacter::MoveRight(float AxisValue)
{
	if(!bMainPlayer) return;

	FVector2D PlayerVec;
	PlayerVec.Y = AxisValue;

	if(StopState.IsEmpty()) MoveComponent->PlayerMoveRight(AxisValue);
}

void AArcanePunkCharacter::ZoomInOut(float AxisValue)
{
	if (AxisValue < -0.01f || AxisValue > 0.01f)
	APSpringArm->ZoomImmediate(AxisValue * WheelZoomCoefficient);
}

void AArcanePunkCharacter::SetAttackRotation(float AddSpeed)
{
	if(PC.IsValid()) 
	{
		if(!GM.IsValid()) return; 
		// Y축 기준
		// if(GM->IsBattleStage()) 
		// MoveComponent->SetAttackRotation(FRotator(GetActorRotation().Pitch, PC.Get()->MousePositionAngle, GetActorRotation().Roll ));

		// X축 기준
		if(GM->IsBattleStage()) 
		MoveComponent->SetAttackRotation(FRotator(GetActorRotation().Pitch, -90.0f + PC.Get()->MousePositionAngle, GetActorRotation().Roll ));
	}
}

void AArcanePunkCharacter::ComboAttack() 
{
	if(!bMainPlayer || !StopState.IsEmpty()) return;
	
	if(!OnLeftMouseClick.IsBound()) {AttackComponent->StartComboAttack(AttackCancelTime);}
	else {OnLeftMouseClick.Broadcast();}

}

void AArcanePunkCharacter::Parrying()
{
	if(!bMainPlayer || !StopState.IsEmpty() || !bCanParrying) return;

	// SkillHubComponent->LastSkill = ESkillKey::None;
	AttackComponent->StartParrying();
}

void AArcanePunkCharacter::InitEquipData(TArray<UAPItemBase *> & EquipArr, FName EquipID)
{
	//후에 세이브 데이터 생기면 로직 추가해야할 듯
	if(EquipArr.IsEmpty())
	{
		if (!EquipID.IsNone())
		{
   			auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return; 
			const FItemData* ItemData = DataTableGI->GetEquipDataTable()->FindRow<FItemData>(EquipID, EquipID.ToString());

			auto ItemReference = NewObject<UAPItemBase>(this, UAPItemBase::StaticClass());

			ItemReference->ID = ItemData->ID;
			ItemReference->ItemType = ItemData->ItemType;
			ItemReference->ItemQuality = ItemData->ItemQuality;
			ItemReference->ItemNumericData = ItemData->ItemNumericData;
			ItemReference->ItemTextData = ItemData->ItemTextData;
			ItemReference->ItemAssetData = ItemData->ItemAssetData;
			ItemReference->ItemStatistics = ItemData->ItemStatistics;

			EquipArr.Add(ItemReference);
		}
	}
	
}

void AArcanePunkCharacter::ChangeEquipData(TArray<UAPItemBase *> & EquipArr, UAPItemBase *NewData)
{
	// 현재는 테스트용 나중에 강화 등 캐릭터 스테이터스에 변화를 고려하여 수정
	if(EquipArr.IsEmpty())
	{
		if(NewData) EquipArr.Add(NewData);
	}
	else
	{
		EquipArr.Empty();
		if(NewData) EquipArr.Add(NewData);;
	}
	
	UpdateStatus();
	if(PC.IsValid()) PC->OnUpdateStatusText.Broadcast();
}

void AArcanePunkCharacter::SetWeaponPosition()
{
	if(!GM.IsValid() || !Weapon) return;

	if(GM->IsBattleStage())
	{
		Weapon->SetHiddenInGame(false);
	}
	else
	{
		Weapon->SetHiddenInGame(true);
	}
}

/*
*	@breif: 대시/회피
*	@writer: YS
*/
void AArcanePunkCharacter::PressedDash()
{
	//@입력 유효성 검사
	if (!bMainPlayer)
	{
		UE_LOGFMT(LogCharacter, Log, "메인 플레이어가 아니므로 대시를 실행하지 않습니다.");
		return;
	}

	//@움직임 가능성 여부
	if (!bCanMove)
	{
		UE_LOGFMT(LogCharacter, Log, "이동이 불가능한 상태이므로 대시를 실행하지 않습니다.");
		return;
	}

	//@멈춤 상태?
	if (!StopState.IsEmpty())
	{
		UE_LOGFMT(LogCharacter, Log, "중단 상태가 있으므로 대시를 실행하지 않습니다.");
		return;
	}

	//@죽음 여부
	if (IsDead())
	{
		UE_LOGFMT(LogCharacter, Log, "캐릭터가 사망 상태이므로 대시를 실행하지 않습니다.");
		return;
	}

	//@행동 상태 검사
	if (bDoing && !AttackComponent->IsComboAttack() && !AttackComponent->IsParrying())
	{
		UE_LOGFMT(LogCharacter, Log, "다른 행동 중이고 콤보 공격이나 패링 중이 아니므로 대시를 실행하지 않습니다.");
		return;
	}

	//@대시 가능 여부 확인
	if (!bCanDash)
	{
		UE_LOGFMT(LogCharacter, Log, "대시가 불가능한 상태입니다. 쿨다운 중일 수 있습니다.");
		return;
	}

	//@대시 실행 상태 설정
	bDoing = true;
	bCanDash = false;
	bCanMove = false;

	UE_LOGFMT(LogCharacter, Log, "캐릭터 대시를 시작합니다.");

	//@충돌 응답 변경
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	//@대시 관련 컴포넌트 설정
	MoveComponent->StartDash();
	GhostTrailSpawnComp->SetRunTrail(true);

	//@HUD
	if (!HUD)
	{
		UE_LOGFMT(LogCharacter, Warning, "HUD가 유효하지 않아 대시 타이머를 업데이트할 수 없습니다.");
		return;
	}

	//@Dash 타이머 시작
	HUD->GetStatusWidget(this)->StartDashTime(MoveComponent->GetDashTime());
	UE_LOGFMT(LogCharacter, Log, "HUD 대시 타이머를 {0}초로 설정했습니다.", MoveComponent->GetDashTime());
}

void AArcanePunkCharacter::ReleasedDash()
{
	//@입력 유효성 검사
	if (!bMainPlayer)
	{
		UE_LOGFMT(LogCharacter, Log, "메인 플레이어가 아니므로 대시 해제를 처리하지 않습니다.");
		return;
	}

	//@죽음 여부 
	if (IsDead())
	{
		UE_LOGFMT(LogCharacter, Log, "캐릭터가 사망 상태이므로 대시 해제를 처리하지 않습니다.");
		return;
	}

	//@캐릭터 상태 복원
	bDoing = false;
	bCanMove = true;

	//@공격 상태 초기화
	AttackComponent->SetComboAttack(false);
	AttackComponent->SetParrying(false);

	//@충돌 응답 복원
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);

	//@대시 관련 컴포넌트 설정 해제
	//MoveComponent->EndDash();
	GhostTrailSpawnComp->SetRunTrail(false);

	//@HUD
	if (HUD)
	{
		UE_LOGFMT(LogCharacter, Warning, "HUD가 유효하지 않아 대시 쿨타임을 설정할 수 없습니다.");
	}

	//@대시 쿨타임 설정
	HUD->GetStatusWidget(this)->StartCoolTimeSlot(ESkillKey::Dash, DashCoolTime);
	UE_LOGFMT(LogCharacter, Log, "대시 쿨타임을 {0}초로 설정했습니다.", DashCoolTime);

	UE_LOGFMT(LogCharacter, Log, "캐릭터 대시를 종료합니다.");
}

void AArcanePunkCharacter::PlaySwapDash()
{
	bDoing = true; bCanMove = false;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	MoveComponent->PlaySwapDash();
	GhostTrailSpawnComp->SetSkillTrail(true);

	GetWorldTimerManager().SetTimer(SwapDashTimerHandle, this, &AArcanePunkCharacter::EndSwapDash, MoveComponent->GetSwapDashTime(), false);
}

void AArcanePunkCharacter::EndSwapDash()
{
	GetWorldTimerManager().ClearTimer(SwapDashTimerHandle);
	bDoing = false; bCanMove = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
	MoveComponent->EndSwapDash();
	GhostTrailSpawnComp->SetSkillTrail(false);
	
	if(PlayerState == EPlayerState::EntryMode) 
	{
		PlayerState = EPlayerState::EntrySkillPlaying;
	}
	else 
	{
		PlayerState = EPlayerState::Idle;
	}

	SwitchPlayerState();
}

void AArcanePunkCharacter::HandleGameplayEvent(const FGameplayTag& EventTag)
{
	//@이벤트 태그 유효성 검사
	if (!EventTag.IsValid())
	{
		UE_LOGFMT(LogCharacter, Warning, "게임플레이 이벤트 처리 실패 - 유효하지 않은 태그");
		return;
	}

	UE_LOGFMT(LogCharacter, Log, "게임플레이 이벤트 처리 시작 - 태그: {0}", *EventTag.ToString());

	//@저스트 닷지 태그 확인
	static const FGameplayTag JustDodgeTag = FGameplayTag::RequestGameplayTag("Ability.Chain.JustDodge");

	if (EventTag == JustDodgeTag)
	{
		UE_LOGFMT(LogCharacter, Log, "저스트 닷지 이벤트 감지 - 닷지 시작");
		StartJustDodge();
	}
	else
	{
		UE_LOGFMT(LogCharacter, Log, "처리할 수 없는 이벤트 태그: {0}", *EventTag.ToString());
	}
}

void AArcanePunkCharacter::StartChainWindow(const FChainActionInfo& ChainInfo)
{
	//@유효성 검사
	if (!ChainInfo.EventTag.IsValid() || !ChainInfo.ActivationEventTag.IsValid())
	{
		UE_LOGFMT(LogCharacter, Warning, "체인 윈도우 시작 실패 - 유효하지 않은 태그");
		return;
	}

	//@체인 윈도우 활성화
	CurrentChainActionInfo = ChainInfo;
	bChainWindowActive = true;
	bCanChainAction = false;

	UE_LOGFMT(LogCharacter, Log, "체인 윈도우 시작 - 이벤트 태그: {0}, 활성화 태그: {1}, 모드: {2}",
		*ChainInfo.EventTag.ToString(),
		*ChainInfo.ActivationEventTag.ToString(),
		ChainInfo.ChainActionMode == EChainActionMode::Delayed ? TEXT("지연 실행") : TEXT("즉시 실행"));
}

void AArcanePunkCharacter::EndChainWindow()
{
	//@체인 윈도우 체크
	if (!bChainWindowActive)
	{
		UE_LOGFMT(LogCharacter, Log, "체인 윈도우 종료 - 체인 윈도우가 활성화되지 않음");
		return;
	}

	//@지연 실행 모드 처리
	if (CurrentChainActionInfo.ChainActionMode == EChainActionMode::Delayed && bCanChainAction)
	{
		UE_LOGFMT(LogCharacter, Log, "지연 실행 모드로 이벤트 처리: {0}",
			*CurrentChainActionInfo.ActivationEventTag.ToString());

		HandleGameplayEvent(CurrentChainActionInfo.ActivationEventTag);
	}

	//@체인 윈도우 비활성화
	bChainWindowActive = false;
	bCanChainAction = false;
	CurrentChainActionInfo = FChainActionInfo();

	UE_LOGFMT(LogCharacter, Log, "체인 윈도우 종료 완료");
}

void AArcanePunkCharacter::StartJustDodge()
{
	//@입력 유효성 검사
	if (!bMainPlayer)
	{
		UE_LOGFMT(LogCharacter, Log, "메인 플레이어가 아니므로 저스트 닷지를 실행하지 않습니다.");
		return;
	}

	//@움직임 가능성 여부
	//if (!bCanMove)
	//{
	//	UE_LOGFMT(LogCharacter, Log, "이동이 불가능한 상태이므로 저스트 닷지를 실행하지 않습니다.");
	//	return;
	//}

	//@죽음 여부
	if (IsDead())
	{
		UE_LOGFMT(LogCharacter, Log, "캐릭터가 사망 상태이므로 저스트 닷지를 실행하지 않습니다.");
		return;
	}

	//@대시 캔슬
	MoveComponent->EndDash();

	//@저스트 닷지 실행
	UE_LOGFMT(LogCharacter, Log, "저스트 닷지 실행 - 체인 윈도우로부터 성공적인 회피");

	//@TODO: 애니메이션 제작 완료 될 경우, ANS로 옮김 처리.
	//@시간 조작 - 슬로모션 적용, 중간 강도, 0.3초간 적용
	UAPTimeManipulationSubsystem* TimeSubsystem = GetGameInstance()->GetSubsystem<UAPTimeManipulationSubsystem>();
	if (TimeSubsystem)
	{
		//@슬로모션 설정
		FTimeDilationSettings SlowMotionSettings;
		SlowMotionSettings.DilationMode = ETimeDilationMode::SlowMotion;
		SlowMotionSettings.DilationIntensity = ETimeDilationIntensity::High;
		SlowMotionSettings.bSmoothTransition = false;

		//@글로벌 슬로모션 시작
		TimeSubsystem->StartGlobalTimeDilation(SlowMotionSettings);

		UE_LOGFMT(LogCharacter, Log, "저스트 닷지 - 중간 강도의 글로벌 슬로모션 적용");

		//@일정 시간 후 슬로모션 해제 타이머 설정 (예: 0.3초)
		FTimerHandle SlowMotionTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			SlowMotionTimerHandle,
			FTimerDelegate::CreateLambda([TimeSubsystem]()
				{
					if (TimeSubsystem)
					{
						TimeSubsystem->StopGlobalTimeDilation(false);
					}
				}),
			0.2f,
			false
		);
	}
	else
	{
		UE_LOGFMT(LogCharacter, Warning, "시간 조작 서브시스템을 찾을 수 없어 슬로모션을 적용할 수 없습니다.");
	}
}


void AArcanePunkCharacter::OnBlockMode()
{ 
	bBlockMode = true;
	GetWorldTimerManager().SetTimer(BlockTimerHandle, this, &AArcanePunkCharacter::ClearBlockMode, BlockTime, false);
}

void AArcanePunkCharacter::ClearBlockMode()
{
	bBlockMode = false;
	GetWorldTimerManager().ClearTimer(BlockTimerHandle);
}

void AArcanePunkCharacter::SetSkillAbility(EEnhanceCategory EnhanceCategory, EEnHanceType EnHanceType)
{
	if(!HUD) return;

	HUD->DisplayEnhanceChoice(EnhanceCategory, EnHanceType);
}

void AArcanePunkCharacter::SaveStatus(const FString & PlayerSlotName)
{
	SavePlayerData(PlayerSlotName);
	SaveGameData(PlayerSlotName);

	UAPDataTableSubsystem::DisplaySystemMesseage(UAPGameInstance::GetDataTableGI(GetWorld()), EStringRowName::SaveComplete, false, false);
}

void AArcanePunkCharacter::SavePlayerData(const FString & PlayerSlotName)
{
	TotalStatus_Origin.SaveSlotName = PlayerSlotName;
	TotalStatus_Origin.PlayerLocation = GetActorLocation();
	
	MyPlayerState->UpdatePlayerData(TotalStatus_Origin);
}

void AArcanePunkCharacter::SaveGameData(const FString & PlayerSlotName)
{
	auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;

	auto MyGameState = Cast<AAPGameState>(UGameplayStatics::GetGameState(GetWorld()));
	MyGameStatus.SaveSlotName = PlayerSlotName;
	MyGameStatus.LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	float RT = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	MyGameStatus.SaveRealTime += RT - APGI->GetSavedRealTime(); APGI->SetSavedRealTime(RT);
	MyGameStatus.SaveDateTime = FDateTime::Now();

	MyGameState->UpdateGameData(MyGameStatus);	
}

void AArcanePunkCharacter::SetRSkill(EUltSkillNumber NewSkill)
{
	RSkill = NewSkill;
}

void AArcanePunkCharacter::AddPassive(const EPassiveNumber & PassiveNum)
{
	if(PassiveSkills.Contains((uint8)PassiveNum)) return;
	
	FSkillAbilityNestingData SkillAbilityNestingData = {};
	PassiveSkills.Emplace((uint8)PassiveNum, SkillAbilityNestingData);
	PassiveComp->ApplyNewPassive(PassiveNum);
}

void AArcanePunkCharacter::AddPassive_Enhance(uint8 PassiveNum, const EEnHanceType & EnHanceType, uint8 AbilityNum, uint16 AbilityNestingNum)
{
	if(!PassiveSkills.Contains(PassiveNum)) return;
	switch (EnHanceType)
	{
	case EEnHanceType::Silver:
		PassiveSkills[PassiveNum].SilverAbilityNestingNum.Emplace(AbilityNum, AbilityNestingNum);
		break;
	
	case EEnHanceType::Gold:
		PassiveSkills[PassiveNum].GoldAbilityNestingNum.Emplace(AbilityNum, AbilityNestingNum);
		break;

	case EEnHanceType::Platinum:
		PassiveSkills[PassiveNum].PlatinumAbilityNestingNum.Emplace(AbilityNum, AbilityNestingNum);
		break;
	}
	PassiveComp->ApplyNewPassive((EPassiveNumber)PassiveNum);
}

void AArcanePunkCharacter::WorldMap()
{
	if(!GM.IsValid()) return; 
	if(!GM->IsBattleStage()) return;
	if(HUD) HUD->OpenWorldMap();
}

void AArcanePunkCharacter::Alt_RightClick()
{
	if(!PC.IsValid()) return;
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	if(HitResult.bBlockingHit)
	{
		OnAltRightMouseClick.Broadcast(HitResult.Location);
	}
}

void AArcanePunkCharacter::CurrentPlayerLocation() // 후에 사용할지 말지 결정, 현재는 필요없는듯?
{
	if(TotalStatus.SaveOperation)
	{
		SetActorLocation(TotalStatus.PlayerLocation);
	}
}

const FTransform & AArcanePunkCharacter::ReturnCameraTransform()
{
    return APCamera->GetComponentTransform();
}

void AArcanePunkCharacter::SetHavingSkills()
{
	HavingSkill.Empty(); 
	HavingSkill.Add(QSkill); HavingSkill.Add(ESkill);
}

float AArcanePunkCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	//@체인 윈도우 체크
	if (bChainWindowActive)
	{
		//@데미지 이벤트와 체인 이벤트가 일치하는지 확인
		static const FGameplayTag DamageEventTag = FGameplayTag::RequestGameplayTag("EventTag.OnDamaged");

		if (CurrentChainActionInfo.EventTag == DamageEventTag)
		{
			UE_LOGFMT(LogCharacter, Log, "데미지 발생으로 체인 액션 감지: {0}",
				*CurrentChainActionInfo.ActivationEventTag.ToString());

			//@체인 액션 가능 상태로 설정
			bCanChainAction = true;

			//@모드에 따른 처리
			if (CurrentChainActionInfo.ChainActionMode == EChainActionMode::Immediate)
			{
				UE_LOGFMT(LogCharacter, Log, "즉시 실행 모드로 이벤트 처리");
				HandleGameplayEvent(CurrentChainActionInfo.ActivationEventTag);
				EndChainWindow(); // 즉시 체인 윈도우 종료
			}
			else
			{
				UE_LOGFMT(LogCharacter, Log, "지연 실행 모드로 체인 액션 활성화");
				// EndChainWindow() 호출 시 처리됨
			}

			return 0.0f; // 데미지를 무시하고 체인 액션 처리
		}
	}

	//@무적 상태 체크

	//@패링 체크
	if (AttackComponent->CheckParryingCondition(DamageEvent, EventInstigator))
	{
		UE_LOGFMT(LogCharacter, Log, "패링 성공으로 데미지가 무시되었습니다.");
		return 0.0f;
	}

	//@초기 변수 설정
	float DamageApplied = DamageAmount;
	float OriginHP = TotalStatus.StatusData.HP;

	//@리플렉팅 모드 체크
	if (ReflectingModeGauge > 0)
	{
		UE_LOGFMT(LogCharacter, Log, "리플렉팅 모드로 {0} 데미지를 반사합니다.", DamageApplied);
		AttackComponent->ReflectDamage(DamageApplied, DamageCauser);
		return DamageApplied;
	}

	//@블록 모드 체크
	if (bBlockMode)
	{
		UE_LOGFMT(LogCharacter, Log, "블록 모드로 데미지가 무시되었습니다.");
		return 0.0f;
	}

	//@패시브 데미지 체크
	bool Check = false;
	if (GetAPPassiveComp()->IsDamagedPassive())
	{
		Check = CheckingDamaged();
	}

	if (Check)
	{
		UE_LOGFMT(LogCharacter, Log, "패시브 효과로 데미지가 무시되었습니다.");
		return 0.0f;
	}

	//@데미지 계산
	DamageApplied = DamageMath(DamageApplied);
	UE_LOGFMT(LogCharacter, Log, "계산된 데미지: {0}", DamageApplied);

	//@방패 HP 체크
	if (CheckShieldHP(DamageApplied, DamageEvent))
	{
		UE_LOGFMT(LogCharacter, Log, "방패가 데미지를 흡수했습니다.");
		return 0.0f;
	}

	//@HP 감소 처리
	float& HP = TotalStatus.StatusData.HP;

	if (DamageApplied >= HP && GetRageMode())
	{
		HP = 1.0f;
		UE_LOGFMT(LogCharacter, Log, "분노 모드로 HP가 1로 유지됩니다.");
	}
	else
	{
		HP = FMath::Clamp<float>(HP - DamageApplied, 0.0f, TotalStatus.StatusData.MaxHP);
		UE_LOGFMT(LogCharacter, Log, "현재 HP: {0}/{1}", HP, TotalStatus.StatusData.MaxHP);
	}

	//@사망 처리
	if (IsDead())
	{
		UE_LOGFMT(LogCharacter, Log, "캐릭터가 사망했습니다.");
		SetCanMove(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// OwnerCharacter->DetachFromControllerPendingDestroy();
		DeadPenalty(3.0f);

		auto BattleGM = Cast<AAPGameModeBattleStage>(GM);
		if (BattleGM)
		{
			BattleGM->PlayerKilled();
		}
	}
	else
	{
		//@분노 모드 체크
		if (GetRageMode())
		{
			UE_LOGFMT(LogCharacter, Log, "분노 모드로 데미지가 무시되었습니다.");
			return 0.0f;
		}

		//@히트 UI 표시
		if (PC.IsValid() && DamageApplied > KINDA_SMALL_NUMBER)
		{
			PC->HitUI();
			UE_LOGFMT(LogCharacter, Log, "히트 UI를 표시합니다.");
		}
	}

	//@상태 업데이트
	UpdateStatus();

	//@HUD 업데이트
	if (HUD)
	{
		HUD->GetStatusWidget(this)->SetHPPercent(this, OriginHP);
		UE_LOGFMT(LogCharacter, Log, "HUD에 HP 정보를 업데이트했습니다.");
	}

	//@패시브 효과 체크
	PassiveComp->CheckDamagedGold();

	Super::TakeDamage(DamageApplied, DamageEvent, EventInstigator, DamageCauser);
	return DamageApplied;
}

bool AArcanePunkCharacter::CheckingDamaged()
{
	// DamagedNumber++;

	// if(DamagedNumber == 8 && DamagedShield.IsValid()) {DamagedNumber = 0; DamagedShield->Destroy(); return true;}
	// else if(DamagedNumber == 7)
	// {
	// 	DamagedShield = GetWorld()->SpawnActor<AActor>(DamagedShieldClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	// 	DamagedShield->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	// }

	return false;
}

bool AArcanePunkCharacter::IsDead()
{
	if(!MyPlayerState) return false;
	
    return TotalStatus.StatusData.HP <= KINDA_SMALL_NUMBER;
}

void AArcanePunkCharacter::DeadPenalty(float DeathTime)
{
	if(!PC.IsValid()) return;
	DisableInput(PC.Get());

	if(MyPlayerState) MyPlayerState->DeathPenalty();

	auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;

	auto MyGameState = Cast<AAPGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// 마을로 설정
	MyGameStatus.LevelName = "Tutorial_Town";//UGameplayStatics::GetCurrentLevelName(GetWorld());

	float RT = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	MyGameStatus.SaveRealTime += RT - APGI->GetSavedRealTime(); APGI->SetSavedRealTime(RT);
	
	MyGameStatus.SaveDateTime = FDateTime::Now();
	MyGameState->UpdateGameData(MyGameStatus);	

	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, PC.Get(), &AArcanePunkPlayerController::DisplayDeadUI, DeathTime, false);
}

FTransform AArcanePunkCharacter::GetFootTransform(bool Left)
{
	if(Left) return FootPrint_L->GetComponentTransform();
    return FootPrint_R->GetComponentTransform();
}

USkeletalMeshComponent *AArcanePunkCharacter::GetPlayerEquipment(uint8 NewValue)
{
	switch(NewValue)
	{
		case 1:
		return Weapon;
		break;
	}
    return nullptr;
}

UAPItemBase *AArcanePunkCharacter::GetEquipData(uint8 NewValue)
{
	switch(NewValue)
	{
		case 1:
		return (WeaponReference.IsEmpty() ? nullptr : WeaponReference.Top());
		break;
	}
    return nullptr;
}

void AArcanePunkCharacter::SetEquipData(uint8 NewValue, UAPItemBase *NewData)
{
	switch(NewValue)
	{
		case 1:
		ChangeEquipData(WeaponReference, NewData);
		break;
	}
}

bool AArcanePunkCharacter::PMCheck(FHitResult &HitResult, const FVector & OverlapStart, const FVector & OverlapEnd) // 캐릭터 발 밑 피지컬머터리얼 체크
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;
	 
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GetCapsuleComponent()->GetScaledCapsuleRadius());
	 
	return GetWorld()->SweepSingleByChannel(HitResult, OverlapStart, OverlapEnd, FQuat::Identity, ECC_Pawn, Sphere, Params);
}

float AArcanePunkCharacter::GetCurrentATK() const
{
    return FinalATK;
}

void AArcanePunkCharacter::UpdateStatus()
{
	TotalStatus.StatusData.MaxMP = TotalStatus_Origin.StatusData.MaxMP;
	FinalATK = TotalStatus.StatusData.ATK;
	WeaponReference.IsEmpty() ? FinalATK = TotalStatus.StatusData.ATK : FinalATK = TotalStatus.StatusData.ATK + WeaponReference.Top()->ItemStatistics.DamageValue;

	if(PC.IsValid()) PC->OnUpdateStatusText.Broadcast();
}

void AArcanePunkCharacter::InitPlayerStatus()
{
	MyPlayerState = Cast<AArcanePunkPlayerState>(GetPlayerState());
	if(!MyPlayerState) return;
	// if(MyPlayerState->PlayerTotalStatus.SaveOperation) CurrentPlayerLocation();
	TotalStatus_Origin = MyPlayerState->PlayerTotalStatus; // 테스트용으로 빼놈, 이 줄 넣어야 세이브 스탯으로 초기화
	TotalStatus = TotalStatus_Origin;

	GetCharacterMovement()->MaxWalkSpeed = TotalStatus.StatusData.MoveSpeed;
	
	auto MyGameState = Cast<AAPGameState>(UGameplayStatics::GetGameState(GetWorld()));
	MyGameStatus = MyGameState->GameData;
	
	OnCheckingShield.Broadcast(this, 0.0f);
}

void AArcanePunkCharacter::BeginInteract()
{
	OnBeginInteract.Broadcast(this);
}


// prodo
void AArcanePunkCharacter::DropItems(UAPItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) + GetActorUpVector() * 150.0f };
		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(DropClass, SpawnLocation, GetActorRotation(), SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("drop item is null"));
	}
}

void AArcanePunkCharacter::ToggleMenu()
{	
	if (HUD)
	{
		UpdateInventoryWidgetPosition(PlayerInventory->GetInventoryContents().Num());
	}
}

void AArcanePunkCharacter::UpdateInventoryWidgetPosition(int32 Numbers)
{
	if (HUD)
	{
		UMainMenu* Inventory;
		Inventory = HUD->GetInventoryWidget();

		// 아이템 개수에 따라서 UI의 위치 수정
		// one item y size = 80

		FVector2D ViewportSize = HUD->GetViewportSize();
		FVector2D WidgetSize = Inventory->GetDesiredSize();

		FVector2D BasePosition = FVector2D(ViewportSize.X - (WidgetSize.X / 2.0f), ViewportSize.Y - (WidgetSize.Y / 2.0f));
		
		FVector2D NewPosition = BasePosition;

		// 1개당 50 +

		switch (Numbers)
		{
		case 0:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y + 120.0f);
			break;
		case 1:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y + 80.0f);
			break;
		case 2:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y + 40.0f);
			break;
		case 3:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y);
			break;
		case 4:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y - 40.0f);
			break;
		case 5:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y - 80.0f);
			break;
		case 6:
		default:
			NewPosition = FVector2D(BasePosition.X, BasePosition.Y - 120.0f);
			break;
		}
		

		Inventory->SetPositionInViewport(NewPosition);
	}
}

void AArcanePunkCharacter::InventorySort()
{
	if(PlayerInventory)
	{
		PlayerInventory->SortingInventory();
	}
}

void AArcanePunkCharacter::SetSkill(ESkillKey SkillKey, FName SkillName) // ChangeSkill 따로
{
    // TODO: 스킬 아이콘 등에 대한 처리도 같이 할 수 있도록 구현하기
	if(!SkillControllers.Contains(SkillKey))
	{
		ASkillController* NewSkillController = GetWorld()->SpawnActor<ASkillController>();
		SkillControllers.Add(SkillKey, NewSkillController);
	}

	SkillControllers[SkillKey]->InitializeSkills(SkillKey, SkillName, this);
	
}

void AArcanePunkCharacter::UseSkill(ESkillKey SkillKey)
{
	
	switch (SkillKey)
    {
    case ESkillKey::Q:
		OnQSkill = true;
        break;
    case ESkillKey::E:
        OnESkill = true;
        break;
    case ESkillKey::R:
        OnRSkill = true;
        break;
    }
	
	if(LastSkillKey != SkillKey && SkillControllers.Contains(LastSkillKey))
	{
		SkillControllers[LastSkillKey]->Restore();
	}

	if(SkillControllers.Contains(SkillKey))
	{
		SkillControllers[SkillKey]->UseSkill(SkillKey, this);
		LastSkillKey = SkillKey;
	}
}

void AArcanePunkCharacter::EnhanceSkill(ESkillKey SkillKey, int32 EnhanceNumber)
{
	if(SkillControllers.Contains(SkillKey))
	{
		SkillControllers[SkillKey]->GetSkillActor()->EnhanceSkill(EnhanceNumber);
	}
}

void AArcanePunkCharacter::PlaySkillAction(UAnimMontage* SkillAction)
{
	if (SkillAction != nullptr)
	{
		PlayerState = EPlayerState::ActiveSkillPlaying;
		bSkilling = true;
		bCanMove = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && SkillAction->IsValidLowLevel())
		{
			// 몽타주 재생
			AnimInstance->Montage_Play(SkillAction);
            
			// // 몽타주가 끝났을 때 입력을 다시 활성화
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this, &AArcanePunkCharacter::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
		}
	}
}

void AArcanePunkCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	PlayerState = EPlayerState::Idle;
	bSkilling = false;
	bCanMove = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AArcanePunkCharacter::RestoreSkills()
{
	for(auto SkillController : SkillControllers)
	{
		if(SkillController.Value && SkillController.Value->IsReady())
		{
			SkillController.Value->Restore();
		}
	}
}

void AArcanePunkCharacter::UseSwapSkill_Exit()
{
	auto Anim = Cast<UArcanePunkCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if(Anim)
	{
		SetDoing(true);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		Anim->PlaySwapSkill_Exit();
	}
}

void AArcanePunkCharacter::UseSwapSkill_Entry()
{
	auto Anim = Cast<UArcanePunkCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if(Anim)
	{
		SetDoing(true);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		Anim->PlaySwapSkill_Entry();
	}
}
