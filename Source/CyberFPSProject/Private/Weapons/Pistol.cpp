#include "Weapons/Pistol.h"
#include "Components/Button.h"
#include "Components/WidgetComponent.h"
#include "MyCharacter.h"

APistol::APistol()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APistol::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponUpgradeMenuWidget)
	{
		BurstShootButton = Cast<UButton>(WeaponUpgradeMenuWidget->GetWidgetFromName("BurstShootButton"));
		HeadShotButton = Cast<UButton>(WeaponUpgradeMenuWidget->GetWidgetFromName("HeadShotButton"));
		FireShootRangeIncreaseButton = Cast<UButton>(WeaponUpgradeMenuWidget->GetWidgetFromName("FireShootRangeButton"));
		if (!BurstShootButton || !HeadShotButton || !FireShootRangeIncreaseButton) { return; }
		BurstShootButton->OnClicked.AddDynamic(this, &APistol::BurstShoot);
		HeadShotButton->OnClicked.AddDynamic(this, &APistol::HeadShotDamageIncrease);
		FireShootRangeIncreaseButton->OnClicked.AddDynamic(this, &APistol::FireShootRangeIncrease);
	}
	
}

void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APistol::BurstShoot()
{
	if (!WeaponUpgradeButtonPressedHandle(Upgrade1, BurstShootButton, true)) { return; }
	FireShootCount = 2;
	CanUpgradeNext2 = true;
}

void APistol::HeadShotDamageIncrease()
{
	if (!WeaponUpgradeButtonPressedHandle(Upgrade2, HeadShotButton, CanUpgradeNext2)) { return; }
	HeadShotDamage = 40;
	CanUpgradeNext3 = true;
}

void APistol::FireShootRangeIncrease()
{
	if (!WeaponUpgradeButtonPressedHandle(Upgrade3, FireShootRangeIncreaseButton, CanUpgradeNext3)) { return; }
	FireShootRange = 850.f;
}
