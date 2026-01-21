
#include "GameInstance/Subsystem/APUserWidgetSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "UserInterface/Common/DisplayOnly/APLoadingUI.h"
#include "UserInterface/Fade/LoadingFade.h"

UAPUserWidgetSubsystem::UAPUserWidgetSubsystem()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> LoadingUIClassFinder(TEXT("/Game/Blueprints/UserInterfaces/UI/Common/System_Message/WBP_Loading"));
	if(LoadingUIClassFinder.Succeeded()) LoadingUIClass = LoadingUIClassFinder.Class;

    static ConstructorHelpers::FClassFinder<UUserWidget> FadeUIClassFinder(TEXT("/Game/Blueprints/UserInterfaces/UI/Common/Fade/Loading/WBP_LoadFade"));
	if(FadeUIClassFinder.Succeeded()) FadeUIClass = FadeUIClassFinder.Class;
    
}

UAPUserWidgetSubsystem* UAPUserWidgetSubsystem::GetSubsystemSafe(UObject* WorldContextObject)
{
    if (!WorldContextObject) return nullptr;

    UWorld* World = WorldContextObject->GetWorld();
    if (!World) return nullptr;

    UGameInstance* GI = World->GetGameInstance();
    if (!GI) return nullptr;

    return GI->GetSubsystem<UAPUserWidgetSubsystem>();
}

void UAPUserWidgetSubsystem::CreateLoadingUI(UObject* WorldContextObject)
{
    if (UAPUserWidgetSubsystem* Subsystem = GetSubsystemSafe(WorldContextObject))
    {
        Subsystem->CreateLoadingUI_Internal();
    }
}

void UAPUserWidgetSubsystem::CreateLoadingUI_Internal()
{
    if (LoadingUI.IsValid()) return;

    LoadingUI = CreateWidget<UAPLoadingUI>(GetWorld(), LoadingUIClass);

    if (LoadingUI.IsValid())
    {
        LoadingUI->AddToViewport();
    }
}

void UAPUserWidgetSubsystem::RemoveLoadingUI(UObject* WorldContextObject)
{
    if (UAPUserWidgetSubsystem* Subsystem = GetSubsystemSafe(WorldContextObject))
    {
        if (UWorld* World = WorldContextObject->GetWorld())
        {
            World->GetTimerManager().SetTimer(
                Subsystem->LoadingTimerHandle,
                Subsystem,
                &UAPUserWidgetSubsystem::RemoveLoadingUI_Internal,
                1.5f,
                false
            );
        }
    }
}

void UAPUserWidgetSubsystem::RemoveLoadingUI_Internal()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(LoadingTimerHandle);
    }

    if(LoadingUI.IsValid()) 
    {
        LoadingUI->RemoveFromParent();
        LoadingUI = nullptr;

        UAPUserWidgetSubsystem::CreateFadeUI(this, false);
    }
}

void UAPUserWidgetSubsystem::UpdateLoadingPercent(UObject *WorldContextObject, float Percent)
{
    if (UAPUserWidgetSubsystem* Subsystem = GetSubsystemSafe(WorldContextObject))
    {
        Subsystem->UpdateLoadingPercent_Internal(Percent);
    }
}

void UAPUserWidgetSubsystem::UpdateLoadingPercent_Internal(float Percent)
{
    if(LoadingUI.IsValid()) 
    {
        LoadingUI->UpdateLoadingPercent(Percent);
    }
}


void UAPUserWidgetSubsystem::CreateFadeUI(UObject *WorldContextObject, bool bOutIn)
{
    if (UAPUserWidgetSubsystem* Subsystem = GetSubsystemSafe(WorldContextObject))
    {
        Subsystem->CreateFadeUI_Internal(bOutIn);
    }
}

void UAPUserWidgetSubsystem::CreateFadeUI_Internal(bool bOutIn)
{
    if(FadeUI.IsValid())
    {
        FadeUI->RemoveFromParent();
        FadeUI = nullptr;
    }

    FadeUI = CreateWidget<ULoadingFade>(GetWorld(), FadeUIClass);
    if(FadeUI.IsValid())
    {
        FadeUI->AddToViewport();

        if(bOutIn)
        {
            FadeUI->FadeOut();
            OnEndedFadeOut.Broadcast();
        }   
        else
        {
            FadeUI->FadeIn();
            OnStartFadeIn.Broadcast();
        }
    }   

    // if(MyCharacter.IsValid()) MyCharacter->EnableInput(this); 
    
    // if(bEntrance)
    // {
    //     auto TutorialGM = Cast<AAPGameModeTutorialStage>(UGameplayStatics::GetGameMode(GetWorld()));
    //     if(TutorialGM) TutorialGM->StartTutorial(this);
                
    //     // CreateEntranceUI();
    // }
}

