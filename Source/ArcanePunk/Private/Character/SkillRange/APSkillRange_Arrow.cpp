
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

void AAPSkillRange_Arrow::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);
    if(SkillAbility.Contains(ESkillAbility::Stun)) bStun = true;
    
    MaxDistance = MaxDistance * (GetActorScale3D().Z / DefaultSize);

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
    OwnerPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerPC) return;

    Targeting = true;
    SetActorTickEnabled(true);
}

void AAPSkillRange_Arrow::ObstacleCheck()
{
    if(!OwnerCharacter) return; if(!OwnerPC) return;
	FHitResult HitResult;
	OwnerPC->GetHitResultUnderCursor(ECC_GameTraceChannel3, false, HitResult);
    FVector HitPoint;
    if(HitResult.bBlockingHit)
	{
        HitPoint = FVector(HitResult.Location.X, HitResult.Location.Y, OwnerCharacter->GetActorLocation().Z);
    }
    else {return;}

    HitPoint = HitPoint - GetActorLocation();  HitPoint = HitPoint/HitPoint.Size(); 

    FHitResult Hits;
    FCollisionShape Shape = FCollisionShape::MakeSphere(RangeDecal->DecalSize.Y/2.0f);

    if(GetWorld()->SweepSingleByChannel(Hits, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + HitPoint * MaxDistance * 2.0f,FQuat::Identity , ECC_Visibility, Shape))
    {   
        // FVector Dist = Hits.Location - OwnerCharacter->GetActorLocation();
        FVector Dist = FVector(Hits.GetActor()->GetActorLocation().X, Hits.GetActor()->GetActorLocation().Y, OwnerCharacter->GetActorLocation().Z);
        Dist = Dist -OwnerCharacter->GetActorLocation();
        TargetDistance =  (FVector(Dist.X, Dist.Y, 0.0f)).Size() / (2.0f * (GetActorScale3D().Z / DefaultSize));
    }
    else
    {
        TargetDistance =  MaxDistance;
    }

    RangeDecal->SetRelativeLocation(FVector(TargetDistance, 0.0f, 0.0f));
    
    HitPoint.Z = 0.0f;
    SetActorRotation(FRotationMatrix::MakeFromX(HitPoint).Rotator());

    RangeDecal->DecalSize.Z = TargetDistance;
    
}

