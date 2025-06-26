#include "InteractItems/GoldBar.h"

AGoldBar::AGoldBar()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AGoldBar::BeginPlay()
{
	Super::BeginPlay();
	
}


void AGoldBar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoldBar::InteractCollision(AMyCharacter* PlayerCharacter)
{
	Super::InteractCollision(PlayerCharacter);
	if (InteractableObjectCollision)
	{
		InteractableObjectCollision->SetGold(GetValue);
	}
}