#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInteractableItem.h"
#include "AmmoCapacity.generated.h"

UCLASS()
class CYBERFPSPROJECT_API AAmmoCapacity : public ABaseInteractableItem
{
	GENERATED_BODY()
	
public:	
	AAmmoCapacity();

	virtual void InteractCollision(class AMyCharacter* PlayerCharacter) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
