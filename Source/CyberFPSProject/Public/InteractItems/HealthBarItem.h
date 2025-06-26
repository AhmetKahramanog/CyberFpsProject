#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInteractableItem.h"
#include "HealthBarItem.generated.h"

UCLASS()
class CYBERFPSPROJECT_API AHealthBarItem : public ABaseInteractableItem
{
	GENERATED_BODY()
	
public:	
	AHealthBarItem();

protected:
	virtual void BeginPlay() override;

	virtual void InteractCollision(class AMyCharacter* PlayerCharacter) override;

public:	
	virtual void Tick(float DeltaTime) override;

};
