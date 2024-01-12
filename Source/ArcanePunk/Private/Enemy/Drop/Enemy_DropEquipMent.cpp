// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropEquipMent.h"

#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"

AEnemy_DropEquipMent::AEnemy_DropEquipMent()
{

}

void AEnemy_DropEquipMent::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UAPItemBase::StaticClass(), ItemQuantity);
}

void AEnemy_DropEquipMent::DropOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Character = Cast<AArcanePunkCharacter>(OtherActor);
	if(!Character) return;
    if(IsPendingKillPending() || !ItemReference) return;

    if (UAPInventoryComponent* PlayerInventory = Character->GetInventory())
	{
		const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);
	    switch(AddResult.OperationResult)
		{
		case EItemAddResult::IAR_NoItemAdded:
			UE_LOG(LogTemp, Warning, TEXT("No add item"));
			break;
		case EItemAddResult::IAR_PartialAmountAdded:
			Character->UpdateInteractionWidget();
			UE_LOG(LogTemp, Warning, TEXT("Yes Paritial"));
			break;
		case EItemAddResult::IAR_AllItemAdded:
			UE_LOG(LogTemp, Warning, TEXT("YES WE GET ALL ITEM"));
			Destroy();
			break;
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
	}
}