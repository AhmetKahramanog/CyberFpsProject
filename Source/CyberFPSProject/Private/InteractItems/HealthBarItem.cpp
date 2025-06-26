#include "InteractItems/HealthBarItem.h"

AHealthBarItem::AHealthBarItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHealthBarItem::BeginPlay()
{
	Super::BeginPlay();
	
}


void AHealthBarItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthBarItem::InteractCollision(AMyCharacter* PlayerCharacter)
{
	Super::InteractCollision(PlayerCharacter);
	if (InteractableObjectCollision)
	{
		InteractableObjectCollision->SetHealth(GetValue);
	}
}