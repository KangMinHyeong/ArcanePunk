
#include "Character/SkillRange/APSkillRange_Arrow.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

AAPSkillRange_Arrow::AAPSkillRange_Arrow()
{

}

void AAPSkillRange_Arrow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Targeting)
	{ 
		ObstacleCheck();
	}
}

void AAPSkillRange_Arrow::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    // if(SkillAbility.Contains(ESkillAbility::Stun)) bStun = true;
    
    MaxDistance = MaxDistance * (GetActorScale3D().Z / DefaultSize);

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    OwnerPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerPC.IsValid()) return;

    Targeting = true;
    SetActorTickEnabled(true);
}

void AAPSkillRange_Arrow::ObstacleCheck()
{
    if(!OwnerCharacter.IsValid()) return; if(!OwnerPC.IsValid()) return;
    
	FHitResult HitResult;
	OwnerPC->GetHitResultUnderCursor(ECC_GameTraceChannel3, false, HitResult);
    FVector HitPoint;
    if(HitResult.bBlockingHit)
	{
        HitPoint = FVector(HitResult.Location.X, HitResult.Location.Y, OwnerCharacter->GetActorLocation().Z);
    }
    else {return;}

    HitPoint = HitPoint - GetActorLocation(); HitPoint = HitPoint/HitPoint.Size();  

    FHitResult Hits;
    FCollisionShape Shape = FCollisionShape::MakeSphere(RangeDecal->DecalSize.Y/2.0f);

    TargetDistance =  MaxDistance;

    RangeDecal->SetRelativeLocation(FVector(TargetDistance, 0.0f, 0.0f));
    
    HitPoint.Z = 0.0f; RotVector = HitPoint; 
    SetActorRotation(FRotationMatrix::MakeFromX(HitPoint).Rotator());

    if(SkillRangeType == ESkillRangeType::Arrow) RangeDecal->DecalSize.Z = TargetDistance;
    
}

