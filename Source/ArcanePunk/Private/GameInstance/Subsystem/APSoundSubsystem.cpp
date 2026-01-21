
#include "GameInstance/Subsystem/APSoundSubsystem.h"

#include "Kismet/GameplayStatics.h"

UAPSoundSubsystem::UAPSoundSubsystem()
{
    static ConstructorHelpers::FObjectFinder<USoundBase> Click_Sound(TEXT("/Game/Asset/Sound/UI/UI_NormalClick.UI_NormalClick"));
	if(Click_Sound.Succeeded()) UIClickSound = Click_Sound.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> UIOpen_Sound(TEXT("/Game/Asset/Sound/UI/UI_Store_Open.UI_Store_Open"));
	if(UIOpen_Sound.Succeeded()) UIOpenSound = UIOpen_Sound.Object;

    static ConstructorHelpers::FObjectFinder<USoundBase> UIClose_Sound(TEXT("/Game/Asset/Sound/UI/UI_Store_Close.UI_Store_Close"));
	if(UIClose_Sound.Succeeded()) UICloseSound = UIClose_Sound.Object;

    static ConstructorHelpers::FObjectFinder<USoundBase> Choice_Sound(TEXT("/Game/Asset/Sound/UI/UI_ChoiceClick.UI_ChoiceClick"));
	if(Choice_Sound.Succeeded()) UIChoiceSound = Choice_Sound.Object;

    static ConstructorHelpers::FObjectFinder<USoundBase> Hover_Sound(TEXT("/Game/Asset/Sound/UI/UI_Hovering.UI_Hovering"));
	if(Hover_Sound.Succeeded()) UIHoverSound = Hover_Sound.Object;

    static ConstructorHelpers::FObjectFinder<USoundBase> Reject_Sound(TEXT("/Game/Asset/Sound/UI/UI_Rejcet.UI_Rejcet"));
	if(Reject_Sound.Succeeded()) RejectSound = Reject_Sound.Object;
    
}

void UAPSoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UAPSoundSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

UAPSoundSubsystem* UAPSoundSubsystem::GetSubsystemSafe(UObject* WorldContextObject)
{
    if (!WorldContextObject) return nullptr;

    UWorld* World = WorldContextObject->GetWorld();
    if (!World) return nullptr;

    UGameInstance* GI = World->GetGameInstance();
    if (!GI) return nullptr;

    return GI->GetSubsystem<UAPSoundSubsystem>();
}

void UAPSoundSubsystem::PlayUIClickSound(UAPSoundSubsystem* SoundGI)
{
    if(SoundGI)
    UGameplayStatics::SpawnSound2D(SoundGI->GetWorld(), SoundGI->GetUIClickSound(), SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().UIVolume);
}

void UAPSoundSubsystem::PlayUIOpenSound(UAPSoundSubsystem* SoundGI)
{
    if(SoundGI)
    UGameplayStatics::SpawnSound2D(SoundGI->GetWorld(), SoundGI->GetUIOpenSound(), SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().UIVolume);
}

void UAPSoundSubsystem::PlayUICloseSound(UAPSoundSubsystem *SoundGI)
{
    if(SoundGI)
    UGameplayStatics::SpawnSound2D(SoundGI->GetWorld(), SoundGI->GetUICloseSound(), SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().UIVolume);
}

void UAPSoundSubsystem::PlayUIChoiceSound(UAPSoundSubsystem* SoundGI)
{
    if(SoundGI)
    UGameplayStatics::SpawnSound2D(SoundGI->GetWorld(), SoundGI->GetUIChoiceSound(), SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().UIVolume);
}

void UAPSoundSubsystem::PlayRejectSound(UAPSoundSubsystem* SoundGI)
{
    if(SoundGI)
    UGameplayStatics::SpawnSound2D(SoundGI->GetWorld(), SoundGI->GetRejectSound(), SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().UIVolume);
}

void UAPSoundSubsystem::SpawnEffectSoundAtLocation(UAPSoundSubsystem *SoundGI, USoundBase *Sound, const FVector &Location, float StartTime)
{
    if(!SoundGI) return;
 
	float SoundVolume = SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().EffectVolume;
	UGameplayStatics::SpawnSoundAtLocation(SoundGI->GetWorld(), Sound, Location, FRotator::ZeroRotator, SoundVolume, 1.0f, StartTime);
}

UAudioComponent* UAPSoundSubsystem::SpawnBGMSoundAtLocation(UAPSoundSubsystem *SoundGI, USoundBase *Sound, float StartTime)
{
    if(!SoundGI) return nullptr;
 
	float SoundVolume = SoundGI->GetGameSoundVolume().MasterVolume * SoundGI->GetGameSoundVolume().BGMVolume;
	return UGameplayStatics::SpawnSound2D(SoundGI->GetWorld(), Sound, SoundVolume, 1.0f, StartTime);
}

void UAPSoundSubsystem::PlayUIHoverSound()
{
    UGameplayStatics::SpawnSound2D(GetWorld(), UIHoverSound, GameSoundVolume.MasterVolume * GameSoundVolume.UIVolume);
}