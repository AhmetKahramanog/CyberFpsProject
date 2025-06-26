#include "BaseWeapon.h"
#include "MyCharacter.h"
#include "NiagaraComponent.h"
#include "Components/WidgetComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/Button.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	GunMesh->SetupAttachment(RootComponent);

	GunFireEFfectSpawn = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunFireEffect"));
	GunFireEFfectSpawn->SetupAttachment(GunMesh);

}


void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = Ammo;

	if (WeaponUpgradeMenuWidgetClass)
	{
		WeaponUpgradeMenuWidget = CreateWidget<UUserWidget>(GetWorld(), WeaponUpgradeMenuWidgetClass);
		if (WeaponUpgradeMenuWidget)
		{
			WeaponUpgradeMenuWidget->AddToViewport();
			WeaponUpgradeMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		InteractedObject = Cast<IInteractItem>(PlayerCharacter);
	}

}


void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::Interact(AMyCharacter* Player)
{
	if (Player)
	{
		Player->CurrentWeapon = this;
	}
}

bool ABaseWeapon::WeaponUpgradeButtonPressedHandle(int32 Cost, UButton* Button,bool CanNextUpgrade)
{
	if (!Button) { return false; }

	if (InteractedObject->GetGold() >= Cost && CanNextUpgrade)
	{
		InteractedObject->SetGold(-Cost);
		FButtonStyle NewStyle = Button->WidgetStyle;
		NewStyle.Normal.TintColor = FSlateColor(FLinearColor::Black);
		NewStyle.Hovered.TintColor = FSlateColor(FLinearColor::Black);
		NewStyle.Pressed.TintColor = FSlateColor(FLinearColor::Black);
		Button->SetStyle(NewStyle);
		Button->OnClicked.Clear();
		return true;
	}
	return false;
}

