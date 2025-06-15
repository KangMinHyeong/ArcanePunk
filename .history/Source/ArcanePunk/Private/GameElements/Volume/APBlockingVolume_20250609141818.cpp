
#include "GameElements/Volume/APBlockingVolume.h"

#include "Components/BoxComponent.h"
#include "AI/NavigationModifier.h"
#include "NavAreas/NavArea_Null.h"

AAPBlockingVolume::AAPBlockingVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BlockingVolum = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingVolum"));
	SetRootComponent(BlockingVolum);
	BlockingVolum->SetCanEverAffectNavigation(true); // NavMesh에 영향 미치도록 설정

	// NavModifierComponent 생성
	NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
	NavModifier->SetupAttachment(BlockingVolum);
	NavModifier->SetAreaClass(UNavArea_Null::StaticClass()); // 이 영역은 NavMesh에서 제외
}

void AAPBlockingVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPBlockingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

