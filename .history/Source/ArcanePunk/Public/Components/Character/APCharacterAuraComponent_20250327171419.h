// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APCharacterAuraComponent.generated.h"

class APlayerController;
class UNiagaraSystem;
class UNiagaraComponent;
class AArcanePunkCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPCharacterAuraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAPCharacterAuraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UNiagaraComponent* GetAuraEffect() {return AuraComp;};
	void SetAuraActive(bool NewBool);

private:
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<APlayerController> OwnerPC;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* AuraEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* AuraComp;
};
