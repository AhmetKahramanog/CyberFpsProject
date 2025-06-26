#include "Enemies/MeleeEnemy.h"

AMeleeEnemy::AMeleeEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(MeleeAttackHandle, this, &AMeleeEnemy::EnemyAttack, AttackTimer, true);

	GetWorldTimerManager().SetTimer(JumpAttackHandle, this, &AMeleeEnemy::JumpAttack, JumpAttackTimer, true);

}


void AMeleeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeleeEnemy::EnemyTakeDamage(float Amount)
{
	Super::EnemyTakeDamage(Amount);
}

void AMeleeEnemy::JumpAttack()
{
	if (IsJumperAttack && Distance() > AttackRange && Distance() < FollowRange)
	{
		UAnimInstance* JumpAttackInstance = GetMesh()->GetAnimInstance();
		if (JumpAttackInstance)
		{
			JumpAttackInstance->Montage_Play(JumpAttackMontage);
		}
	}
}

void AMeleeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

