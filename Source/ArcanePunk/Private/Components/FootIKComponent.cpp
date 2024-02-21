#include "Components/FootIKComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

#define CheckFalse(p){ if(p == false) return ; }

UFootIKComponent::UFootIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFootIKComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UFootIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator leftRotation;
	Trace(LeftSocket, leftDistance, leftRotation);
	UE_LOG(LogTemp, Log, TEXT("Character left :: %f"), leftDistance);

	if (leftDistance != 0)
	{
		Rotate(LeftSocket, leftDistance, leftRotation);
	}

	FRotator rightRotation;
	Trace(RightSocket, rightDistance, rightRotation);

	if (rightDistance != 0)
	{
		Rotate(RightSocket, rightDistance, rightRotation);
	}

	float offset = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);
}

void UFootIKComponent::Trace(FName InSocket, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.0f;

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket);
	FVector start = FVector(location.X, location.Y, OwnerCharacter->GetActorLocation().Z);


	float traceZ = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector end = FVector(location.X, location.Y, traceZ);

	DrawDebugLine(GetWorld(), start, end, FColor::Green);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ignoreActors, DrawDebugType, hitResult, true, FLinearColor::Green, FLinearColor::Red);

	CheckFalse(hitResult.IsValidBlockingHit());

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = OffsetDistance + length - TraceDistance;
}

void UFootIKComponent::Rotate(FName InSocket, float& OutDistance, FRotator& OutRotation)
{
	FVector normal = hitResult.ImpactNormal;
	float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);

	OutRotation = FRotator(pitch, 0.0f, roll);
}

