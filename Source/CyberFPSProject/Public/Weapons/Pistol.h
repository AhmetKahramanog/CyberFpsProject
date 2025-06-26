#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BaseWeapon.h"
#include "Pistol.generated.h"

UCLASS()
class CYBERFPSPROJECT_API APistol : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	APistol();

private:
	class UButton* BurstShootButton;

	class UButton* HeadShotButton;

	class UButton* FireShootRangeIncreaseButton;

	UFUNCTION()
	void BurstShoot();

	UFUNCTION()
	void HeadShotDamageIncrease();

	UFUNCTION()
	void FireShootRangeIncrease();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
