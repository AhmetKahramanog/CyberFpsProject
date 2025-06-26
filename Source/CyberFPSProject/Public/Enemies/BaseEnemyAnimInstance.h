#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInstance.generated.h"

UCLASS()
class CYBERFPSPROJECT_API UBaseEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	bool IsMoving;

	UPROPERTY(BlueprintReadOnly)
	bool IsAimingIdle;

	UPROPERTY(BlueprintReadOnly)
	bool IsRanger;

public:
	virtual void NativeInitializeAnimation() override;

	virtual	void NativeUpdateAnimation(float DeltaTime) override;

private:
	class ABaseEnemy* EnemyCharacter;
	
};
