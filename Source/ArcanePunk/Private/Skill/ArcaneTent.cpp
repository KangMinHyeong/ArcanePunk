
#include "Skill/ArcaneTent.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/DecalComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/Character/APSkillHubComponent.h"

AArcaneTent::AArcaneTent()
{
    TentTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("TentTrigger"));
    TentDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("TentDecal"));

    SetRootComponent(TentTrigger);
    TentDecal->SetupAttachment(TentTrigger);

    SkillCategory = ESkillCategory::Spawn_Static;
}

void AArcaneTent::BeginPlay()
{
    Super::BeginPlay();  

}

void AArcaneTent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AArcaneTent::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); OwnerCharacter->SetReturnToHideTime(ReturnToHideTime); OwnerCharacter->SetInArcaneTent(true);
    TentTrigger->OnComponentBeginOverlap.AddDynamic(this, &AArcaneTent::OnBeginHiding);
    TentTrigger->OnComponentEndOverlap.AddDynamic(this, &AArcaneTent::OnEndHiding);
}

void AArcaneTent::DestroySKill()
{
    Super::DestroySKill();

    if(OwnerCharacter) OwnerCharacter->SetInArcaneTent(false);
    if(OwnerCharacter) OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber13()->bActivate = false;
}

void AArcaneTent::OnBeginHiding(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if(OtherActor != GetOwner()) return; if(!OwnerCharacter) return;

    OwnerCharacter->SetInArcaneTent(true);
}

void AArcaneTent::OnEndHiding(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    if(OtherActor != GetOwner()) return; if(!OwnerCharacter) return;

    OwnerCharacter->SetInArcaneTent(false);
}
