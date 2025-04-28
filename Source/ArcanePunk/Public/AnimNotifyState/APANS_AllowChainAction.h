#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "APANS_AllowChainAction.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogANS_ChainAction, Log, All)

//@전방 선언
#pragma region Forward Declaration
#pragma endregion

//@열거형
#pragma region Enums
#pragma endregion

//@구조체
#pragma region Structs
#pragma endregion

//@이벤트/델리게이트
#pragma region Delegates
#pragma endregion

/**
 *	@UAPANS_AllowChainAction
 * 
 *	애니메이션 재생 중 연쇄 동작 허용 구간을 설정하는 Anim Notify State.
 */
UCLASS(meta = (DisplayName = "연쇄 동작 허용 구간 설정"))
class ARCANEPUNK_API UAPANS_AllowChainAction : public UAnimNotifyState
{

//@친추 클래스
#pragma region Friend Class
#pragma endregion

	GENERATED_BODY()

//@Defualt Setting
#pragma region Default Setting
public:
	UAPANS_AllowChainAction(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
#pragma endregion

//@Property/Info...etc
#pragma region Property or Subwidgets or Infos...etc
#pragma endregion

//@Delegates
#pragma region Delegates
#pragma endregion

//@Callbacks
#pragma region Callbacks
#pragma endregion

//@Utility(Setter, Getter,...etc)
#pragma region Utility
#pragma endregion
	
};
