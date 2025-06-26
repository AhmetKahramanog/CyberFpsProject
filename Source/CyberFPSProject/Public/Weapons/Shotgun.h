#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BaseWeapon.h"
#include "Shotgun.generated.h"

UCLASS()
class CYBERFPSPROJECT_API AShotgun : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	AShotgun();

	class UButton* HeadShotImpactButton;

	class UButton* HeadShotImpactUpgradeButton;

	class UButton* FiveBulletStrongButton;

	UFUNCTION()
	void HeadShotImpact();

	UFUNCTION()
	void HeadShotImpactUpgrade();

	UFUNCTION()
	void FiveBulletStrong();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
