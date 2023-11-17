#include "ArcanePunk/Public/Minimap/CMinimapActor.h"
#include "Blueprint/UserWidget.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
<<<<<<< Updated upstream:Source/ArcanePunk/CMinimapActor.cpp
#include "ArcanePunkCharacter.h"
=======
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
>>>>>>> Stashed changes:Source/ArcanePunk/Private/Minimap/CMinimapActor.cpp
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"

ACMinimapActor::ACMinimapActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACMinimapActor::BeginPlay()
{
    Super::BeginPlay();

    FString WidgetBlueprintPath = "/Game/Minimap/WB_Minimap.WB_Minimap_C";

    UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LoadClass<UUserWidget>(nullptr, *WidgetBlueprintPath));
    if (WidgetInstance)
    {

<<<<<<< Updated upstream:Source/ArcanePunk/CMinimapActor.cpp
        // ����Ʈ�� �߰�
=======
>>>>>>> Stashed changes:Source/ArcanePunk/Private/Minimap/CMinimapActor.cpp
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

<<<<<<< Updated upstream:Source/ArcanePunk/CMinimapActor.cpp
                // ���⼭ GetActorRotation ��� Character�� �Լ� ���
=======
>>>>>>> Stashed changes:Source/ArcanePunk/Private/Minimap/CMinimapActor.cpp
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

