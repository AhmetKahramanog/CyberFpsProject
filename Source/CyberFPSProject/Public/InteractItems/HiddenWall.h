#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HiddenWall.generated.h"

UCLASS()
class CYBERFPSPROJECT_API AHiddenWall : public AActor
{
	GENERATED_BODY()
	
public:	
	AHiddenWall();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* HiddenWallCollider;

	UPROPERTY(EditInstanceOnly)
	class AEnemiesZone* EnemyArea;

protected:
	virtual void BeginPlay() override;

private:
	void WallHandle(ECollisionEnabled::Type CollisionType,bool OverlapEvent);

	FTimerHandle WallOffHandle;

public:	
	virtual void Tick(float DeltaTime) override;

};
