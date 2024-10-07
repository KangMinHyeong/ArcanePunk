
#include "UserInterface/Inform/Skill/APSkillWindow.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "UserInterface/Inform/Skill/APSkillWindowSlot.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/Inform/Skill/APSkillWindowInformation.h"

void UAPSkillWindow::NativeConstruct()
{
    Super::NativeConstruct();
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;

    SetIsFocusable(true);
    SetKeyboardFocus();
    BindButton();

    InitActiveSkill();
    InitPassiveSkill();

    UAPSoundSubsystem::PlayUIOpenSound(UAPGameInstance::GetSoundGI(GetWorld()));
}

FReply UAPSkillWindow::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) // 클릭 아이템 인벤토리로 (오른쪽 마우스)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    return Reply.Handled();
}

FReply UAPSkillWindow::NativeOnMouseWheel(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);

    return Reply.Handled();
}

FReply UAPSkillWindow::NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) // 전부 인벤토리로 (F키)
{
    FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    if(InKeyEvent.GetKey() == EKeys::K || InKeyEvent.GetKey() == EKeys::Escape)
    {
        RemoveFromParent();
        return Reply.Handled();
    }
    
	return Reply.Handled();
}

void UAPSkillWindow::BindButton()
{
    Button_Cancel->OnClicked.AddDynamic(this, &UAPSkillWindow::OnClick_Cancel);
    Button_Active->OnClicked.AddDynamic(this, &UAPSkillWindow::OnClick_Active);
    Button_Passive->OnClicked.AddDynamic(this, &UAPSkillWindow::OnClick_Passive);

    Button_Cancel->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Active->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
    Button_Passive->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
}

void UAPSkillWindow::OnClick_Cancel()
{
    UAPSoundSubsystem::PlayUICloseSound(UAPGameInstance::GetSoundGI(GetWorld()));
    RemoveFromParent();
}

void UAPSkillWindow::OnClick_Active()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    Switcher_Change->SetActiveWidget(ScrollBox_Active);
    Text_Active->SetColorAndOpacity(OnColor);
    Text_Passive->SetColorAndOpacity(OffColor);
    ScrollBox_Information->ClearChildren();

    auto ButtonStyle = Button_Active->GetStyle(); 
    ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Button_Active->SetStyle(ButtonStyle); 
    ButtonStyle.Normal.SetResourceObject(TabImage_OFF);
    Button_Passive->SetStyle(ButtonStyle); 
}

void UAPSkillWindow::OnClick_Passive()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));

    Switcher_Change->SetActiveWidget(ScrollBox_Passive);
    Text_Active->SetColorAndOpacity(OffColor);
    Text_Passive->SetColorAndOpacity(OnColor);
    ScrollBox_Information->ClearChildren();

    auto ButtonStyle = Button_Passive->GetStyle(); 
    ButtonStyle.Normal.SetResourceObject(TabImage_ON);
    Button_Passive->SetStyle(ButtonStyle); 
    ButtonStyle.Normal.SetResourceObject(TabImage_OFF);
    Button_Active->SetStyle(ButtonStyle); 
}

void UAPSkillWindow::InitActiveSkill()
{
    auto Skill_Q = CreateWidget<UAPSkillWindowSlot>(GetWorld(), SkillWindowSlotClass);
    auto Skill_E = CreateWidget<UAPSkillWindowSlot>(GetWorld(), SkillWindowSlotClass);
    auto Skill_R = CreateWidget<UAPSkillWindowSlot>(GetWorld(), SkillWindowSlotClass);

	ScrollBox_Active->AddChild(Skill_Q);
    ScrollBox_Active->AddChild(Skill_E);
    ScrollBox_Active->AddChild(Skill_R);

    Skill_Q->InitSkillCategory(this, EEnhanceCategory::Enhance_Q);
    Skill_E->InitSkillCategory(this, EEnhanceCategory::Enhance_E);
    Skill_R->InitSkillCategory(this, EEnhanceCategory::Enhance_R);
}

void UAPSkillWindow::InitPassiveSkill()
{
    for(auto PassiveSkills : OwnerCharacter->GetPassiveSkills())
    {
        auto Passive = CreateWidget<UAPSkillWindowSlot>(GetWorld(), SkillWindowSlotClass);
        ScrollBox_Passive->AddChild(Passive);
        Passive->InitPassiveCategory(this, PassiveSkills.Key);
    }
}

void UAPSkillWindow::OpenSkillInformation(EEnhanceCategory UpdateEnhanceCategory, FSkillNameList* SkillNameTable, float CoolTime)
{
    ScrollBox_Information->ClearChildren();

    auto Information = CreateWidget<UAPSkillWindowInformation>(GetWorld(), SkillWindowInformationClass);
    ScrollBox_Information->AddChild(Information);
    Information->InitInformationData_Skill(UpdateEnhanceCategory, SkillNameTable, CoolTime);
}

void UAPSkillWindow::OpenEnhanceInformation(EEnHanceType UpdateEnhanceType,  FSkillAbilityDataSheet* UpdateAbilityData, uint16 UpdateNestingNumber)
{
    ScrollBox_Information->ClearChildren();

    auto Information = CreateWidget<UAPSkillWindowInformation>(GetWorld(), SkillWindowInformationClass);
    ScrollBox_Information->AddChild(Information);
    Information->InitInformationData_Enhance(UpdateEnhanceType, UpdateAbilityData, UpdateNestingNumber);
}
