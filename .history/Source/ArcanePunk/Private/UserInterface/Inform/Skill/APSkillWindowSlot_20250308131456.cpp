
#include "UserInterface/Inform/Skill/APSkillWindowSlot.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/StackBox.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/Inform/Skill/APSkillWindow.h"
#include "UserInterface/Inform/Skill/APEnhanceSlot.h"
#include "Components/Character/APSkillHubComponent.h"

void UAPSkillWindowSlot::NativeConstruct()
{
    Super::NativeConstruct();
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
    DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI.IsValid()) return; 
    
    Button_Skill->OnClicked.AddDynamic(this, &UAPSkillWindowSlot::OnClick_SkillInformation);
    Button_Skill->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
}

void UAPSkillWindowSlot::InitSkillCategory(UUserWidget* Parent, EEnhanceCategory UpdateEnhanceCategory)
{
    ParentWidget = Parent;
    EnhanceCategory = UpdateEnhanceCategory;
    FString SkillCategory = TEXT("");
    UEnum* SkillNum = nullptr;
    switch (EnhanceCategory)
    {
    case EEnhanceCategory::Enhance_Q:
        SkillCategory = TEXT("Q 스킬");
        SkillNumber = (uint8)OwnerCharacter->GetQSkill();
        // EnhanceData = OwnerCharacter->GetQSkillNumber()->GetSkillAbilityNestingData();
        // CoolTime = OwnerCharacter->GetQSkillNumber()->GetCoolTime();
        SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); 
        break;
    
    case EEnhanceCategory::Enhance_E:
        SkillCategory = TEXT("E 스킬");
        SkillNumber = (uint8)OwnerCharacter->GetESkill();
        // EnhanceData = OwnerCharacter->GetESkillNumber()->GetSkillAbilityNestingData();
        // CoolTime = OwnerCharacter->GetESkillNumber()->GetCoolTime();
        SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); 
        break;

    case EEnhanceCategory::Enhance_R:
        SkillCategory = TEXT("R 스킬");
        SkillNumber = (uint8)OwnerCharacter->GetRSkill();
        EnhanceData = OwnerCharacter->GetRSkillNumber()->GetSkillAbilityNestingData();
        CoolTime = OwnerCharacter->GetRSkillNumber()->GetCoolTime();
        SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EUltSkillNumber"), true); 
        break;
    }
    Text_SkillName->SetText(FText::FromString(SkillCategory));
    if(SkillNum) SkillRowName = SkillNum->GetNameStringByValue(SkillNumber);

    InitSkillImage();
    InitEnhance();
}

void UAPSkillWindowSlot::InitPassiveCategory(UUserWidget* Parent, uint8 UpdatePassiveNumber)
{
    ParentWidget = Parent;
    EnhanceCategory = EEnhanceCategory::Enhance_Passive;
    SkillNumber = UpdatePassiveNumber;
    if(OwnerCharacter->GetPassiveSkills().Contains(SkillNumber)) EnhanceData = OwnerCharacter->GetPassiveSkills()[SkillNumber];

    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EPassiveNumber"), true); if(!SkillNum) return;
    SkillRowName = SkillNum->GetNameStringByValue(SkillNumber);
    SkillNameTable = DataTableGI->GetSkillNameListDataTable()->FindRow<FSkillNameList>(FName(*SkillRowName), SkillRowName); if(!SkillNameTable) return;

    Text_SkillName->SetText(FText::FromString(SkillNameTable->SkillName_Korean));

    InitSkillImage();
    InitEnhance();
}

void UAPSkillWindowSlot::InitSkillImage()
{   
    SkillNameTable = DataTableGI->GetSkillNameListDataTable()->FindRow<FSkillNameList>(FName(*SkillRowName), SkillRowName); if(!SkillNameTable) return;
    auto SkillImage = SkillNameTable->SkillSlotImage;

    auto ButtonStyle = Button_Skill->GetStyle(); 
    ButtonStyle.Normal.SetResourceObject(SkillImage);
    ButtonStyle.Hovered.SetResourceObject(SkillImage);
    ButtonStyle.Pressed.SetResourceObject(SkillImage);
    Button_Skill->SetStyle(ButtonStyle); 
    //  SetResourceObject() SetBrushFromTexture();
}

void UAPSkillWindowSlot::InitEnhance()
{
    for(auto Silver : EnhanceData.SilverAbilityNestingNum)
    {
        auto Enhance = CreateWidget<UAPEnhanceSlot>(GetWorld(), EnhanceSlotClass);
        StackBox_Skill->AddChild(Enhance);
        Enhance->InitEnhanceData(ParentWidget.Get(), EEnHanceType::Silver, FName(*SkillRowName), Silver.Key, Silver.Value);
    }
    for(auto Gold : EnhanceData.GoldAbilityNestingNum)
    {
        auto Enhance = CreateWidget<UAPEnhanceSlot>(GetWorld(), EnhanceSlotClass);
        StackBox_Skill->AddChild(Enhance);
        Enhance->InitEnhanceData(ParentWidget.Get(), EEnHanceType::Gold, FName(*SkillRowName), Gold.Key, Gold.Value);
    }
    for(auto Platinum : EnhanceData.PlatinumAbilityNestingNum)
    {
        auto Enhance = CreateWidget<UAPEnhanceSlot>(GetWorld(), EnhanceSlotClass);
        StackBox_Skill->AddChild(Enhance);
        Enhance->InitEnhanceData(ParentWidget.Get(), EEnHanceType::Platinum, FName(*SkillRowName), Platinum.Key, Platinum.Value);
    }
}

void UAPSkillWindowSlot::OnClick_SkillInformation()
{
    UAPSoundSubsystem::PlayUIClickSound(UAPGameInstance::GetSoundGI(GetWorld()));
    auto SkillWindow = Cast<UAPSkillWindow>(ParentWidget.Get()); if(!SkillWindow) return;
    SkillWindow->OpenSkillInformation(EnhanceCategory, SkillNameTable, CoolTime);
}
