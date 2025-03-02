// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APGhostTrail.generated.h"

class UPoseableMeshComponent;
class UMaterialInstance;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AAPGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPGhostTrail();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void InitPose(USkeletalMeshComponent* OwnerCharacterMesh);
	void InitSkillPose(USkeletalMeshComponent* OwnerCharacterMesh, float TraceTime);
	void InitSkillPoseFirst(USkeletalMeshComponent* OwnerCharacterMesh, float TraceTime);
private:
	void OnDestory();
	void SetVisibility();
	
private:
	UPROPERTY(EditAnywhere)
	UPoseableMeshComponent* PoseableMesh;	//메쉬의 포즈정보만 복사해오는 컴포넌트

	UPROPERTY(EditAnywhere)
	UMaterialInstance* GhostMaterial;	

	TArray<UMaterialInstanceDynamic*> Materials;	//매테리얼은 한 메쉬당 한개이므로 여러개 생성
	
	UPROPERTY(EditAnywhere)
	float Opacity = 5.0f;

	UPROPERTY(EditAnywhere)
	float DefaultOpacity = 0.75f;

	UPROPERTY(EditAnywhere)
	float FadeOutSpeed = 2.0f;

	FTimerHandle TimerHandle;

	bool bSkillTrail = false;

	bool bRunTrail = false;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
};
