#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"


UCLASS()
class CYBERFPSPROJECT_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxHealth;

	float EnemyCurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UNiagaraSystem* BloodHeadEffect;

	virtual void EnemyTakeDamage(float Amount);

	void BloodEffectSpawn(FName BoneName);

	virtual void EnemyMovement(float DeltaTime);

	virtual void EnemyRotation(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float FollowRange;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AttackRange;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float FollowSpeed;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* EnemyWeaponKeeper;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<UAnimMontage*> AttackMontages;

	float Distance();

	UPROPERTY(EditAnywhere)
	bool EnemyIsRanger;

	void EnemyBackForce(float Force);

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackTimer;

	void Die();

	bool IsDie;

	class AEnemiesZone* OwnerZone;

protected:
	virtual void BeginPlay() override;

	virtual void EnemyAttack();

	class AMyCharacter* PlayerCharacter;

private:
	class UNiagaraComponent* BloodComponent;

	class UNiagaraComponent* BloodHeadComponent;

	FTimerHandle BloodEffectHandle;

	FTimerHandle BloodHeadEffectHandle;

	int32 AttackIndex;

	void BloodEffectDespawn(FTimerHandle& TimerHandle, class UFXSystemComponent* Component);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
