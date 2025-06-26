#include "InteractItems/Shop.h"
#include "MyCharacter.h"
#include "BaseWeapon.h"
#include <Components/VerticalBox.h>
#include "Components/WidgetComponent.h"
#include <Components/Button.h>
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "Components/HorizontalBoxSlot.h"
#include <Components/Image.h>
#include "EngineUtils.h"

AShop::AShop()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShop::BeginPlay()
{
	Super::BeginPlay();

	if (ShopMenuWidgetClass)
	{
		ShopMenuWidget = CreateWidget<UUserWidget>(GetWorld(), ShopMenuWidgetClass);
		if (ShopMenuWidget)
		{
			ShopMenuWidget->AddToViewport();
			WeaponSlotList = Cast<UHorizontalBox>(ShopMenuWidget->GetWidgetFromName("WeaponSlots"));
			ShopMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AShop::Interact(AMyCharacter* Character)
{
	if (!Character || !ShopMenuWidget) { return; }
	IsMenuOpen = !IsMenuOpen;
	PlayerCharacter = Character;
	ShopMenu();
}

AShop* AShop::Get(UWorld* World)
{
	for (TActorIterator<AShop> It(World); It; ++It)
	{
		return *It;
	}
	return nullptr;
}

void AShop::ShopMenu()
{
	PC = Cast<APlayerController>(PlayerCharacter->GetController());
	if (!PC) { return; }
	if (!PlayerCharacter) { return; }

	if (IsMenuOpen)
	{
		PC->bShowMouseCursor = true;
		FInputModeGameAndUI GameMode;
		PC->SetInputMode(GameMode);
		ShopMenuWidget->SetVisibility(ESlateVisibility::Visible);
		WeaponUpgradePanel();
	}
	else
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly GameMode;
		PC->SetInputMode(GameMode);
		ShopMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		for (ABaseWeapon* Weapon : PlayerCharacter->WeaponsInventory)
		{
			if (Weapon && Weapon->WeaponUpgradeMenuWidget)
			{
				Weapon->WeaponUpgradeMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void AShop::WeaponUpgradePanel()
{
	if (WeaponSlotList)
	{
		WeaponSlotList->ClearChildren();
		for (ABaseWeapon* Weapon : PlayerCharacter->WeaponsInventory)
		{
			UButton* WeaponUpgradeButton = NewObject<UButton>(this);

			UImage* WeaponImage = NewObject<UImage>(this);

			FSlateBrush Brush;

			Brush.SetResourceObject(Weapon->WeaponImage);
			Brush.ImageSize = FVector2D(64.f, 64.f);
			WeaponImage->SetBrush(Brush);

			WeaponUpgradeButton->AddChild(WeaponImage);

			USizeBox* SizeBox = NewObject<USizeBox>(this);
			SizeBox->SetWidthOverride(130.f);
			SizeBox->SetHeightOverride(100.f);
			SizeBox->AddChild(WeaponUpgradeButton);

			UHorizontalBoxSlot* Slot = WeaponSlotList->AddChildToHorizontalBox(SizeBox);

			if (Slot)
			{
				Slot->SetPadding(FMargin(10.f, 0.f));
				Slot->SetHorizontalAlignment(HAlign_Center);
				Slot->SetVerticalAlignment(VAlign_Center);
			}

			ButtonToIndexMap.Add(WeaponUpgradeButton, Weapon->WeaponType);

			WeaponUpgradeButton->OnClicked.AddDynamic(this, &AShop::SelectedWeaponButton);
		}
	}
}

void AShop::SelectedWeaponButton()
{

	for (auto& Pair : ButtonToIndexMap)
	{
		UButton* Button = Pair.Key;
		EWeaponType Type = Pair.Value;
		if (Button->HasUserFocus(PC))
		{
			for (ABaseWeapon* Weapon : PlayerCharacter->WeaponsInventory)
			{
				if (Weapon && Weapon->WeaponUpgradeMenuWidget)
				{
					Weapon->WeaponUpgradeMenuWidget->SetVisibility(ESlateVisibility::Hidden);
				}
				if (Weapon && Weapon->WeaponType == Type && Weapon->WeaponUpgradeMenuWidget)
				{
					Weapon->WeaponUpgradeMenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
		}
	}
}
