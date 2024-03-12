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
#include "Components/Character/SkillNumber/SkillNumber8.h"
#include "Components/Character/SkillNumber/SkillNumber9.h"
#include "Components/Character/SkillNumber/SkillNumber10.h"
#include "Components/Character/SkillNumber/SkillNumber11.h"
#include "Components/Character/SkillNumber/SkillNumber12.h"
#include "Components/Character/SkillNumber/SkillNumber13.h"
#include "Components/Character/SkillNumber/SkillNumber14.h"
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
	FORCEINLINE USkillNumber8* GetSkillNumber8() const {return SkillNum_8;};
	FORCEINLINE USkillNumber9* GetSkillNumber9() const {return SkillNum_9;};
	FORCEINLINE USkillNumber10* GetSkillNumber10() const {return SkillNum_10;};
	FORCEINLINE USkillNumber11* GetSkillNumber11() const {return SkillNum_11;};
	FORCEINLINE USkillNumber12* GetSkillNumber12() const {return SkillNum_12;};
	FORCEINLINE USkillNumber13* GetSkillNumber13() const {return SkillNum_13;};
	FORCEINLINE USkillNumber14* GetSkillNumber14() const {return SkillNum_14;};

	FORCEINLINE UUltSkillNumber_1* GetUltSkillNumber1() const {return UltSkillNum_1;};

private:
	UPROPERTY(EditAnywhere)
	USkillNumberBase* SkillNumBase;
	UPROPERTY(EditAnywhere)
	USkillNumber1* SkillNum_1;
	UPROPERTY(EditAnywhere)
	USkillNumber2* SkillNum_2; 
	UPROPERTY(EditAnywhere)
	USkillNumber3* SkillNum_3;
	UPROPERTY(EditAnywhere)
	USkillNumber4* SkillNum_4;
	UPROPERTY(EditAnywhere)
	USkillNumber5* SkillNum_5;
	UPROPERTY(EditAnywhere)
	USkillNumber6* SkillNum_6;
	UPROPERTY(EditAnywhere)
	USkillNumber7* SkillNum_7;
	UPROPERTY(EditAnywhere)
	USkillNumber8* SkillNum_8;
	UPROPERTY(EditAnywhere)
	USkillNumber9* SkillNum_9;
	UPROPERTY(EditAnywhere)
	USkillNumber10* SkillNum_10;
	UPROPERTY(EditAnywhere)
	USkillNumber11* SkillNum_11;
	UPROPERTY(EditAnywhere)
	USkillNumber12* SkillNum_12;
	UPROPERTY(EditAnywhere)
	USkillNumber13* SkillNum_13;
	UPROPERTY(EditAnywhere)
	USkillNumber14* SkillNum_14;

	UPROPERTY(EditAnywhere)
	UUltSkillNumber_1* UltSkillNum_1;

};
