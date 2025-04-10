#include "Components/Character/SkillNumber/SkillNumberBase.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/SkillRange/APSkillRange_Arrow.h"
#include "Character/SkillRange/APSkillRange_Circle.h"
#include "Components/Character/APMovementComponent.h"
#include "GameInstance/APGameInstance.h"
#include "Character/SkillRange/APSkillRange_TwoCircle.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Skill/SkillDataManager.h"

USkillNumberBase::USkillNumberBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillNumberBase::BeginPlay()
{
	Super::BeginPlay();
	
	CreateInit();
	InitSkillData();
}

void USkillNumberBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckingOtherSkill();
}

void USkillNumberBase::CreateInit()
{
	MaxChargeNum = MaxChargeNum_Origin;
	CurrentChargeNum = MaxChargeNum;

	SetComponentTickEnabled(false);
	AddAbilityList();

	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerCharacterPC.IsValid()) return;
}

void USkillNumberBase::InitSkillData()
{
	auto APGI = Cast<UAPGameInstance>(GetOwner()->GetGameInstance()); if(!APGI) return;
	auto DataTableGI = Cast<UAPDataTableSubsystem>(APGI->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return; 
	SettingGI = Cast<UAPSettingSubsystem>(APGI->GetSubsystemBase(UAPSettingSubsystem::StaticClass()));

	switch (SkillKey)
    {
        case ESkillKey::Q:
        SkillAbilityNestingData = APGI->GetQSkillAbilityNestingData();
        break;

        case ESkillKey::E:
        SkillAbilityNestingData = APGI->GetESkillAbilityNestingData();
        break;

        case ESkillKey::R:
        SkillAbilityNestingData = APGI->GetRSkillAbilityNestingData();
        break;
    }
	UpdateSkillData();

	auto DataTable = DataTableGI->GetSkillNameListDataTable()->FindRow<FSkillNameList>(SkillAbilityNestingData.SkillName, SkillAbilityNestingData.SkillName.ToString()); if(!DataTable) return;
	SkillNameListData = *DataTable;
	CurrentCoolTime = SkillNameListData.CoolTime;
	MPConsumption = SkillNameListData.MPConsumption;
}

void USkillNumberBase::CheckingOtherSkill()
{
	if(!OwnerCharacter.IsValid()) return;

	if(CheckSmartKey(SkillKey)) 
	{ 
		SetComponentTickEnabled(false);
		OnSkill(); return; 
	}
	if(OwnerCharacter->GetAPSkillHubComponent()->GetSkillState() != ESkillKey::None && Skilling)
	{
		if(OwnerCharacter->GetAPSkillHubComponent()->GetSkillState() != OwnerCharacter->GetAPSkillHubComponent()->LastSkill) {Remove_Skill(); return;}
	}
	if(OwnerCharacter->returnState() != ECharacterState::None && Skilling)
	{
		Remove_Skill(); return;
	}
	if(SkillKey != OwnerCharacter->GetAPSkillHubComponent()->LastSkill) 
	{
		if(SkillKey == ESkillKey::None &&  OwnerCharacter->GetAPSkillHubComponent()->LastSkill == ESkillKey::R) return;
		Remove_Skill(); return;
	}
}

void USkillNumberBase::ClearSkillAbilityNestingData(EEnHanceType ClearEnHanceType)
{
	switch (ClearEnHanceType)
	{
	case EEnHanceType::Silver:
	SkillAbilityNestingData.SilverAbilityNestingNum.Empty();
		break;
	
	case EEnHanceType::Gold:
	SkillAbilityNestingData.GoldAbilityNestingNum.Empty();
		break;

	case EEnHanceType::Platinum:
	SkillAbilityNestingData.PlatinumAbilityNestingNum.Empty();
		break;
	}
}

void USkillNumberBase::AddSkillAbilityNestingData(EEnHanceType AddEnHanceType, TPair<uint8, uint16> UpdateAbilityNestingNum)
{
	switch (AddEnHanceType)
	{
	case EEnHanceType::Silver:
	SkillAbilityNestingData.SilverAbilityNestingNum.Add(UpdateAbilityNestingNum);
		break;
	
	case EEnHanceType::Gold:
	SkillAbilityNestingData.GoldAbilityNestingNum.Add(UpdateAbilityNestingNum);
		break;

	case EEnHanceType::Platinum:
	SkillAbilityNestingData.PlatinumAbilityNestingNum.Add(UpdateAbilityNestingNum);
		break;
	}
}

void USkillNumberBase::PlaySkill()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerCharacterPC.IsValid()) return;
}

void USkillNumberBase::Spawn_SkillRange()
{
	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->OnLeftMouseClick.AddDynamic(this, &USkillNumberBase::OnSkill);
}

void USkillNumberBase::OnSkill()
{
	if(!OwnerCharacter.IsValid()) return;
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->UpdateMPBar(MPConsumption, true);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->OperateSkillSlot(SkillKey);
	OwnerCharacter->GetAPHUD()->GetStatusWidget()->StartCoolTimeSlot(SkillKey, CurrentCoolTime - AddSkillCoolTime);
	AddSkillCoolTime = 0.0f;
	OwnerCharacter->OnSkillTrigger.AddDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.AddDynamic(this, &USkillNumberBase::SkillEnd);
	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);
}

void USkillNumberBase::Remove_Skill()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	Remove_SkillRange();

	OwnerCharacter->OnSkillTrigger.RemoveDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillChargingTrigger.RemoveDynamic(this, &USkillNumberBase::Enhance);
	OwnerCharacter->OnSkillEnhanceTrigger.RemoveDynamic(this, &USkillNumberBase::DoubleEnhance);
}

void USkillNumberBase::Remove_SkillRange()
{
	if(!OwnerCharacter.IsValid()) return; if(!OwnerCharacterPC.IsValid()) return;
	
	SetComponentTickEnabled(false);

	if(SkillRange_Target.IsValid()) SkillRange_Target->Destroy();
	if(SkillRange_Circle.IsValid()) SkillRange_Circle->Destroy();
	if(SkillRange_TWoCircle.IsValid()) SkillRange_TWoCircle->Destroy();
 
	OwnerCharacterPC->SetMouseCursor();
	USkillDataManager::CursorImmediately();
	Skilling = false;

	OwnerCharacter->OnLeftMouseClick.RemoveDynamic(this, &USkillNumberBase::OnSkill);
}

void USkillNumberBase::SkillEnd()
{
	if(!OwnerCharacter.IsValid()) return; 
	OwnerCharacter->OnSkillTrigger.RemoveDynamic(this, &USkillNumberBase::Activate_Skill);
	OwnerCharacter->OnSkillEndTrigger.RemoveDynamic(this, &USkillNumberBase::SkillEnd);
	OwnerCharacter->OnSkillEndWithSkillKey.Broadcast(SkillKey, CurrentSkillNumber);
}

void USkillNumberBase::RemoveEffect()
{
}

void USkillNumberBase::Activate_Skill()
{
	if(!OwnerCharacter.IsValid()) return; 
	OwnerCharacter->OnSkillTrigger.RemoveDynamic(this, &USkillNumberBase::Activate_Skill);
}

void USkillNumberBase::Enhance()
{
	if(!OwnerCharacter.IsValid()) return; 
	OwnerCharacter->OnSkillChargingTrigger.RemoveDynamic(this, &USkillNumberBase::Enhance);
}

void USkillNumberBase::DoubleEnhance()
{
	if(!OwnerCharacter.IsValid()) return; 
	OwnerCharacter->OnSkillEnhanceTrigger.RemoveDynamic(this, &USkillNumberBase::DoubleEnhance);
}

void USkillNumberBase::MarkingOn(AActor *OtherActor, float Time)
{
}

void USkillNumberBase::MarkErase()
{
}

void USkillNumberBase::UpdateSkillData()
{
    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetWorld()->GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return; 
	RowDataTable = DataTableGI->GetSkillAbilityRowDataTable()->FindRow<FSkillAbilityRowNameData>(SkillAbilityNestingData.SkillName, SkillAbilityNestingData.SkillName.ToString());
}

void USkillNumberBase::UpdatAbilityData(EEnHanceType EnHanceType, uint8 AbilityNum)
{
    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetWorld()->GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return; 
	if(AbilityNum == 0) return; 
	AbilityNum--;

	switch (EnHanceType)
	{
		case EEnHanceType::Silver:
		AbilityData = DataTableGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowDataTable->SilverRowName[AbilityNum]), RowDataTable->SilverRowName[AbilityNum]);
		break;
	
		case EEnHanceType::Gold:
		AbilityData = DataTableGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowDataTable->GoldRowName[AbilityNum]), RowDataTable->GoldRowName[AbilityNum]);
		break;

		case EEnHanceType::Platinum:
		AbilityData = DataTableGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowDataTable->PlatinumRowName[AbilityNum]), RowDataTable->PlatinumRowName[AbilityNum]);
		break;
	}
}

void USkillNumberBase::OnCoolDown()
{
}

bool USkillNumberBase::CheckSkillCondition()
{
	bool Check = true;
	if(!OwnerCharacter.IsValid()) Check = false;
	if(OwnerCharacter->GetDoing()) Check = false;
	if(OwnerCharacter->GetPlayerStatus().StatusData.MP < MPConsumption || !CheckSkillCool(SkillKey)) {Check = false;}
	
	if(!Check)
	{
		UAPDataTableSubsystem::DisplaySystemMesseage(UAPGameInstance::GetDataTableGI(GetWorld()), EStringRowName::CannotSkill, true, true); 
		UAPSoundSubsystem::PlayRejectSound(UAPGameInstance::GetSoundGI(GetWorld()));
	}
	
    return Check;
}

bool USkillNumberBase::CheckSmartKey(ESkillKey WhichKey)
{
	if(!SettingGI.IsValid()) return false;

	if(SettingGI->GetSmartKey()[(uint8)WhichKey])
	{
		OwnerCharacterPC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		OwnerCharacter->SetHomingPoint(HitResult.Location);
		return true;
	}

	return false;
}

void USkillNumberBase::SetMouseCursor(AArcanePunkPlayerController *PC, ESkillCursor NewCursor)
{
	switch (NewCursor)
	{
		case ESkillCursor::Default:
		PC->CurrentMouseCursor = EMouseCursor::Default; 
		break;
		case ESkillCursor::Crosshairs:
		PC->CurrentMouseCursor = EMouseCursor::Crosshairs; 
		break;
	}
	
	USkillDataManager::CursorImmediately();
}

bool USkillNumberBase::CheckSkillCool(ESkillKey WhichKey)
{
	if(!OwnerCharacter.IsValid()) false;

	switch (WhichKey)
	{
		case ESkillKey::Q:
        return OwnerCharacter->GetbCanSkill_Q();
        break;
    
        case ESkillKey::E:
        return OwnerCharacter->GetbCanSkill_E();
        break;

        case ESkillKey::R:
        return OwnerCharacter->GetbCanSkill_R();
        break;
	}

    return true;
}

void USkillNumberBase::SkillCancel()
{
	if(!OwnerCharacter.IsValid()) return;

	OwnerCharacter->SetDoing(false);
	// OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
}

void USkillNumberBase::AddAbilityList()
{
}

void USkillNumberBase::ActivateSkillRange_Round(float Range)
{
	if(!OwnerCharacter.IsValid()) return;

	SkillRange_Circle = GetWorld()->SpawnActor<AAPSkillRange>(USkillDataManager::GetInstance()->GetAPSkillRange(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator + FRotator(90,0,0));
	if(!SkillRange_Circle.IsValid()) return;
	SkillRange_Circle->SetOwner(OwnerCharacter.Get());
	SkillRange_Circle->GetDecalComponent()->DecalSize = FVector(15.0f, Range, Range);
	SkillRange_Circle->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);

	
}

void USkillNumberBase::ActivateSkillRange_Target(float Range_1, float Range_2, ESkillRangeType SkillRangeType)
{
	if(!OwnerCharacter.IsValid()) return;

	if(SkillRangeType == ESkillRangeType::Control_Circle)
	{
		SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(USkillDataManager::GetInstance()->GetAPSkillRange_Target(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		SkillRange_Target->SetScale(Range_1);
	}
	else if(SkillRangeType == ESkillRangeType::Arrow)
	{
		SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(USkillDataManager::GetInstance()->GetAPSkillRange_Arrow(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		if(SkillRange_Target.IsValid()) SkillRange_Target->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		SkillRange_Target->SetScale_Arrow(Range_1, Range_2);
		SkillRange_Target->GetDecalComponent()->DecalSize = FVector(5.0f, Range_1, Range_2);
	}
	else if(SkillRangeType == ESkillRangeType::Around_Circle)
	{
		SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(USkillDataManager::GetInstance()->GetAPSkillRange_Circle(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		if(SkillRange_Target.IsValid()) SkillRange_Target->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		SkillRange_Target->GetDecalComponent()->DecalSize = FVector(5.0f, Range_1, Range_2);
	}
	else if(SkillRangeType == ESkillRangeType::Square)
	{
		SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(USkillDataManager::GetInstance()->GetAPSkillRange_Square(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		if(SkillRange_Target.IsValid()) SkillRange_Target->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		SkillRange_Target->GetDecalComponent()->DecalSize = FVector(5.0f, Range_1, Range_2);
	}
	else if(SkillRangeType == ESkillRangeType::Two_Circle)
	{
		SkillRange_TWoCircle = GetWorld()->SpawnActor<AAPSkillRange_TwoCircle>(USkillDataManager::GetInstance()->GetAPSkillRange_TWoCircle(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		SkillRange_TWoCircle->GetDecalComponent()->DecalSize = FVector(5.0f, Range_1, Range_1);
		SkillRange_TWoCircle->GetDecalComponent_2()->SetHiddenInGame(true);
		SkillRange_TWoCircle->SetOwner(OwnerCharacter.Get());
		SkillRange_TWoCircle->SetScale(Range_2);
		SkillRange_TWoCircle->SetSkillRangeType(SkillRangeType);
	}
	else if(SkillRangeType == ESkillRangeType::SectorCircle)
	{
		SkillRange_Target = GetWorld()->SpawnActor<AAPSkillRange>(USkillDataManager::GetInstance()->GetAPSkillRange_SectorCircle(), OwnerCharacter->GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		if(SkillRange_Target.IsValid()) SkillRange_Target->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		SkillRange_Target->GetDecalComponent()->DecalSize = FVector(5.0f, Range_1, Range_2);
	}
	
	if(!SkillRange_Target.IsValid()) return;
	SkillRange_Target->SetOwner(OwnerCharacter.Get());
	SkillRange_Target->SetSkillRangeType(SkillRangeType);
}

void USkillNumberBase::CharacterRotation()
{
	if(!OwnerCharacter.IsValid()) return; if(!SkillRange_Target.IsValid()) return;
		
	FVector Loc = SkillRange_Target->GetDecalComponent()->GetComponentLocation() - OwnerCharacter->GetMesh()->GetComponentLocation();

	Loc.Z = 0.0f; 
	
	FRotator Rotation = FRotationMatrix::MakeFromX(Loc).Rotator();
	
	OwnerCharacter->GetAPMoveComponent()->SetAttackRotation(Rotation, RotSpeed);

	OwnerCharacter->OnSkillRotationTrigger.RemoveDynamic(this, &USkillNumberBase::CharacterRotation);
}

void USkillNumberBase::CharacterRotation_TwoCircle()
{
	if(!OwnerCharacter.IsValid()) return; if(!SkillRange_TWoCircle.IsValid()) return;

	FVector Loc = SkillRange_TWoCircle->GetDecalComponent()->GetComponentLocation() - OwnerCharacter->GetMesh()->GetComponentLocation();

	Loc.Z = 0.0f; FRotator Rotation = FRotationMatrix::MakeFromX(Loc).Rotator();
	
	OwnerCharacter->GetAPMoveComponent()->SetAttackRotation(Rotation, RotSpeed);
}

void USkillNumberBase::CharacterRotation_Cursor(FHitResult& Hit)
{
	if(!OwnerCharacter.IsValid()) return;

	FVector Loc = Hit.Location - OwnerCharacter->GetMesh()->GetComponentLocation();

	Loc.Z = 0.0f; FRotator Rotation = FRotationMatrix::MakeFromX(Loc).Rotator();
	
	OwnerCharacter->GetAPMoveComponent()->SetAttackRotation(Rotation, RotSpeed);
}

void USkillNumberBase::CharacterRotation_Sector()
{
	if(!OwnerCharacter.IsValid()) return;

	OwnerCharacter->GetAPMoveComponent()->SetAttackRotation(SkillRange_Target->GetActorRotation(), RotSpeed);
}

