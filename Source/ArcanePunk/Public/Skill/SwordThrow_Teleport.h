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
	virtual void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	void ActiveMark(AActor *OtherActor);

protected:
	virtual void DestroySword() override;
	
private:
	UPROPERTY(EditAnywhere)
	float MarkDistance = -50.0f;
};
