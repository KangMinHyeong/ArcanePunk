#include "ArcanePunk/Public/Minimap/CMinimapActor.h"
#include "Blueprint/UserWidget.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"

ACMinimapActor::ACMinimapActor()
{
	PrimaryActorTick.bCanEverTick = true;

    FString WidgetBlueprintPath = "/Game/Minimap/WB_Minimap.WB_Minimap_C";

    WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LoadClass<UUserWidget>(nullptr, *WidgetBlueprintPath));
}

void ACMinimapActor::BeginPlay()
{
    Super::BeginPlay();

    if (WidgetInstance)
    {
        WidgetInstance->AddToViewport();
    }
	
}

void ACMinimapActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UMaterialParameterCollection* ParameterCollection = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/Minimap/MP_Minimap.MP_Minimap"));

    if (ParameterCollection)
    {
        AArcanePunkCharacter* Player = nullptr;

        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArcanePunkCharacter::StaticClass(), FoundActors);
        if (FoundActors.Num() > 0)
        {
            Player = Cast<AArcanePunkCharacter>(FoundActors[0]);
        }
        else
        {
            Player = GetWorld()->SpawnActor<AArcanePunkCharacter>(AArcanePunkCharacter::StaticClass(), FVector(100.0f, 0.0f, 100.0f), FRotator::ZeroRotator);
        }

        if (Player)
        {
            UMaterialParameterCollectionInstance* ParameterCollectionInstance = GetWorld()->GetParameterCollectionInstance(ParameterCollection);

            if (ParameterCollectionInstance)
            {
                FName VectorParameterName = FName("Player Location");
                FName ScalarParameterName = FName("Player Yaw");
                FVector VectorParameterValue = Player->GetActorLocation();
                float ScalarParameterValue = Player->GetControlRotation().Yaw;
                float ScalarParameterZeroValue = Player->GetControlRotation().Yaw + 360;
                float Alpha = (ScalarParameterValue < 0.0f) ? 0.0f : 1.0f;
                float SelectedValue = FMath::Lerp(ScalarParameterZeroValue, ScalarParameterValue, Alpha);

                SelectedValue /= 360;

                ParameterCollectionInstance->SetVectorParameterValue(VectorParameterName, VectorParameterValue);
                ParameterCollectionInstance->SetScalarParameterValue(ScalarParameterName, SelectedValue);
            }
        }
    }
}

