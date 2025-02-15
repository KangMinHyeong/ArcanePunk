
#include "UserInterface/Conversation/APConversationText.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Interfaces/InteractionInterface.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "PlayerState/APPlayerData.h"
#include "UserInterface/Conversation/APConversationUI.h"

void UAPConversationText::NativeOnInitialized()
{
    Super::NativeOnInitialized();

}

void UAPConversationText::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(!bTick) return;
    if(bSkipPress) CheckFullSkip(InDeltaTime);
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

void UAPConversationText::SetConversation(UAPConversationUI* ParentWidget, const FDialogueDataTable & Dialogues, const FCharaterUIData &  CharacterUIData)
{
    ParentUI = ParentWidget;

    if(CharacterUIData.Portraits.Num() <= Dialogues.Portrait_Type)
    {
        ConversationPortrait->SetBrushFromTexture(CharacterUIData.Portraits[Dialogues.Portrait_Type - 1]);
        ConversationActorName->SetText(FText::FromString(CharacterUIData.CharaterName));
    }
    
    bEmphasize = Dialogues.bEmphasize;

    ConversationCompleteTriangle->SetVisibility(ESlateVisibility::Collapsed);
    InitValue(); 
    ConversationLetters = UKismetStringLibrary::GetCharacterArrayFromString(Dialogues.Diologue_Text);
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

        auto Text = CreateCustomTextBlock();
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

UTextBlock* UAPConversationText::CreateCustomTextBlock()
{
    auto Text = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

    FSlateFontInfo Font = Text->GetFont(); 
    if(bEmphasize)
    {
        Font.OutlineSettings.OutlineColor = OutlineColor;
        Font.OutlineSettings.OutlineSize = OutlineSize;
    }
    
    Font.Size = BaseFontSize;
    Text->SetColorAndOpacity(CustomColor);
    Font.FontObject = CustomFont;
    Text->SetFont(Font);    
    Text->SetVisibility(ESlateVisibility::Hidden);
    Text->SetRenderOpacity(0.0f);

    return Text;
}

void UAPConversationText::CheckFullSkip(float InDeltaTime)
{
    SkipPressTime += InDeltaTime;

    if(SkipPressTime >= 0.2f && SkipPressTime < 3.0f)
    {
        Skip_ProgressBar->SetVisibility(ESlateVisibility::Visible);
        Skip_ProgressBar->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), (SkipPressTime - 0.2f) / 2.79f);
    }
    if(SkipPressTime >= 3.0f)
    {
        InitFullSkip();
        if(ParentUI.IsValid()) ParentUI->StartFullSkip();
    }
}

void UAPConversationText::InitFullSkip()
{
    bSkipPress = false;
    SkipPressTime = 0.0f;
    Skip_ProgressBar->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), 0.0f);
    Skip_ProgressBar->SetVisibility(ESlateVisibility::Hidden);
}