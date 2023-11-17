#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMinimapActor.generated.h"

UCLASS()
class ARCANEPUNK_API ACMinimapActor : public AActor
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap")
		UClass* MinimapWidgetClass;

public:	
	ACMinimapActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
