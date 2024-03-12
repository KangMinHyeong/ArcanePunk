// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropDice.h"

#include "Character/ArcanePunkCharacter.h"

void AEnemy_DropDice::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UAPItemBase::StaticClass(), ItemQuantity);
}

void AEnemy_DropDice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnDropDice();
}

void AEnemy_DropDice::OnDropDice()
{
    TWeakObjectPtr<AArcanePunkCharacter> Character = InteractTrigger->Character; if(!Character.IsValid()) return;

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
			// Character->UpdateInteractionWidget();
			UE_LOG(LogTemp, Warning, TEXT("Yes Paritial"));
			break;
		case EItemAddResult::IAR_AllItemAdded:
			UE_LOG(LogTemp, Warning, TEXT("YES WE GET ALL ITEM"));
			Destroy();
			break;
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
	}
    // auto PD = Character->GetPlayerStatus();
    // PD.PlayerGoodsData.RerollDice = PD.PlayerGoodsData.RerollDice + ItemQuantity;
    // Character->SetPlayerStatus(PD);

    Destroy();
}
