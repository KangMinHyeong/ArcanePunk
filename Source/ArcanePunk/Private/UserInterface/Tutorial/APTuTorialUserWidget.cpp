// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Tutorial/APTuTorialUserWidget.h"
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

	if (Keys[NowIndex]->GetVisibility() == ESlateVisibility::Collapsed) {
		UE_LOG(LogTemp, Warning, TEXT("To Visible"));
		Keys[NowIndex]->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Keys[NowIndex]->GetVisibility() == ESlateVisibility::Visible) {
		UE_LOG(LogTemp, Warning, TEXT("To Collapsed"));
		Keys[NowIndex]->SetVisibility(ESlateVisibility::Collapsed);
	}
}



bool UAPTuTorialUserWidget::UpdateWidget(const FString PressedKey)
{

	// ���� �ʿ��� �Է��� Ű���� �̹��� Ȱ��ȭ
	//Keys[NowIndex]->SetVisibility(ESlateVisibility::Visible);


	// ���� ȭ��� �����ְ� �ִ� Ű �Է��� �������� ���� Ű�Է����� �Ѿ����
	// �Ѿ�� �� �ʿ��� �Է��� Ű���� �̹��� ��Ȱ��ȭ
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


