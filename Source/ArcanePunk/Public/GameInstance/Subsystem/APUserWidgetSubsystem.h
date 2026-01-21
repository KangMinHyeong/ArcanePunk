// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "APUserWidgetSubsystem.generated.h"

// UserWidget 생성 및 제거, 함수 호출 등을 위한 Subsystem

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartFadeIn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndedFadeOut);

//@전방 선언
#pragma region Forward Declaration
class UUserWidget;
class UAPLoadingUI;
class ULoadingFade;
#pragma endregion

UCLASS()
class ARCANEPUNK_API UAPUserWidgetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UAPUserWidgetSubsystem();

#pragma region Getter
// UAPUserWidgetSubsystem Getter 
public:
	static UAPUserWidgetSubsystem* GetSubsystemSafe(UObject* WorldContextObject);
#pragma endregion

#pragma region Func_Create
// UserWidget 생성 
public:
	static void CreateLoadingUI(UObject* WorldContextObject);
	void CreateLoadingUI_Internal(); // 실질 구현

	static void CreateFadeUI(UObject* WorldContextObject, bool bOutIn); // bOutIn == true 면 Out
	void CreateFadeUI_Internal(bool bOutIn); // 실질 구현

#pragma endregion

#pragma region Func_Remove
// UserWidget 제거 
public:
	static void RemoveLoadingUI(UObject* WorldContextObject);
	void RemoveLoadingUI_Internal(); // 실질 구현


#pragma endregion

#pragma region Func_Call
// UserWidget 함수 호출
public:
	static void UpdateLoadingPercent(UObject* WorldContextObject, float Percent);
	void UpdateLoadingPercent_Internal(float Percent); // 실질 구현

#pragma endregion


private:
#pragma region Val_SubClassof
	UPROPERTY()
	TSubclassOf<UUserWidget> LoadingUIClass;
	UPROPERTY()
	TSubclassOf<UUserWidget> FadeUIClass;
#pragma endregion

#pragma region Val_ClassPtr
	TWeakObjectPtr<UAPLoadingUI> LoadingUI;
	TWeakObjectPtr<ULoadingFade> FadeUI;
#pragma endregion
	
#pragma region Val_TimerHandle
	FTimerHandle LoadingTimerHandle;

#pragma endregion

#pragma region Delegate
public:
	FOnStartFadeIn OnStartFadeIn;
	FOnEndedFadeOut OnEndedFadeOut;
#pragma endregion


};
