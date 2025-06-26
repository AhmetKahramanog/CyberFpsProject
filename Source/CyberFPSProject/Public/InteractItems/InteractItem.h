#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractItem.generated.h"

UINTERFACE(MinimalAPI)
class UInteractItem : public UInterface
{
	GENERATED_BODY()
};


class CYBERFPSPROJECT_API IInteractItem
{
	GENERATED_BODY()

public:
	virtual void Interact(class AMyCharacter* Player);

	virtual int32 GetGold();

	virtual void SetGold(int32 GoldAmount);

	virtual	void InteractCollision(class AMyCharacter* Player);

	virtual void SetAmmo(int32 AmmoAmount);

	virtual void SetHealth(int32 HealthAmount);
};
