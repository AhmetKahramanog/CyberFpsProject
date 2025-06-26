#include "InteractItems/BaseInteractableItem.h"
#include "Components/SphereComponent.h"
#include "MyCharacter.h"

ABaseInteractableItem::ABaseInteractableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	InteractableMesh->SetupAttachment(RootComponent);

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(InteractableMesh);

}


void ABaseInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseInteractableItem::InteractCollision(AMyCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) { return; }
	if (PlayerCharacter->GetClass()->ImplementsInterface(UInteractItem::StaticClass()))
	{
		InteractableObjectCollision = Cast<IInteractItem>(PlayerCharacter);
		Destroy();
	}
}

