#include "Weapons/AssultRifle.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
AAssultRifle::AAssultRifle()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAssultRifle::BeginPlay()
{
	Super::BeginPlay();
	if (WeaponUpgradeMenuWidget)
	{
		FireShootSpeedButton = Cast<UButton>(WeaponUpgradeMenuWidget->GetWidgetFromName("FireSpeedButton"));
		BleedingDamageButton = Cast<UButton>(WeaponUpgradeMenuWidget->GetWidgetFromName("BleedingDamageButton"));
		BleedingDamageIncreaseButton = Cast<UButton>(WeaponUpgradeMenuWidget->GetWidgetFromName("BleedingDamageIncreaseButton"));
		if (!FireShootSpeedButton || !BleedingDamageButton || !BleedingDamageIncreaseButton) { return; }
		FireShootSpeedButton->OnClicked.AddDynamic(this, &AAssultRifle::FireShootSpeed);
		BleedingDamageButton->OnClicked.AddDynamic(this, &AAssultRifle::BleedingDamage);
		BleedingDamageIncreaseButton->OnClicked.AddDynamic(this, &AAssultRifle::BleedingDamageIncrease);
	}
}

void AAssultRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BleedingTimer > 7.f)
	{
		IsBleeding = false;
		GetWorld()->GetTimerManager().ClearTimer(TakeEnemyBleedingHandle);
		RifleShootCount = 0;
	}

	if (IsBleeding)
	{
		BleedingTimer += DeltaTime;
	}
	else
	{
		BleedingTimer = 0.f;
	}
	
}

void AAssultRifle::BleedingDamage()
{
	if (!WeaponUpgradeButtonPressedHandle(Upgrade1, BleedingDamageButton, true)) { return; }
	IsBleedUnlock = true;
	CanUpgradeNext2 = true;
}

void AAssultRifle::BleedingDamageIncrease()
{
	if (!WeaponUpgradeButtonPressedHandle(Upgrade3, BleedingDamageIncreaseButton, CanUpgradeNext2)) { return; }
	BleedingCoolDown = 0.2f;
	CanUpgradeNext3 = true;
}


void AAssultRifle::FireShootSpeed()
{
	if (!WeaponUpgradeButtonPressedHandle(Upgrade1, FireShootSpeedButton, CanUpgradeNext3)) { return; }
	RifleFireShootSpeed = 0.23f;
}




