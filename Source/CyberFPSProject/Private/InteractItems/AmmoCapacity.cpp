#include "InteractItems/AmmoCapacity.h"

AAmmoCapacity::AAmmoCapacity()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AAmmoCapacity::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAmmoCapacity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoCapacity::InteractCollision(AMyCharacter* PlayerCharacter)
{
	Super::InteractCollision(PlayerCharacter);
	if (InteractableObjectCollision)
	{
		InteractableObjectCollision->SetAmmo(GetValue);
	}
}