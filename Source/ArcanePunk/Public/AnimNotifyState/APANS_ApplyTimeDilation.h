#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameInstance/Subsystem/APTimeManipulationSubsystem.h"

#include "APANS_ApplyTimeDilation.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogANS_TimeDilation, Log, All);

/**
 *	@UAPANS_ApplyTimeDilation
 *
 *	애니메이션 특정 재생 구간 동안 글로벌 타임 딜레이션을 구현합니다.
 */
UCLASS(meta = (DisplayName = "타임 딜레이션 적용"))
class ARCANEPUNK_API UAPANS_ApplyTimeDilation : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    UAPANS_ApplyTimeDilation();

protected:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
    virtual FString GetNotifyName_Implementation() const override;

private:
    UPROPERTY(EditAnywhere, Category = "타임 딜레이션", meta = (DisplayName = "글로벌 적용"))
    bool bApplyGlobalDilation = true;

private:
    UPROPERTY(EditAnywhere, Category = "타임 딜레이션", meta = (DisplayName = "딜레이션 설정"))
    FTimeDilationSettings DilationSettings;
};