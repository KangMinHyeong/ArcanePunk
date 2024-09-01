// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APTransparentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPTransparentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPTransparentComponent();

	FORCEINLINE void SetDestroy(bool NewBool) {bDestroy = NewBool;};

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FadeOut();
	void FadeIn();

	void SetMeshMaterials(UMeshComponent* Meshes);

private:
	bool bFadeOut = false;	
	float Opacity = 1.0f;

	UPROPERTY(EditAnywhere)
	float FadeOutLimit = 0.15f;
	UPROPERTY(EditAnywhere)
	float FadeSpeed = 0.15f;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> Materials;

	UPROPERTY(EditAnywhere)
	bool bAuto = true;
	UPROPERTY(EditAnywhere)
	bool bDestroy = false;
};
