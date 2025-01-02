
#include "UserInterface/Conversation/APConversationText.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Interfaces/InteractionInterface.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

void UAPConversationText::NativeOnInitialized()
{
    Super::NativeOnInitialized();

}

void UAPConversationText::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(!bTick) return;
    if(bTextComplete) {bTick = false; return;}

    if(LetterNum > 0)
    {
        for(int32 i = 0; i<LetterNum; i++)
        {
            float RO = FMath::FInterpTo(TextLetters[i]->GetRenderOpacity(), 1.0f, InDeltaTime, RenderSpeed);
            TextLetters[i]->SetRenderOpacity(RO);
        }

        if(TextLetters.Num() == LetterNum && TextLetters[LetterNum-1]->GetRenderOpacity() >= 0.975f) 
        {
            bTextComplete = true; 
            bTick = false;
            ConversationCompleteTriangle->SetVisibility(ESlateVisibility::Visible);
        }
    }

    if(FlowTime >= TextFlowSpeed)
    {
        FlowTime = 0.0f;

        if(TextLetters.Num() == LetterNum) return;
        TextLetters[LetterNum]->SetVisibility(ESlateVisibility::Visible);
        LetterNum++;
    }
    else {FlowTime += InDeltaTime;}     
}

void UAPConversationText::SetConversation(FSequenceStringDataTable* ContentTextData)
{
    ConversationPortrait->SetBrushFromTexture(ContentTextData->Portrait);
    ConversationActorName->SetText(FText::FromString(ContentTextData->SequenceCharacterName));

    ConversationCompleteTriangle->SetVisibility(ESlateVisibility::Collapsed);
    // EmphasizeFontSize = ConversationData.EmphasizeFontSize;
    // EmphasizeColor = ConversationData.EmphasizeColor;
    // EmphasizeLetterIndex = ConversationData.EmphasizeLetterIndex;

    InitValue(); 
    ConversationLetters = UKismetStringLibrary::GetCharacterArrayFromString(ContentTextData->Content);
    if(ConversationLetters.IsEmpty()) {bTextComplete = true;}
    InitText();

    bTick = true;
}

void UAPConversationText::SkipConversation()
{
    for(auto Letter : TextLetters)
    {
        Letter->SetVisibility(ESlateVisibility::Visible);
        Letter->SetRenderOpacity(1.0f);
    }
    ConversationCompleteTriangle->SetVisibility(ESlateVisibility::Visible);
    bTextComplete = true;
}

void UAPConversationText::InitValue()
{
    ConversationLetters.Empty(); 
    bTextComplete = false; 
    LetterNum = 0; 
    TextWrapBox->ClearChildren(); 
    TextLetters.Empty();
}

void UAPConversationText::InitText()
{
    uint8 TextNum = 0;
    
    UWrapBox* Wrap = CreateCustomWrap();
    for(auto Letter : ConversationLetters)
    {
        if(Letter == "/")
        {
            TextWrapBox->AddChildToWrapBox(Wrap);
            Wrap = CreateCustomWrap();
            TextNum++;
            continue;
        }

        auto Text = CreateCustomTextBlock(EmphasizeLetterIndex.Contains(TextNum) ? true : false);
        Text->SetText(FText::FromString(Letter));
        auto WrapSlot = Wrap->AddChildToWrapBox(Text); WrapSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

        TextLetters.Add(Text);

        TextNum++;
        if(TextNum == ConversationLetters.Num())
        {
            TextWrapBox->AddChildToWrapBox(Wrap);
            Wrap = CreateCustomWrap();
        }
    }
}

UWrapBox* UAPConversationText::CreateCustomWrap()
{
    auto Wrap = NewObject<UWrapBox>(this, UWrapBox::StaticClass());

    Wrap->SetExplicitWrapSize(true); 
    Wrap->SetWrapSize(TextWrapBox->GetWrapSize()); 
    Wrap->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left); 
    Wrap->SetInnerSlotPadding(TextWrapBox->GetInnerSlotPadding());
    Wrap->SetVisibility(ESlateVisibility::Visible);

    return Wrap;
}

UTextBlock* UAPConversationText::CreateCustomTextBlock(bool bEmphasize)
{
    auto Text = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

    FSlateFontInfo Font = Text->GetFont(); 
    if(bEmphasize)
    {
        Font.Size = EmphasizeFontSize;
        Text->SetColorAndOpacity(EmphasizeColor);
    }
    else
    {
        Font.Size = BaseFontSize;
        Text->SetColorAndOpacity(CustomColor);
    }
    Font.FontObject = CustomFont;
    Text->SetFont(Font);    
    Text->SetVisibility(ESlateVisibility::Hidden);
    Text->SetRenderOpacity(0.0f);

    return Text;
}