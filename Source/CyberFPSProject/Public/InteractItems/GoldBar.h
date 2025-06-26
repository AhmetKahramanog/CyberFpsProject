#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInteractableItem.h"
#include "GoldBar.generated.h"

UCLASS()
class CYBERFPSPROJECT_API AGoldBar : public ABaseInteractableItem
{
	GENERATED_BODY()
	
public:	
	AGoldBar();


	virtual void InteractCollision(class AMyCharacter* PlayerCharacter) override;

protected:
	virtual void BeginPlay() override;

	/*UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

public:	
	virtual void Tick(float DeltaTime) override;

};
