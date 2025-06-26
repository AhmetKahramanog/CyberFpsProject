#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractItem.h"
#include "../BaseWeapon.h"
#include "Shop.generated.h"

UCLASS()
class CYBERFPSPROJECT_API AShop : public AActor,public IInteractItem
{
	GENERATED_BODY()
	
public:	
	AShop();

	static AShop* Get(UWorld* World);

	virtual void Interact(class AMyCharacter* Character) override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ShopMenuWidgetClass;

protected:
	virtual void BeginPlay() override;

private:
	bool IsMenuOpen;

	class AMyCharacter* PlayerCharacter;


	class UUserWidget* ShopMenuWidget;

	void ShopMenu();

	void WeaponUpgradePanel();

	class UHorizontalBox* WeaponSlotList;

	UFUNCTION()
	void SelectedWeaponButton();

	UPROPERTY()
	TMap<class UButton*, EWeaponType> ButtonToIndexMap;

	APlayerController* PC;

public:	
	virtual void Tick(float DeltaTime) override;

};
