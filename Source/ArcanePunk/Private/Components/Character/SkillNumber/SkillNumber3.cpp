
#include "Components/Character/SkillNumber/SkillNumber3.h"

#include "Character/ArcanePunkCharacter.h"
#include "SpawnPoint/APSpawnPointBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void USkillNumber3::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckingOtherSkill();
}

void USkillNumber3::PlaySkill(uint8 SkillType)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;	

	if(Skilling)
	{
		Remove_Skill3();
	}
	else
	{
		OwnerCharacter->SetDoing(true);

		Skilling = true;
		OwnerCharacter->SetbMouseAttack(false);

		CurrentSkillType = SkillType;
		Spawn_Skill3();
	}
	
}

void USkillNumber3::Spawn_Skill3()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;
	OwnerCharacterPC->bShowMouseCursor = false;
	CursorImmediately();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	Skill3_SpawnPoint = GetWorld()->SpawnActor<AAPSpawnPointBase>(OwnerCharacter->GetSkill3_SpawnPont(), OwnerCharacter->GetActorTransform(), SpawnParams);
	if(Skill3_SpawnPoint) Skill3_SpawnPoint->SetSkillType(CurrentSkillType);
	if(Skill3_SpawnPoint) Skill3_SpawnPoint->SetAttackMouse(true);
	if(Skill3_SpawnPoint) OwnerCharacter->SetSkill_SpawnPoint(1);

	Skill3Range_SpawnPoint = GetWorld()->SpawnActor<AAPSpawnPointBase>(OwnerCharacter->GetSkill3Range_SpawnPont(), OwnerCharacter->GetActorTransform(), SpawnParams);
	if(Skill3Range_SpawnPoint) Skill3Range_SpawnPoint->SetRangeLocation();
	if(Skill3Range_SpawnPoint) Skill3Range_SpawnPoint->SetSkillType(CurrentSkillType);

	OwnerCharacter->GetAPSkillHubComponent()->RemoveSkillState();
	OwnerCharacter->SetDoing(false);
	SetComponentTickEnabled(true);
}

void USkillNumber3::CheckingOtherSkill()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	if(CheckSmartKey(3, OwnerCharacter)) 
	{ 
		Cast_Skill3(); return; 
	}
	if(OwnerCharacter->GetDoing() && Skilling)
	{
		Remove_Skill3(); return;
	}
	if(OwnerCharacter->returnState() != ECharacterState::None && Skilling)
	{
		Remove_Skill3(); return;
	}
}

void USkillNumber3::Remove_Skill3()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
	if(!OwnerCharacterPC) return;

	Skilling = false;

	if(Skill3_SpawnPoint) Skill3_SpawnPoint->Destroy();
	if(Skill3Range_SpawnPoint) Skill3Range_SpawnPoint->Destroy();
 
	OwnerCharacter->SetbMouseAttack(true);
	OwnerCharacterPC->bShowMouseCursor = true;
	SetComponentTickEnabled(false);
}

void USkillNumber3::Cast_Skill3()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
	auto OwnerCharacterPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerCharacterPC) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;
	if(!Skill3_SpawnPoint) return;
	
	bool IsOutRange = false; // 스마트키에서 마우스 커서가 스킬 범위 밖에 있는지 체크

	OwnerCharacterPC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	float Distance =  FVector::Distance(OwnerCharacter->GetActorLocation(), HitResult.Location); float Limit = OwnerCharacter->GetSkill3_LimitDist() * (Skill3_SpawnPoint->GetActorScale3D().X / Skill3_SpawnPoint->DefaultSize);
	if(Distance > Limit) 
	{
		if(!CheckSmartKey(3, OwnerCharacter)){UE_LOG(LogTemp, Display, TEXT("Your a")); return;} // 스마트키가 아닌데 바깥범위면 리턴 (빨간색으로 뜸)
		else // 스마트키 On, 바깥범위면 마우스 커서 방향으로 최대사거리 스킬 시전 , 백터 Lerp 계산 
		{
			IsOutRange = true;
			MaxDist = FMath::Lerp(OwnerCharacter->GetActorLocation(), HitResult.Location,  (Limit /(OwnerCharacter->GetActorLocation() - HitResult.Location).Size()));
			UE_LOG(LogTemp, Display, TEXT("Your b"));
		} 
	} 

	OwnerAnim->PlaySkill_3_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SpawnAttackSphere(IsOutRange);
	Remove_Skill3();
}

void USkillNumber3::SpawnAttackSphere(bool IsOutRange)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	bool bStun = false;
	if(Skill3_SpawnPoint) bStun = Skill3_SpawnPoint->GetbStun();

	if(IsOutRange) 
	{
		OwnerCharacter->GetAttackComponent()->NormalAttack(FVector(MaxDist.X, MaxDist.Y, Skill3_SpawnPoint->GetActorLocation().Z - Skill3_SpawnPoint->GetBlockingArea()->GetScaledCapsuleHalfHeight()), false, 1.2f, bStun, 3.0f ,true, Skill3_SpawnPoint->GetBlockingArea()->GetScaledCapsuleRadius());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OwnerCharacter->Skill3_Effect, FVector(MaxDist.X, MaxDist.Y, Skill3_SpawnPoint->GetActorLocation().Z - Skill3_SpawnPoint->GetBlockingArea()->GetScaledCapsuleHalfHeight()), OwnerCharacter->GetActorRotation());
	}
	else 
	{
		OwnerCharacter->GetAttackComponent()->NormalAttack(HitResult.Location+FVector(0,0,25.0f), false, 1.2f, bStun, 3.0f ,true, Skill3_SpawnPoint->GetBlockingArea()->GetScaledCapsuleRadius());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OwnerCharacter->Skill3_Effect, HitResult.Location+FVector(0,0,25.0f), OwnerCharacter->GetActorRotation());
	}
}