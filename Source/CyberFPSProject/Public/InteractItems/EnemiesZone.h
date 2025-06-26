#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemiesZone.generated.h"

UCLASS()
class CYBERFPSPROJECT_API AEnemiesZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemiesZone();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* EnemiesZoneCollider;

	int32 EnemyCount;

	bool IsPlayerInZone;

	void OnEnemyDied(class ABaseEnemy* DeadEnemy);

protected:
	virtual void BeginPlay() override;

private:
	TArray<class ABaseEnemy*> EnemiesZone;

	void FindEnemiesInZone();

	FTimerHandle FindEnemyHandle;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

};
