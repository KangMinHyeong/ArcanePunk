// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APPillarAttack.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;

UCLASS()
class ARCANEPUNK_API AAPPillarAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPPillarAttack();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void InitPillar(bool bEnd);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	FORCEINLINE bool IsHidden() const {return bHidden;};
	FORCEINLINE bool IsThrown() const {return bThrowed;};
	void ThrowedPillar(FVector & Target);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsHitting() const {return bHitting;};

private:
	void SpawnDamageText(AController* EventInstigator, float Damage, FVector AddLocation);

	UFUNCTION()
  	void OnOverlaping(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnHitting(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void DestroyPillar();

	void SetPillarMaterial();

	FORCEINLINE void HitEnd() {SkinChange(false);};

	void SkinChange(bool Hitting);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PillarMesh;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* PillarSkin;
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* ThrownTrigger;

	UPROPERTY(EditAnywhere)
	float PillarHP = 50.0f;
	float OriginHP;
	
	UPROPERTY(EditAnywhere)
	float ThrowedSpeed = 1500.0f;

	UPROPERTY(EditAnywhere)
	float UpCoeff = 150.0f;

	UPROPERTY(EditAnywhere)
	float PillarCoefficient = 1.5f;

	bool bHidden = false;
	bool bThrowed = false;
	bool bHitting = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DamageTextClass;
	UPROPERTY(EditAnywhere, Category = "Hit")
	FVector DamageTextAddLocation = FVector(0,0,0);

	FTimerHandle DestroyTimerHandle;
	FTimerHandle AppearTimerHandle;
	FTimerHandle HitTimerHandle;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 5.0f;
	UPROPERTY(EditAnywhere)
	float AppearTime = 15.0f;
	
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> Materials;

	UPROPERTY(EditAnywhere)
	float FadeOutSpeed = 3.0f;
	float Opacity = 1.0f;
};
