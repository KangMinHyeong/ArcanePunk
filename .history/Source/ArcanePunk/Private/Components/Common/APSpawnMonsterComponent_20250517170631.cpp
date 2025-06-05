
#include "Components/Common/APSpawnMonsterComponent.h"

#include "GameElements/SpawnPoint/APSpawnPointBase.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameInstance/APGameInstance.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

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
    
    Params.AddIgnoredActor(GetOwner());
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Ground"), Actors);
	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }
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

void UAPSpawnMonsterComponent::SpawnLocationRandom(TSubclassOf<AAPSpawnPointBase> SpawnPointClass, int32 SpawnMonsterNum, TArray<AActor*>& SpawnLocations, const FVector & MinimumRange, const FVector & MaximumRange)
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

void UAPSpawnMonsterComponent::SpawnMonsterRandomWithTriangle(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass,  uint8 SpawnMonsterNum, const FVector & V_1, const FVector & V_2, const FVector & V_3)
{
    int32 loopcnt = 0;
    while(SpawnMonsterNum > 0)
    {
        if(loopcnt > 100) {UE_LOG(LogTemp, Display, TEXT("Your loopcnt")); return;}
        auto Location = GetRandomLocation(V_1, V_2, V_3);
        if(!IsPossibleLocation(Location))
        {
            loopcnt++;
            continue;
        }      
        
        auto SpawnMonster = GetWorld()->SpawnActor<AEnemy_CharacterBase>(SpawnMonsterClass, Location, GetOwner()->GetActorRotation(), SpawnParams);
        if(SpawnMonster) 
        {
            SpawnMonster->SetActorLocation(SpawnMonster->GetActorLocation()+ SpawnMonster->GetActorUpVector() * SpawnMonster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
            PlaySpawnEffect(SpawnMonster->GetMesh()->GetComponentLocation());
            SpawnMonsterNum--;
            SpawnMonster->SetOwnerSpawnVolume(GetOwner());
        }
    }
}

void UAPSpawnMonsterComponent::SpawnMonsterRandomWithSquare(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass, uint8 SpawnMonsterNum, const FVector & V_1, const FVector & V_2, const FVector & V_3, const FVector & V_4)
{
    int32 loopcnt = 0;
    while(SpawnMonsterNum > 0)
    {
        if(loopcnt > 20) {UE_LOG(LogTemp, Display, TEXT("Your loopcnt")); return;}
        bool Check = FMath::RandBool();
        auto Location = GetRandomLocation(V_1, V_2, V_3);
        if(Check) Location = GetRandomLocation(V_3, V_4, V_1);

        if(!IsPossibleLocation(Location))
        {
            loopcnt++;
            continue;
        }
        
        auto SpawnMonster = GetWorld()->SpawnActor<AEnemy_CharacterBase>(SpawnMonsterClass, Location, GetOwner()->GetActorRotation(), SpawnParams);
        if(SpawnMonster) 
        {
            FVector SetLoc = SpawnMonster->GetActorLocation();
            SetLoc.Z = GetOwner()->GetActorLocation().Z + SpawnMonster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 12.5f;
            SpawnMonster->SetActorLocation(SetLoc);
            PlaySpawnEffect(SpawnMonster->GetMesh()->GetComponentLocation());
            SpawnMonsterNum--;
            SpawnMonster->SetOwnerSpawnVolume(GetOwner());
        }
    }
}

void UAPSpawnMonsterComponent::SpawnMonsterRandomWithLocation(TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass, uint8 SpawnMonsterNum, const FVector & SpawnLocation)
{
    while(SpawnMonsterNum > 0)
    {
        FRotator rot = GetOwner()->GetActorRotation(); rot.Yaw += 180.0f;
        auto SpawnMonster = GetWorld()->SpawnActor<AEnemy_CharacterBase>(SpawnMonsterClass, SpawnLocation, rot, SpawnParams);
        if(SpawnMonster) 
        {
            SpawnMonster->SetActorLocation(SpawnMonster->GetActorLocation()+ SpawnMonster->GetActorUpVector() * SpawnMonster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
            PlaySpawnEffect(SpawnMonster->GetMesh()->GetComponentLocation());
            SpawnMonsterNum--;
            SpawnMonster->SetOwnerSection(GetOwner());
        }
    }
}

void UAPSpawnMonsterComponent::PlaySpawnEffect(const FVector & Location)
{
    auto GM = Cast<AAPGameModeBattleStage>(UGameplayStatics::GetGameMode(GetWorld())); if(!GM) return;
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GM->GetSpawnEffect(),  Location);

    UAPSoundSubsystem::SpawnEffectSoundAtLocation(UAPGameInstance::GetSoundGI(GetWorld()) , GM->GetSpawnSound(), Location, Sink);
}

FVector UAPSpawnMonsterComponent::GetRandomLocation(const FVector & V_1, const FVector & V_2, const FVector & V_3) const
{
    float r1 = FMath::RandRange(0.0f, 1.0f);
    float r2 = FMath::RandRange(0.0f, 1.0f);

    if (r1 + r2 > 1.0f)
    {
        r1 = 1.0f - r1;
        r2 = 1.0f - r2;
    }

    float A = 1.0f - r1 - r2;
    float B = r1;
    float C = r2;

    return FVector(
        A * V_1.X + B * V_2.X + C * V_3.X,
        A * V_1.Y + B * V_2.Y + C * V_3.Y,
        V_1.Z
    );
}

// TArray 없이 스폰

bool UAPSpawnMonsterComponent::IsPossibleLocation(const FVector & Location)
{
    FCollisionShape Sphere = FCollisionShape::MakeSphere(120.0f);  

    TArray<FHitResult> HitResult;
    bool bResult = GetWorld()->SweepMultiByChannel(HitResult, 
    Location, 
    Location, 
    FQuat::Identity,   
    ECC_Camera, 
    Sphere,
    Params);// 타격 판정 인자 Params 인자 추가

    FColor color = FColor::Green;
    if(HitResult.Num() > 0) {color = FColor::Red; bResult = true;}

    // DrawDebugSphere(GetWorld(),
	// 		Location,
	// 		120.0f,
	// 		12,
	// 		color,
	// 		false,
	// 		5.0f,
	// 		0,
	// 		10);
    return !bResult;
}
