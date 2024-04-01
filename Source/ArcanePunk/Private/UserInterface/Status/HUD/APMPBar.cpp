// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Status/HUD/APMPBar.h"

#include "Components/WidgetSwitcher.h"

void UAPMPBar::NativeConstruct()
{
    Super::NativeConstruct();

    MPSwitcher->SetActiveWidget(MP_Origin);
}

void UAPMPBar::UseMP()
{
    MPSwitcher->SetActiveWidget(MP_Use);
    FadeOut();
}

void UAPMPBar::RecoveryMP()
{
    MPSwitcher->SetActiveWidget(MP_Origin);
    FadeIn();
}
