
#include "Skill/Terminator.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ATerminator::ATerminator()
{
}

void ATerminator::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void ATerminator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bBuffOn)
    {
        ATKSpeedBuff(DeltaTime);
        ATKBuff(DeltaTime);
        FastBuff(DeltaTime);
    }

    if(bComplete) DestroySKill();
}

void ATerminator::ATKSpeedBuff(float DeltaTime)
{
    if(!OwnerCharacter.IsValid()) return;
    CurrentATKSpeedCoefficient = FMath::FInterpConstantTo(CurrentATKSpeedCoefficient, 1.0f, DeltaTime, BuffEndSpeed);
    auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 
	PD.StatusData.ATKSpeed = PD.StatusData.ATKSpeed * OwnerCharacter->GetBuffComp()->GetCurrentATKSpeedCoefficient() * CurrentATKSpeedCoefficient;
    OwnerCharacter->SetDefaultATKSpeed(PD.StatusData.ATKSpeed);

    if(abs(CurrentATKSpeedCoefficient - 1.0f) <= KINDA_SMALL_NUMBER) {bComplete = true;}
    else {bComplete = false;}
}

void ATerminator::ATKBuff(float DeltaTime)
{
    if(!OwnerCharacter.IsValid()) return;
    CurrentATKCoefficient = FMath::FInterpConstantTo(CurrentATKCoefficient, 1.0f, DeltaTime, BuffEndSpeed);
    auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 
	PD.StatusData.ATK = PD.StatusData.ATK * OwnerCharacter->GetBuffComp()->GetCurrentATKCoefficient() * CurrentATKCoefficient;
    OwnerCharacter->SetDefaultATK(PD.StatusData.ATK);

    if(abs(CurrentATKCoefficient - 1.0f) <= KINDA_SMALL_NUMBER)  {bComplete = true;}
    else {bComplete = false;}
}

void ATerminator::FastBuff(float DeltaTime)
{
    if(!OwnerCharacter.IsValid()) return;
    CurrentFastCoefficient = FMath::FInterpConstantTo(CurrentFastCoefficient, 1.0f, DeltaTime, BuffEndSpeed);
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->GetDefaultSpeed_Origin() * OwnerCharacter->GetCrowdControlComp()->GetCurrentSlowCoefficient() * OwnerCharacter->GetCrowdControlComp()->GetCurrentFastCoefficient() * CurrentFastCoefficient;
    OwnerCharacter->SetDefaultSpeed( OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed );

    if(abs(CurrentFastCoefficient - 1.0f) <= KINDA_SMALL_NUMBER) {bComplete = true;}
    else {bComplete = false;}
}

void ATerminator::DestroySKill()
{
    DeActivate_Ult();
    Super::DestroySKill();
}

void ATerminator::SetTerminatorMode()
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TerminatorStartEffect, GetActorLocation(), GetActorRotation());

    TerminatorEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(TerminatorEffect, OwnerCharacter->GetMesh(), TEXT("TerminatorEffect"), OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), FVector(1,1,1), EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::None, true);
    TerminatorEffectComp->SetVariableFloat(TEXT("Time"), DestroyTime);

    CurrentFastCoefficient = FastCoefficient;
    CurrentATKCoefficient = ATKCoefficient;
    CurrentATKSpeedCoefficient = ATKSpeedCoefficient;

    BuffEndSpeed = (CurrentFastCoefficient - 1.0f) / DestroyTime;

    bBuffOn = true;
    SetActorTickEnabled(true);
}

void ATerminator::ResettTerminatorMode()
{
    auto PD = OwnerCharacter->GetPlayerStatus_Origin(); 
	PD.StatusData.ATKSpeed = PD.StatusData.ATKSpeed * OwnerCharacter->GetBuffComp()->GetCurrentATKSpeedCoefficient();
    OwnerCharacter->SetDefaultATKSpeed(PD.StatusData.ATKSpeed);

    PD.StatusData.ATK = PD.StatusData.ATK * OwnerCharacter->GetBuffComp()->GetCurrentATKCoefficient();
    OwnerCharacter->SetDefaultATK(PD.StatusData.ATK);

    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->GetDefaultSpeed_Origin() * OwnerCharacter->GetCrowdControlComp()->GetCurrentSlowCoefficient() * OwnerCharacter->GetCrowdControlComp()->GetCurrentFastCoefficient();
    OwnerCharacter->SetDefaultSpeed( OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed );

    Destroy();
}

void ATerminator::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;

    SetTerminatorMode();
}