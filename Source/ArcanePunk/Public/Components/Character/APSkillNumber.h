// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumber1.h"
#include "Components/Character/SkillNumber/SkillNumber2.h"
#include "Components/Character/SkillNumber/SkillNumber3.h"
#include "Components/Character/SkillNumber/SkillNumber4.h"
#include "Components/Character/SkillNumber/SkillNumber5.h"
#include "Components/Character/SkillNumber/SkillNumber6.h"
#include "Components/Character/SkillNumber/SkillNumber7.h"
#include "Components/Character/SkillNumber/UltSkillNumber_1.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/ActorComponent.h"
#include "APSkillNumber.generated.h"

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

	void BindSkill(ESkillNumber SkillNumber, ESkillTypeState SkillType, ESkillKey WhichKey);
	void BindUltSkill(EUltSkillNumber UltSkillNumber, ESkillTypeState SkillType, ESkillKey WhichKey);
	
	FORCEINLINE USkillNumberBase* GetSkillNumberBase() const {return SkillNumBase;};
	FORCEINLINE USkillNumber1* GetSkillNumber1() const {return SkillNum_1;};
	FORCEINLINE USkillNumber2* GetSkillNumber2() const {return SkillNum_2;};
	FORCEINLINE USkillNumber3* GetSkillNumber3() const {return SkillNum_3;};
	FORCEINLINE USkillNumber4* GetSkillNumber4() const {return SkillNum_4;};
	FORCEINLINE USkillNumber5* GetSkillNumber5() const {return SkillNum_5;};
	FORCEINLINE USkillNumber6* GetSkillNumber6() const {return SkillNum_6;};
	FORCEINLINE USkillNumber7* GetSkillNumber7() const {return SkillNum_7;};

	FORCEINLINE UUltSkillNumber_1* GetUltSkillNumber1() const {return UltSkillNum_1;};

private:
	UPROPERTY()
	USkillNumberBase* SkillNumBase;
	UPROPERTY()
	USkillNumber1* SkillNum_1;
	UPROPERTY()
	USkillNumber2* SkillNum_2; 
	UPROPERTY()
	USkillNumber3* SkillNum_3;
	UPROPERTY()
	USkillNumber4* SkillNum_4;
	UPROPERTY()
	USkillNumber5* SkillNum_5;
	UPROPERTY()
	USkillNumber6* SkillNum_6;
	UPROPERTY()
	USkillNumber7* SkillNum_7;


	UPROPERTY()
	UUltSkillNumber_1* UltSkillNum_1;

};
