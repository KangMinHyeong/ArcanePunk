
#include "GameInstance/APGameInstance.h"

#include "Save/APSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Skill/SkillDataManager.h"
#include "GameElements/Volume/SpawnVolume/APSpawnVolume.h"

void UAPGameInstance::Init()
{
    Super::Init();

    APSaveGameData = NewObject<UAPSaveGame>();

    OnSkillEnhanceDataUpdate.AddUObject(this, &UAPGameInstance::UpdateSkillEnhanceData);
    OnSkillEnhanceDataClear.AddUObject(this, &UAPGameInstance::ClearSkillEnhanceData);

    USkillDataManager::GetInstance()->ReadSkillData();
}

void UAPGameInstance::InitData()
{
    SpawnVolumes.Empty();
}

void UAPGameInstance::UpdateSkillEnhanceData(ESkillKey UpdateSkillKey, FSkillAbilityNestingData UpdateSkillAbilityNestingData)
{
    switch (UpdateSkillKey)
    {
        case ESkillKey::Q:
        QSkillAbilityNestingData = UpdateSkillAbilityNestingData;
        break;

        case ESkillKey::E:
        ESkillAbilityNestingData = UpdateSkillAbilityNestingData;
        break;

        case ESkillKey::R:
        RSkillAbilityNestingData = UpdateSkillAbilityNestingData;
        break;
    }
}

void UAPGameInstance::ClearSkillEnhanceData(ESkillKey UpdateSkillKey)
{
    switch (UpdateSkillKey)
    {
        case ESkillKey::Q:
        QSkillAbilityNestingData = {};
        break;

        case ESkillKey::E:
        ESkillAbilityNestingData = {};
        break;

        case ESkillKey::R:
        RSkillAbilityNestingData = {};
        break;
    }
}

int32 UAPGameInstance::CheckGoldAmount()
{
    int32 Amount = 1;
    float PlusPercent = FMath::RandRange(0.0f, 100.0f);
    if(PlusPercent <= GoldPlusPercent) Amount++;
    OnGettingGold.Broadcast(Amount);

    return Amount;
}

void UAPGameInstance::OpenLevel(const UObject * WorldContextObject, const FName LevelName)
{
    SavedRealTime = 0.0f;
    UGameplayStatics::OpenLevel(WorldContextObject, LevelName);
}

UAPSoundSubsystem *UAPGameInstance::GetSoundGI(UObject *WorldContextObject)
{
    if (UAPGameInstance* GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
    {
        return Cast<UAPSoundSubsystem>(GI->GetSubsystemBase(UAPSoundSubsystem::StaticClass()));
    }

    return nullptr;
}

UAPDataTableSubsystem *UAPGameInstance::GetDataTableGI(UObject *WorldContextObject)
{
    if (UAPGameInstance* GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
    {
        return Cast<UAPDataTableSubsystem>(GI->GetSubsystemBase(UAPDataTableSubsystem::StaticClass()));
    }

    return nullptr;
}

UAPSettingSubsystem *UAPGameInstance::GetSettingGI(UObject *WorldContextObject)
{
    if (UAPGameInstance* GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
    {
        return Cast<UAPSettingSubsystem>(GI->GetSubsystemBase(UAPSettingSubsystem::StaticClass()));
    }

    return nullptr;
}

void UAPGameInstance::RegisterSpawnVolume(FName SpawnDataID, AAPSpawnVolume* SpawnVolume)
{
    UE_LOG(LogTemp, Display, TEXT("Your RegisterSpawnVolume"));
    SpawnVolumes.Add({SpawnDataID, SpawnVolume});
}

AAPSpawnVolume* UAPGameInstance::FindSpawnVolume(FName SpawnDataID)
{
    if(SpawnVolumes.Contains(SpawnDataID))
    {
        UE_LOG(LogTemp, Display, TEXT("Your Contains SpawnDataID"));
        return SpawnVolumes[SpawnDataID];
    }

    UE_LOG(LogTemp, Display, TEXT("Your not SpawnDataID"));
    return nullptr;
}