#include "Enemies/BaseEnemyAnimInstance.h"
#include "Enemies/BaseEnemy.h"

void UBaseEnemyAnimInstance::NativeInitializeAnimation()
{
	EnemyCharacter = Cast<ABaseEnemy>(TryGetPawnOwner());
}

void UBaseEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (!EnemyCharacter) { return; }
	IsMoving = EnemyCharacter->GetVelocity().Size2D() > 0.5f;
	IsAimingIdle = EnemyCharacter->Distance() < EnemyCharacter->AttackRange;
	IsRanger = EnemyCharacter->EnemyIsRanger;

}
