#include "UserInterface/HUD/APHUD.h"
#include "Logging/StructuredLog.h"

#include "UserInterface/Inventory/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"
#include "UserInterface/Tutorial/APTuTorialUserWidget.h"
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"
#include "UserInterface/Inform/Stage/APStageInformationUI.h"
#include "UserInterface/EnhanceUI/EnhanceGauge.h"
#include "UserInterface/HUD/Status/Bar/ImitatorSkillSlot.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/HUD/Status/Bar/APSwapBarUI.h"
#include "UserInterface/Common/DisplayOnly/APSystemMessage.h"

#include "Kismet/GameplayStatics.h"
#include "GameInstance/Subsystem/APSystemMessageSubsystem.h"


DEFINE_LOG_CATEGORY(LogHUD)

AAPHUD::AAPHUD()
{}

void AAPHUD::BeginPlay()
{
	Super::BeginPlay();

	//@메인 메뉴
	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(-4);
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}

	//@시스템 메시지 위젯 생성
	if (!SystemMessageClass)
	{
		UE_LOGFMT(LogHUD, Warning, "시스템 메시지 위젯 클래스가 설정되지 않음");
		return;
	}

	SystemMessageWidget = CreateWidget<UAPSystemMessage>(GetWorld(), SystemMessageClass);
	if (!SystemMessageWidget.Get())
	{
		UE_LOGFMT(LogHUD, Error, "시스템 메시지 위젯 생성 실패: SystemMessageClass가 유효하지만 CreateWidget 실패");
		return;
	}

	SystemMessageWidget->AddToViewport(500);
	SystemMessageWidget->SetVisibility(ESlateVisibility::Collapsed);
	UE_LOGFMT(LogHUD, Log, "시스템 메시지 위젯 생성 완료");

	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController)
	{
		int32 ViewportSizeX;
		int32 ViewportSizeY;
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		ViewportSize.X = static_cast<double>(ViewportSizeX);
		ViewportSize.Y = static_cast<double>(ViewportSizeY);
	}
}

void AAPHUD::Init(TArray<uint8> UI_ID)
{
	PC = Cast<AArcanePunkPlayerController>(GetOwningPlayerController());
	if(!StatusBarUIClass || !PC) return;

	for(int32 i = 0; i<PC->GetPlayerNumbers(); i++)
	{
		auto StatusBarWidget = CreateWidget<UAPStatusBar>(GetWorld(), StatusBarUIClass);
		StatusBarWidgets.Emplace(StatusBarWidget);
	}

	SwapBar = CreateWidget<UAPSwapBarUI>(GetWorld(), SwapBarClass);
	SwapBar->AddToViewport(-10);
	SwapBar->Init(UI_ID);
}

void AAPHUD::BindStatusBar(AArcanePunkCharacter* OwnerPlayer, uint8 Index)
{
	StatusBarWidgets[Index]->BindStatusBar(OwnerPlayer);
}

void AAPHUD::ActivateStatusBar(uint8 Index)
{
	StatusBarWidgets[Index]->AddToViewport(-10);
	SwapBar->Active(Index);
}

void AAPHUD::DeactivateStatusBar(uint8 Index, float Gauge)
{
	StatusBarWidgets[Index]->RemoveFromParent();
	SwapBar->Deactive(Index, Gauge);
}

void AAPHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAPHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	}
}

void AAPHUD::ToggleMenu()
{
	auto Character = Cast<AArcanePunkCharacter>(GetOwningPawn());
	if(!Character) return;

	if (bIsMenuVisible)
	{
		HideMenu();
	}
	else
	{
		DisplayMenu();
	}
}


// Minhyeong
void AAPHUD::SetBossHPUI()
{
	if(!BossHPUIClass) return;
	BossHPWidget = CreateWidget<UUserWidget>(GetWorld(), BossHPUIClass);
	BossHPWidget->AddToViewport(-1);
}

void AAPHUD::DisplayEnhanceChoice(EEnhanceCategory EnhanceCategory, EEnHanceType UpdateEnHanceType, bool bOnlyEnhance, uint8 EnhanceSkillNum)
{
	if(!EnhanceChoiceClass) return;

	auto EnhanceUI = CreateWidget<UAPEnhanceChoice>(GetWorld(), EnhanceChoiceClass); if(!EnhanceUI) return;
	if(bOnlyEnhance) EnhanceUI->SetSkillAppearPercent(0.0f);
	EnhanceUI->InitType(EnhanceCategory, UpdateEnHanceType, EnhanceSkillNum);

	// ESkillTypeState, EnHanceType 의 정보를 EnhanceUI에게 넘겨주기
	EnhanceUI->AddToViewport(20);
}

void AAPHUD::DisplayEnhanceGauge(int32 TargetNum, int32 MaxNum)
{
	auto EnhanceGaugeUI = CreateWidget<UEnhanceGauge>(GetWorld(), EnhanceGaugeUIClass); if(!EnhanceGaugeUI) return;
	EnhanceGaugeUI->DisplayEnhanceGauge(TargetNum, MaxNum);
	EnhanceGaugeUI->AddToViewport(20);
}

void AAPHUD::OpenWorldMap()
{
	if(StageInformationUI.IsValid())
	{
		StageInformationUI->RemoveFromParent();
		StageInformationUI = nullptr;
	}
	else
	{
		StageInformationUI = CreateWidget<UAPStageInformationUI>(GetWorld(), StageInformationUIClass);
		StageInformationUI->AddToViewport();
	}
}

void AAPHUD::SetStatusVisibility(bool bHide)
{
	auto StatusBarWidget = StatusBarWidgets[PC->MainPlayerIndex];

	if(bHide)
	{
		StatusBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		StatusBarWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

UAPStatusBar *AAPHUD::GetStatusWidget(AArcanePunkCharacter* Player)
{
	return StatusBarWidgets[Player->PlayerIndex];
}

bool AAPHUD::CheckSwapCoolDown(uint8 Index)
{
    return SwapBar->CheckSwapCoolDown(Index);
}

float AAPHUD::UpdateSwapGauge(uint8 Index, float Add)
{
	return StatusBarWidgets[Index]->UpdateSwapGauge(Add);
}

void AAPHUD::ClearSwapGauge(uint8 PlayerIndex)
{
	StatusBarWidgets[PlayerIndex]->ClearSwapGauge();
}
