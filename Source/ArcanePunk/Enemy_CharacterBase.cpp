// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_CharacterBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemy_CharacterBase::AEnemy_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy_CharacterBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageApplied = FMath::Min(HP, DamageApplied);

	
	HP = HP - DamageMath(DamageApplied);
	UE_LOG(LogTemp, Display, TEXT("Monster HP : %f"), HP);
	//GetWorldTimerManager().SetTimer(HitTimerHandle, this, &ABossMonster_Stage1::CanBeDamagedInit, bGodModeTime, false);

	if(IsDead())
	{
	// 	UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh(), TEXT("DeadSound"));
	// 	bDead = true;
	// 	ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	// 	if(GameMode != nullptr)
	// 	{
	// 		GameMode->PawnKilled(this);
	// 	}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABossMonster_Stage1::Destoryed, DeathLoadingTime, false);

	// 	ASimpleShooterPlayerController* PlayerController = Cast<ASimpleShooterPlayerController>(UGameplayStatics::GetPlayerPawn(this, 0)->GetController());
	// 	if(PlayerController != nullptr)
	// 	{
	// 		PlayerController->RemoveBossHPUI();
	// 		PlayerController->ClearGameEnd();
	// 	}

	}

    return DamageApplied;
}

bool AEnemy_CharacterBase::IsDead()
{
    return HP<=0;
}

float AEnemy_CharacterBase::DamageMath(float Damage)
{
    return Damage * Defense_constant * (1/(Defense_constant + Character_Defense));
}
