
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "APSoundSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FOnChangingSoundVolume, float, float, float, float) // MasterVolume, BGM, Effect, UI

USTRUCT(BlueprintType)
struct FGameSoundVolume
{
	GENERATED_USTRUCT_BODY()

	FGameSoundVolume() {}

	UPROPERTY()
	float MasterVolume = 0.5f;
	UPROPERTY()
	float BGMVolume = 0.5f;
	UPROPERTY()
	float EffectVolume = 0.5f;
	UPROPERTY()
	float UIVolume = 0.5f;
};

UCLASS(Blueprintable, BlueprintType)
class ARCANEPUNK_API UAPSoundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:	
	UAPSoundSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Safe getter with null checks
	static UAPSoundSubsystem* GetSubsystemSafe(UObject* WorldContextObject);

	FORCEINLINE const FGameSoundVolume & GetGameSoundVolume() {return GameSoundVolume;};

	// 후에 값이 변할때마다 업데이트
	FORCEINLINE void SetGameMasterVolume(float NewValue) {GameSoundVolume.MasterVolume = NewValue;};
	FORCEINLINE void SetGameBGMVolume(float NewValue) {GameSoundVolume.BGMVolume = NewValue;};
	FORCEINLINE void SetGameEffectVolume(float NewValue) {GameSoundVolume.EffectVolume = NewValue;};
	FORCEINLINE void SetGameUIVolume(float NewValue) {GameSoundVolume.UIVolume = NewValue;};

	FORCEINLINE USoundBase* GetUIClickSound() const {return UIClickSound;};
	FORCEINLINE USoundBase* GetUIOpenSound() const {return UIOpenSound;};
	FORCEINLINE USoundBase* GetUICloseSound() const {return UICloseSound;};
	FORCEINLINE USoundBase* GetUIChoiceSound() const {return UIChoiceSound;};
	FORCEINLINE USoundBase* GetUIHoverSound() const {return UIHoverSound;};
	FORCEINLINE USoundBase* GetRejectSound() const {return RejectSound;};

	// Play Sound
	static void PlayUIClickSound(UAPSoundSubsystem* SoundGI);
	static void PlayUIOpenSound(UAPSoundSubsystem* SoundGI);
	static void PlayUICloseSound(UAPSoundSubsystem* SoundGI);
	static void PlayUIChoiceSound(UAPSoundSubsystem* SoundGI);
	static void PlayRejectSound(UAPSoundSubsystem* SoundGI);

	static void SpawnEffectSoundAtLocation(UAPSoundSubsystem* SoundGI, USoundBase* Sound, const FVector & Location, float StartTime = 0.0f);
	static UAudioComponent* SpawnBGMSoundAtLocation(UAPSoundSubsystem* SoundGI, USoundBase* Sound, float StartTime = 0.0f);

	UFUNCTION()
	void PlayUIHoverSound();

private:
	// UI Sound
	UPROPERTY()
	USoundBase* UIClickSound;
	UPROPERTY()
	USoundBase* UIOpenSound;
	UPROPERTY()
	USoundBase* UICloseSound;
	UPROPERTY()
	USoundBase* UIChoiceSound;
	UPROPERTY()
	USoundBase* UIHoverSound;
	UPROPERTY()
	USoundBase* RejectSound;

	UPROPERTY()
	FGameSoundVolume GameSoundVolume;

public:
	FOnChangingSoundVolume OnChangingSoundVolume;
};
