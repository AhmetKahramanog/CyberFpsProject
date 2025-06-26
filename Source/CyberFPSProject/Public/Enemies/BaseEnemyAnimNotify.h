#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BaseEnemyAnimNotify.generated.h"


UCLASS()
class CYBERFPSPROJECT_API UBaseEnemyAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void OpenCollision();

	class ABaseEnemy* BaseEnemyCharacter;
	
};
