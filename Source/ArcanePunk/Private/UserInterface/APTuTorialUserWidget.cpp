// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/APTuTorialUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"


void UAPTuTorialUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	

	// MoveForward
	NeedInput.Emplace("PressUp");
	Keys.Emplace(W);
	NeedInput.Emplace("PressDown");
	Keys.Emplace(S);

	// MoveRight
	NeedInput.Emplace("PressRight");
	Keys.Emplace(D);
	NeedInput.Emplace("PressLeft");
	Keys.Emplace(A);

	// Jogging
	NeedInput.Emplace("PressShift + PressMove");
	Keys.Emplace(Shift);

	// ZoomInOut
	NeedInput.Emplace("WheelUp");
	Keys.Emplace(Shift);
	NeedInput.Emplace("WheelDown");
	Keys.Emplace(Shift);

	// Attack
	NeedInput.Emplace("ClickLeft");
	Keys.Emplace(Shift);
	NeedInput.Emplace("ClickRight");
	Keys.Emplace(Shift);

	// Skill
	NeedInput.Emplace("PressQ");
	Keys.Emplace(Q);
	NeedInput.Emplace("PressE");
	Keys.Emplace(E);
	NeedInput.Emplace("PressR");
	Keys.Emplace(R);

	// Experience
	//NeedInput.Emplace("PressU");
	//NeedInput.Emplace("PressV");
	NeedInput.Emplace("PressF");
	Keys.Emplace(F);
	NeedInput.Emplace("PressTab");
	Keys.Emplace(Tab);
	//NeedInput.Emplace("PressCtrl + 9");

	NowIndex = 0;
	Time = 0;
}

void UAPTuTorialUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("PressUp"));

}

void UAPTuTorialUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	// 1frmae -> deltatime -> 2frame

	Time += DeltaTime;

	if (Time >= 0.5f) {
		WidgetBlink();
		Time = 0;
	}
}

void UAPTuTorialUserWidget::WidgetBlink()
{

	if (Keys[NowIndex]->Visibility == ESlateVisibility::Collapsed) {
		UE_LOG(LogTemp, Warning, TEXT("To Visible"));
		Keys[NowIndex]->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Keys[NowIndex]->Visibility == ESlateVisibility::Visible) {
		UE_LOG(LogTemp, Warning, TEXT("To Collapsed"));
		Keys[NowIndex]->SetVisibility(ESlateVisibility::Collapsed);
	}
}



bool UAPTuTorialUserWidget::UpdateWidget(const FString PressedKey)
{

	// 현재 필요한 입력의 키보드 이미지 활성화
	//Keys[NowIndex]->SetVisibility(ESlateVisibility::Visible);


	// 현재 화면상 보여주고 있는 키 입력이 들어왔을때 다음 키입력으로 넘어가야함
	// 넘어가기 전 필요한 입력의 키보드 이미지 비활성화
	if (NeedInput[NowIndex] == PressedKey) {
		Keys[NowIndex]->SetVisibility(ESlateVisibility::Collapsed);
		NowIndex++;
	}

	if (NowIndex < NeedInput.Num())
	{
		KeyPressText->SetText(FText::FromString(NeedInput[NowIndex]));
		//return false;
		return true;
	}
	return true;
}


