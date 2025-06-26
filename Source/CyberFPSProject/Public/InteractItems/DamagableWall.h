#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagableWall.generated.h"

UCLASS()
class CYBERFPSPROJECT_API ADamagableWall : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamagableWall();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WallMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* WallBoxCollider;

	UFUNCTION()
	void OnWallDamageBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWallHitGround(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	FTimerHandle BackNormalWallHandle;

};
