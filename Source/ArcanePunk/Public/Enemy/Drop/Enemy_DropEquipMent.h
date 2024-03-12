// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "Enemy_DropEquipMent.generated.h"

class UAPItemBase;
class UDataTable;

UCLASS()
class ARCANEPUNK_API AEnemy_DropEquipMent : public AEnemy_DropBase
{
	GENERATED_BODY()
public:
	AEnemy_DropEquipMent();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDropEquip();

private:

};
