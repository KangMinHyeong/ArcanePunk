// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "APAuraComponent.generated.h"

class AArcanePunkPlayerController;

UCLASS()
class ARCANEPUNK_API UAPAuraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:	
	UAPAuraComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TWeakObjectPtr<AArcanePunkPlayerController> OwnerPC;	
};
