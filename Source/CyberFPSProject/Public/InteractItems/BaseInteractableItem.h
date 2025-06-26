#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../InteractItems/InteractItem.h"
#include "BaseInteractableItem.generated.h"

UCLASS()
class CYBERFPSPROJECT_API ABaseInteractableItem : public AActor, public IInteractItem
{
	GENERATED_BODY()
	
public:	
	ABaseInteractableItem();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* InteractableMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollider;

	UPROPERTY(EditAnywhere, Category = "Value")
	int32 GetValue;


	virtual void InteractCollision(class AMyCharacter* PlayerCharacter) override;

protected:
	virtual void BeginPlay() override;

	IInteractItem* InteractableObjectCollision;

public:	
	virtual void Tick(float DeltaTime) override;

};
