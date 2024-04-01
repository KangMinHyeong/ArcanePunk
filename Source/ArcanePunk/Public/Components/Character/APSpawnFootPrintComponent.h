// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APSpawnFootPrintComponent.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSpawnFootPrintComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSpawnFootPrintComponent();

protected:
	virtual void BeginPlay() override;

	void SpawnSound(USoundBase* Sound, FVector Location);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnFootPrint(bool LeftFoot, bool Step = true); // 풋프린트 생성

private:
	TSubclassOf<AActor> GetFootClass(bool Left);

private:
	TWeakObjectPtr<UNiagaraComponent> RunEffect;

	int32 Num = -1;

	UPROPERTY(EditAnywhere)
	float SoundCoefficient = 10.0f;

	UPROPERTY(EditAnywhere)
	float UpCoeff = 5.0f;

	UPROPERTY(EditAnywhere)
	float BackCoeff = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	TSubclassOf<AActor> LeftFootClass;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	TSubclassOf<AActor> RightFootClass;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	TArray< UNiagaraSystem*> StepEffect;

	UPROPERTY(EditAnywhere, Category = "Foot Print")
	TArray< USoundBase* > StepSound;
};
