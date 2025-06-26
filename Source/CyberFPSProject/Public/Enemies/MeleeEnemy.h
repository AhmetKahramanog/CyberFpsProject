#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "MeleeEnemy.generated.h"

UCLASS()
class CYBERFPSPROJECT_API AMeleeEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	AMeleeEnemy();

	virtual void EnemyTakeDamage(float Amount) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly)
	bool IsJumperAttack;

	UPROPERTY(EditInstanceOnly)
	UAnimMontage* JumpAttackMontage;

	UPROPERTY(EditInstanceOnly)
	float JumpAttackTimer;

private:
	FTimerHandle MeleeAttackHandle;

	FTimerHandle JumpAttackHandle;

	void JumpAttack();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
