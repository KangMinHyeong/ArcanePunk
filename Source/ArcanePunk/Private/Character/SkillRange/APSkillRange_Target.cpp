// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SkillRange/APSkillRange_Target.h"

#include "PlayerController/ArcanePunkPlayerController.h"

void AAPSkillRange_Target::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Targeting)
	{ 
		if(!OwnerPC.IsValid()) return;
		FHitResult HitResult;
		OwnerPC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		if(HitResult.bBlockingHit)
		{
			float Distance =  FVector::Distance(OwnerCharacter->GetActorLocation(), HitResult.Location);
			
			FVector Start = OwnerCharacter->GetMesh()->GetComponentLocation();

			if(Distance <= MaxDistance) 
			{
                // SetActorLocation(HitResult.Location);
				// SetActorLocation(HitResult.ImpactPoint);
				SetActorLocation(FVector(HitResult.Location.X, HitResult.Location.Y, Start.Z));
			}
			else
			{   
                float Alpha = MaxDistance / (HitResult.Location - Start).Size();
                float Location_X = FMath::Lerp(Start.X, HitResult.Location.X, Alpha);
                float Location_Y = FMath::Lerp(Start.Y, HitResult.Location.Y, Alpha);
                SetActorLocation(FVector(Location_X, Location_Y, Start.Z));
			}
			// SetActorLocation(FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z + BlockingArea->GetScaledCapsuleHalfHeight()));
		}
	}
}

void AAPSkillRange_Target::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);

	RangeDecal->SetHiddenInGame(true);
    MaxDistance = MaxDistance * (GetActorScale3D().Z / DefaultSize);

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    OwnerPC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!OwnerPC.IsValid()) return;

    Targeting = true;
    SetActorTickEnabled(true);
}
