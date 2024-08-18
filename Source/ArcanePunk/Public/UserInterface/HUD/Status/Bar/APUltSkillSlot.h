// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/HUD/Status/Bar/APSkillSlot.h"
#include "APUltSkillSlot.generated.h"

UCLASS()
class ARCANEPUNK_API UAPUltSkillSlot : public UAPSkillSlot
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	
};
