
#include "GameElements/Collision/APLimitCameraArea.h"

#include "Components/BoxComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/SpringArmComponent.h"

AAPLimitCameraArea::AAPLimitCameraArea()
{
	PrimaryActorTick.bCanEverTick = true;

	AreaTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaTrigger"));
	
	SetRootComponent(AreaTrigger);
}

void AAPLimitCameraArea::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	AreaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAPLimitCameraArea::OnOverlap);
	AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &AAPLimitCameraArea::OnOverlapEnd);
}

void AAPLimitCameraArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Player.IsValid())
	{
		if(!X_Limit && !Y_Limit) {return;}

		if(X_Limit) 
		{
			float Add = FixedSpringArmLocation.Y - Player->GetActorLocation().Y;
			Player->GetMySpringArm()->TargetOffset.Y = Add;
		}

		if(Y_Limit) 
		{
			float Add = FixedSpringArmLocation.X - Player->GetActorLocation().X;
			Player->GetMySpringArm()->TargetOffset.X = Add;
		}
	}
}

void AAPLimitCameraArea::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(Cast<AArcanePunkCharacter>(OtherActor))
	{
		Player = Cast<AArcanePunkCharacter>(OtherActor);
		FixedSpringArmLocation = Player->GetMySpringArm()->GetComponentLocation();
		SetActorTickEnabled(true);
	}
}

void AAPLimitCameraArea::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if(Cast<AArcanePunkCharacter>(OtherActor))
	{
		SetActorTickEnabled(false);
		Player = Cast<AArcanePunkCharacter>(OtherActor);

		Player->GetMySpringArm()->TargetOffset.X = 0.0f;
		Player->GetMySpringArm()->TargetOffset.Y = 0.0f;
	}
}
