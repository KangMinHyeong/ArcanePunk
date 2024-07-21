
#include "UserInterface/Cursor/APBattleCursor.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "Components/CanvasPanel.h"

void UAPBattleCursor::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    CheckCursorUnder();
}

void UAPBattleCursor::CheckCursorUnder()
{
    auto PC = Cast<AArcanePunkPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    auto Character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(!PC || !Character) return;

    FHitResult HitResult;
    float X = 0.0f; float Y = 0.0f; float Angle = 0.0f;
    if(PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
    {
        // X축 기준
        X = HitResult.Location.X - Character->GetActorLocation().X; Y = HitResult.Location.Y - Character->GetActorLocation().Y;
        
        // Y축 기준
        // Y = HitResult.Location.X - Character->GetActorLocation().X; X = HitResult.Location.Y - Character->GetActorLocation().Y;
    }
    else
    {
        PC->GetMousePosition(X,Y);
        FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
        X = X - ViewportSize.X/2; Y = ViewportSize.Y/2 - Y;
    }

    // Y축 기준
    // if(X >= 0.0f &&  Y < 0.0f){ Angle = 90.0f - FMath::RadiansToDegrees(UKismetMathLibrary::Atan(Y/X));}
    // else if(X < 0.0f &&  Y < 0.0f){Angle = -90.0f - FMath::RadiansToDegrees(UKismetMathLibrary::Atan(Y/X));}
    // else{Angle = FMath::RadiansToDegrees(UKismetMathLibrary::Atan(X/Y));}

    // X축 기준
    if(X >= 0.0f &&  Y < 0.0f){ Angle = 90.0f + FMath::RadiansToDegrees(UKismetMathLibrary::Atan(Y/X));}
    else if(X < 0.0f &&  Y < 0.0f){Angle = - 90.0f + FMath::RadiansToDegrees(UKismetMathLibrary::Atan(Y/X));}
    else{Angle = 180.0f - FMath::RadiansToDegrees(UKismetMathLibrary::Atan(X/Y));}
    

    MainPanel->SetRenderTransformAngle(Angle);
    PC->MousePositionAngle = Angle;
}