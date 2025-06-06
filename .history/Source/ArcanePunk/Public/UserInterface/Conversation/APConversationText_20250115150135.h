
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APConversationText.generated.h"

class UImage;
class UTextBlock;
class UWrapBox;
struct FDialogueDataTable;
struct FCharaterUIData;
class UWidgetAnimation;

UCLASS()
class ARCANEPUNK_API UAPConversationText : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	FORCEINLINE void SetTickEnable(bool NewBool) {bTick = NewBool;};

	void SetConversation(const FDialogueDataTable & Dialogues, const FCharaterUIData & CharacterUIData);
	
	void SkipConversation();
	
private:
	void InitValue();
	void InitText();
	UWrapBox* CreateCustomWrap();
	UTextBlock* CreateCustomTextBlock();
	void CheckFullSkip(float InDeltaTime);
	void CancelFullSkip();

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* ConversationPortrait;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* ConversationCompleteTriangle;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UWrapBox* TextWrapBox;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* ConversationActorName;

	TArray<FString> ConversationLetters;
	UPROPERTY()
	TArray<UTextBlock*> TextLetters;

	uint8 LetterNum = 0;
	
	UPROPERTY(EditAnywhere, Category=SlateStyleRules, meta=(AllowedClasses="/Script/Engine.Font", DisplayName="Base Font"))
	TObjectPtr<const UObject> CustomFont;

	// Custom
	UPROPERTY(EditAnywhere)
	FSlateColor CustomColor;
	UPROPERTY(EditAnywhere)
	uint8 BaseFontSize;
	UPROPERTY(EditAnywhere)
	FLinearColor OutlineColor;
	
	UPROPERTY(EditAnywhere)
	int32 OutlineSize;

	UPROPERTY(EditAnywhere)
	float TextFlowSpeed = 0.25f;
	UPROPERTY(EditAnywhere)
	float RenderSpeed = 10.0f;
	float FlowTime = 0.0f;
	
	bool bTick = false;
	bool bEmphasize;
		

public:
	bool bTextComplete = false;
	bool bSkipPress = false;

	float SkipPressTime = 0.0f;
};
