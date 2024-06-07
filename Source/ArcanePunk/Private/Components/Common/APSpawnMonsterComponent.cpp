
#include "Components/Common/APSpawnMonsterComponent.h"

#include "SpawnPoint/APSpawnPointBase.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameInstance/APGameInstance.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Kismet/GameplayStatics.h"

UAPSpawnMonsterComponent::UAPSpawnMonsterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPSpawnMonsterComponent::BeginPlay()
{
	Super::BeginPlay();

    SpawnParamsSetting();
}

void UAPSpawnMonsterComponent::SpawnParamsSetting()
{
    if(GetOwner()) SpawnParams.Owner = GetOwner();
    SpawnParams.bNoFail = true;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

void UAPSpawnMonsterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAPSpawnMonsterComponent::SpawnLocation(TSubclassOf<AAPSpawnPointBase> SpawnPointClass, int32 SpawnMonsterNum, TArray<AActor*>& SpawnLocations, USceneComponent* SpawnRoot)
{
    float Location = 0.0f;

    for(int32 i = 0; i<SpawnMonsterNum; i++)
    {
        i%2 == 1 ? Location = Location + i * 200.0f : Location = Location - i * 200.0f;
        int32 NullLocation = 1;
        AAPSpawnPointBase* SpawnPoint = nullptr;
        while(!SpawnPoint)
        {
			if(SpawnRoot) {SpawnPoint = GetWorld()->SpawnActor<AAPSpawnPointBase>(SpawnPointClass, SpawnRoot->GetComponentLocation() + SpawnRoot->GetForwardVector()*400.0f * NullLocation + SpawnRoot->GetRightVector() * Location, SpawnRoot->GetComponentRotation(), SpawnParams);}
            else {SpawnPoint = GetWorld()->SpawnActor<AAPSpawnPointBase>(SpawnPointClass, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector()*400.0f * NullLocation + GetOwner()->GetActorRightVector() * Location, GetOwner()->GetActorRotation(), SpawnParams);}
            NullLocation++;
        }
        SpawnLocations.Add(SpawnPoint);
    }
}

void UAPSpawnMonsterComponent::SpawnLocationRandom(TSubclassOf<AAPSpawnPointBase> SpawnPointClass, int32 SpawnMonsterNum, TArray<AActor*>& SpawnLocations, FVector MinimumRange, FVector MaximumRange)
{   
    while(SpawnMonsterNum != 0)
    {
        float Location_X =  FMath::RandRange(MinimumRange.X, MaximumRange.X); float Location_Y =  FMath::RandRange(MinimumRange.Y, MaximumRange.Y);
        AAPSpawnPointBase* SpawnPoint = GetWorld()->SpawnActor<AAPSpawnPointBase>(SpawnPointClass, FVector(Location_X, Location_Y, 0.0f), GetOwner()->GetActorRotation(), SpawnParams);
        
        if(SpawnPoint) {SpawnLocations.Add(SpawnPoint); SpawnMonsterNum--;}
    }
}

// Location TArray 받아서 스폰
void UAPSpawnMonsterComponent::SpawnMonsterFromLocation(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass, TArray<AActor*>& SpawnLocations, TArray<AEnemy_CharacterBase*>& MonsterArr)
{
    while(!SpawnLocations.IsEmpty())
    {
        auto SpawnMonster = GetWorld()->SpawnActor<AEnemy_CharacterBase>(SpawnMonsterClass, SpawnLocations.Top()->GetActorLocation(), SpawnLocations.Top()->GetActorRotation());
        if(SpawnMonster) PlaySpawnEffect(SpawnMonster->GetMesh()->GetComponentLocation());
        SpawnLocations.Top()->Destroy();
        SpawnLocations.Pop();
        MonsterArr.Add(SpawnMonster);
    }
}

void UAPSpawnMonsterComponent::SpawnMonsterRandomWithoutLocationActor(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass,  uint8 SpawnMonsterNum, FVector MinimumRange, FVector MaximumRange)
{
    while(SpawnMonsterNum != 0)
    {
        float Location_X =  FMath::RandRange(MinimumRange.X, MaximumRange.X); float Location_Y =  FMath::RandRange(MinimumRange.Y, MaximumRange.Y);
        auto SpawnMonster = GetWorld()->SpawnActor<AEnemy_CharacterBase>(SpawnMonsterClass, FVector(Location_X, Location_Y, 0.0f), GetOwner()->GetActorRotation(), SpawnParams);
        if(SpawnMonster) PlaySpawnEffect(SpawnMonster->GetMesh()->GetComponentLocation());
        if(SpawnMonster) SpawnMonsterNum--;
    }
}

void UAPSpawnMonsterComponent::PlaySpawnEffect(FVector Location)
{
    auto GM = Cast<AAPGameModeBattleStage>(UGameplayStatics::GetGameMode(GetWorld())); if(!GM) return;
    auto GI = Cast<UAPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); if(!GI) return;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GM->GetSpawnEffect(),  Location);

    float SpawnSoundVolume = GI->GetGameSoundVolume().MasterVolume * GI->GetGameSoundVolume().EffectVolume * GM->GetSpawnSoundVolume();
    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GM->GetSpawnSound(), Location, FRotator::ZeroRotator, SpawnSoundVolume, 1.0f, Sink);
}

// TArray 없이 스폰