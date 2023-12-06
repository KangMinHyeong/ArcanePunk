
#include "Components/Character/APAttackComponent.h"

#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

UAPAttackComponent::UAPAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAPAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAPAttackComponent::StartAttack_A(bool & bCanMove)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	if(bAttack_A)
	{
		if(!FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo)) return;
		if (CanCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ComboAttackStart();
		OwnerAnim->PlayAttack_A_Montage();
		OwnerAnim->JumpToComboSection(CurrentCombo);
		bAttack_A = true;
		bCanMove = false;
	}
}

void UAPAttackComponent::StartAttack_B(bool &bCanMove)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	if(bAttack_B) return;
	bAttack_B = true;
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), Attack_Sound, GetActorLocation(), E_SoundScale);
	OwnerAnim->PlayAttack_B_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UAPAttackComponent::SetAttack_A(bool NewBool)
{
	bAttack_A = NewBool;
}

void UAPAttackComponent::SetAttack_B(bool NewBool)
{
	bAttack_B = NewBool;
}

bool UAPAttackComponent::GetAttack_A()
{
    return bAttack_A;
}

bool UAPAttackComponent::GetAttack_B()
{
    return bAttack_B;
}

void UAPAttackComponent::ComboAttackStart()
{
	CanCombo = true;
	IsComboInputOn = false;
	if(!FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1)) return;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void UAPAttackComponent::ComboAttackEnd()
{
	CanCombo = false;
	IsComboInputOn = false;
	CurrentCombo = 0;
}

void UAPAttackComponent::ComboCheck()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	CanCombo = false;
	if (IsComboInputOn)
	{
		ComboAttackStart();
		OwnerAnim->JumpToComboSection(CurrentCombo);
	}
}

//AttackTrace 코드 시작
bool UAPAttackComponent::AttackTrace(FHitResult &HitResult, FVector &HitVector, FVector Start, bool CloseAttack)
{
	FRotator Rotation = GetOwner()->GetActorRotation();
	FVector End = Start;
	if(CloseAttack) End = End + Rotation.Vector() * MaxDistance + FVector::UpVector* 25.0f; // 캐릭터와 몬스터의 높이차가 심하면 + FVector::UpVector* MonsterHigh
	else End = End + FVector::UpVector* 100.0f;

	// 아군은 타격 판정이 안되게 하는 코드
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);
	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }    
	
	HitVector = -Rotation.Vector();

	//DrawDebugSphere(GetWorld(), End, AttackRadius, 10, FColor::Green, false, 5);

	FCollisionShape Sphere;
	if(CloseAttack) Sphere = FCollisionShape::MakeSphere(AttackRadius);
	else Sphere = FCollisionShape::MakeSphere(AttackRadius*1.25);

	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);// 타격 판정 인자 Params 인자 추가
}

void UAPAttackComponent::NormalAttack(FVector Start, bool CloseAttack, float Multiple)
{
	auto Character = Cast<AArcanePunkCharacter>(GetOwner());
	if(!Character) return;
	if(Character->GetCurrentCharacterState() != 0) return;

	float Damage = Character->GetMyPlayerStatus().ATK * Multiple;
	FHitResult HitResult;
	FVector HitVector;
	bool Line = AttackTrace(HitResult, HitVector, Start, CloseAttack);
	if(Line)
	{
		if(AActor* Actor = HitResult.GetActor())
		{
			FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
			AController* MyController = Cast<AController>(Character->GetController());
			if(!MyController) return;
			Actor->TakeDamage(Damage, myDamageEvent, MyController, GetOwner());
		}
	}
}
//AttackTrace 코드 끝