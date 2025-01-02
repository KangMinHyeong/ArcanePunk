
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APConversationText.generated.h"

class UImage;
class UTextBlock;
class UWrapBox;
struct FSequenceStringDataTable;
class UWidgetAnimation;

UCLASS()
class ARCANEPUNK_API UAPConversationText : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void SetConversation(FSequenceStringDataTable* ContentTextData);
	
	void SkipConversation();
	
private:
	void InitValue();
	void InitText();
	UWrapBox* CreateCustomWrap();
	UTextBlock* CreateCustomTextBlock(bool bEmphasize);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* ConversationPortrait;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* ConversationCompleteTriangle;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UWrapBox* TextWrapBox;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* ConversationActorName;

	FTimerHandle TextFlowTimerHandle;

	TArray<FString> ConversationLetters;

	UPROPERTY()
	TArray<UTextBlock*> TextLetters;

	uint8 LetterNum = 0;

	// Custom
	UPROPERTY(EditAnywhere)
	float TextFlowSpeed = 0.25f;

	UPROPERTY(EditAnywhere, Category=SlateStyleRules, meta=(AllowedClasses="/Script/Engine.Font", DisplayName="Base Font"))
	TObjectPtr<const UObject> CustomFont;

	UPROPERTY(EditAnywhere)
	FSlateColor CustomColor;

	UPROPERTY(EditAnywhere)
	uint8 BaseFontSize;

	FSlateColor EmphasizeColor;

	uint8 EmphasizeFontSize;

	TArray<uint16> EmphasizeLetterIndex;

	UPROPERTY(EditAnywhere)
	float RenderSpeed = 10.0f;

	bool bTick = true;

	float FlowTime = 0.0f;

public:
	bool bTextComplete = false;
};
