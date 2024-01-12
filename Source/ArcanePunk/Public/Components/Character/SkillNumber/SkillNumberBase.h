// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/ActorComponent.h"
#include "SkillNumberBase.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API USkillNumberBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillNumberBase();

protected:
	virtual void BeginPlay() override;

	void SetMouseCursor(AArcanePunkPlayerController *PC, ECursorType NewCursor);
	void CursorImmediately();
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PlaySkill(uint8 SkillType = 0);	
	bool CheckSmartKey(uint8 SkillType, class AArcanePunkCharacter* OwnerCharacter);

protected:
	UPROPERTY()
	uint8 CurrentSkillType = 0;

	UPROPERTY()
	FHitResult HitResult;
};
