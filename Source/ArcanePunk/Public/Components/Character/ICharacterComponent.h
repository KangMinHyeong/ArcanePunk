// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacterComponent.generated.h"

class AAPCharacterBase;

// 더미 인터페이스 클래스 (언리얼 리플렉션 시스템용)
UINTERFACE(MinimalAPI, Blueprintable)
class UCharacterComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 캐릭터 컴포넌트 인터페이스
 * 모든 캐릭터 컴포넌트가 구현해야 하는 기본 인터페이스
 */
class ARCANEPUNK_API ICharacterComponent
{
	GENERATED_BODY()

public:
	/**
	 * 컴포넌트 초기화
	 * @param OwnerCharacter - 이 컴포넌트를 소유한 캐릭터
	 */
	virtual void InitializeComponent(AAPCharacterBase* OwnerCharacter) = 0;

	/**
	 * 컴포넌트 활성화/비활성화
	 * @param bEnabled - true면 활성화, false면 비활성화
	 */
	virtual void SetComponentEnabled(bool bEnabled) = 0;

	/**
	 * 컴포넌트 상태 확인
	 * @return true면 활성화, false면 비활성화
	 */
	virtual bool IsComponentEnabled() const = 0;
};
