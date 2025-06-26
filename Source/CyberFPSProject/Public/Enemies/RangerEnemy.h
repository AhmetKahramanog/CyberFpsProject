#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "RangerEnemy.generated.h"

UCLASS()
class CYBERFPSPROJECT_API ARangerEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ARangerEnemy();

	virtual void EnemyAttack() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GunFireEffectSpawn;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UNiagaraSystem* GunFireEffect;

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle ShootHandle;

	void DelayShooting();

	FTimerHandle DelatShootHandle;

	FTimerHandle GunFireHandle;

	class UNiagaraComponent* GunFireEffectComponent;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
