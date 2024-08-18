
#include "UserInterface/Shop/APShopListTitle.h"

#include "GameInstance/APGameInstance.h"
#include "Components/TextBlock.h"

void UAPShopListTitle::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;  

    APGI->SetTextBlock(Text_GoodsName, EStringRowName::ItemList);
    APGI->SetTextBlock(Text_GoodsNum, EStringRowName::Quantity);
    APGI->SetTextBlock(Text_GoodsPrice, EStringRowName::Price);

}