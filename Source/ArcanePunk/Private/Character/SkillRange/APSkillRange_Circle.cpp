// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SkillRange/APSkillRange_Circle.h"

#include "PlayerController/ArcanePunkPlayerController.h"

void AAPSkillRange_Circle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Targeting)
	{ 
		if(!OwnerPC) return;
		FHitResult HitResult;
		OwnerPC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		if(HitResult.bBlockingHit)
		{
			TargetDistance =  FVector::Distance(OwnerCharacter->GetActorLocation(), HitResult.Location);
            TargetDistance = FMath::Clamp(TargetDistance, Wide, MaxDistance);

			RangeDecal->DecalSize = FVector(5.0f, TargetDistance, TargetDistance);
            RangeDecal->CreateDynamicMaterialInstance()->SetScalarParameterValue(TEXT("InCircle"), (TargetDistance - Wide)/(2.0f*TargetDistance));
		}
	}
}

void AAPSkillRange_Circle::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);
    
    MaxDistance = MaxDistance * (GetActorScale3D().Z / DefaultSize);
    
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter) return;
    OwnerPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerPC) return;

    Targeting = true;
    SetActorTickEnabled(true);
}
