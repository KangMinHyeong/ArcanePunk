#include "AnimNotifyState/APANS_AllowChainAction.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogANS_ChainAction)

//@Defualt Setting
#pragma region Default Setting
UAPANS_AllowChainAction::UAPANS_AllowChainAction(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAPANS_AllowChainAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
}

void UAPANS_AllowChainAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}
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
