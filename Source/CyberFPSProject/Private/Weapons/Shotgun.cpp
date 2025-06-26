#include "Weapons/Shotgun.h"
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include "MyCharacter.h"

AShotgun::AShotgun()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
	if (!WeaponUpgradeMenuWidget) { return; }
	HeadShotImpactButton = Cast<UButton>(WeaponUpgradeMenuWidget->GetWidgetFromName("HeadShotImpactButton"));
	HeadShotImpactUpgradeButton = Cast<UButton>(WeaponUpgradeMenuWidget->GetWidgetFromName("HeadShotImpactUpgradeButton"));
	FiveBulletStrongButton = Cast<UButton>(WeaponUpgradeMenuWidget->GetWidgetFromName("FiveBulletStrongButton"));
	if (!HeadShotImpactButton || !HeadShotImpactUpgradeButton || !FiveBulletStrongButton) { return; }
	HeadShotImpactButton->OnClicked.AddDynamic(this, &AShotgun::HeadShotImpact);
	HeadShotImpactUpgradeButton->OnClicked.AddDynamic(this, &AShotgun::HeadShotImpactUpgrade);
	FiveBulletStrongButton->OnClicked.AddDynamic(this, &AShotgun::FiveBulletStrong);
}


void AShotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (ShotgunFireCount >= 4)
	{
		VFX oynatýlýcak / Material deðiþicek
	}
	else
	{
		VFX kapatýlýcak // varsayýlan materyale dönücek
	}*/

}

void AShotgun::HeadShotImpact()
{
	if (!WeaponUpgradeButtonPressedHandle(Upgrade1, HeadShotImpactButton,true)) { return; }
	FireForce = 200.f;
	CanUpgradeNext2 = true;
}


void AShotgun::HeadShotImpactUpgrade()
{
	if (!WeaponUpgradeButtonPressedHandle(Upgrade2, HeadShotImpactUpgradeButton,CanUpgradeNext2)) { return; }
	FireForce = 4500.f;
	CanUpgradeNext3 = true;
}

void AShotgun::FiveBulletStrong()
{
	if (!WeaponUpgradeButtonPressedHandle(Upgrade3, FiveBulletStrongButton,CanUpgradeNext3)) { return; }
	IsStrongerBullet = true;
}
