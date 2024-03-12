// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "APNPCAIController.generated.h"

class UBehaviorTree;

UCLASS()
class ARCANEPUNK_API AAPNPCAIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void OnPossess(APawn* InPawn) override;
	
protected:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehavior;	
};
