// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AlterEgoAI.generated.h"

class UBehaviorTree;

UCLASS()
class ARCANEPUNK_API AAlterEgoAI : public AAIController
{
	GENERATED_BODY()
public:
	virtual void OnPossess(APawn* InPawn) override;
	
protected:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehavior;		
};
