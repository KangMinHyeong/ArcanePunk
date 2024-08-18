// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnemyBaseAIController.generated.h"

class UBehaviorTree;
class UCrowdFollowingComponent;
struct FEnvQueryResult;

UCLASS()
class ARCANEPUNK_API AEnemyBaseAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyBaseAIController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnPossess(APawn* InPawn) override;
	bool GetEyeSightTrace();
	bool GetDistanceTrace();

	void UnPossessing();
	
protected:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehavior;

	UPROPERTY(EditAnywhere)
	bool EyeSightTrace = true;

	UPROPERTY(EditAnywhere)
	bool DistanceTrace = true;

	UPROPERTY(EditAnywhere)
	UCrowdFollowingComponent* CrowdFollowComp;

};
