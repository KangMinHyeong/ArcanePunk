
#include "UserInterface/Shop/APShopListTitle.h"

#include "GameInstance/APGameInstance.h"
#include "Components/TextBlock.h"

void UAPShopListTitle::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_GoodsName, EStringRowName::ItemList);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_GoodsNum, EStringRowName::Quantity);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_GoodsPrice, EStringRowName::Price);

}