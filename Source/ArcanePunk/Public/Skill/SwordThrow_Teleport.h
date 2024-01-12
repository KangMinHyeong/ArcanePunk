// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SwordThrowBase.h"
#include "SwordThrow_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API ASwordThrow_Teleport : public ASwordThrowBase
{
	GENERATED_BODY()
public:
	ASwordThrow_Teleport();

	virtual void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	void ActiveMark(AActor *OtherActor);

protected:
	virtual void DestroySword() override;

};
