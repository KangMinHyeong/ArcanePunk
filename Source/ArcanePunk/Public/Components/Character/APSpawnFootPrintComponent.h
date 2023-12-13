// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APSpawnFootPrintComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSpawnFootPrintComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSpawnFootPrintComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnFootPrint(bool LeftFoot); // 풋프린트 생성
};
