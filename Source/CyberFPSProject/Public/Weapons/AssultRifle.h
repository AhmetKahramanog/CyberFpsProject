#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BaseWeapon.h"
#include "AssultRifle.generated.h"

UCLASS()
class CYBERFPSPROJECT_API AAssultRifle : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	AAssultRifle();

	class UButton* FireShootSpeedButton;

	class UButton* BleedingDamageButton;

	class UButton* BleedingDamageIncreaseButton;

	UFUNCTION()
	void FireShootSpeed();

	UFUNCTION()
	void BleedingDamage();

	UFUNCTION()
	void BleedingDamageIncrease();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
