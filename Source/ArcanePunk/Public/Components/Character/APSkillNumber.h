// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APSkillNumber.generated.h"

class USkillNumber1;
class USkillNumber2;
class USkillNumber3;
class USkillNumber4;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillNumber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSkillNumber();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindSkill(uint8 SkillNumber, uint8 SkillType);

	FORCEINLINE USkillNumber1* GetSkillNumber1() const {return SkillNum_1;};
	FORCEINLINE USkillNumber2* GetSkillNumber2() const {return SkillNum_2;};
	FORCEINLINE USkillNumber3* GetSkillNumber3() const {return SkillNum_3;};
	FORCEINLINE USkillNumber4* GetSkillNumber4() const {return SkillNum_4;};
private:
	UPROPERTY()
	USkillNumber1* SkillNum_1;
	UPROPERTY()
	USkillNumber2* SkillNum_2; 
	UPROPERTY()
	USkillNumber3* SkillNum_3;
	UPROPERTY()
	USkillNumber4* SkillNum_4;
};
