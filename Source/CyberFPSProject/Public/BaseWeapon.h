#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractItems/InteractItem.h"
#include "BaseWeapon.generated.h"
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol,
	Shotgun,
	AssultRifle
};

UCLASS()
class CYBERFPSPROJECT_API ABaseWeapon : public AActor, public IInteractItem
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

	virtual void Interact(class AMyCharacter* Player) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* GunFireEFfectSpawn;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 Ammo;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ReloadTimer;

	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float GunDamage;

	UPROPERTY(EditAnywhere,Category = "UI")
	UTexture2D* WeaponImage;

	UPROPERTY(EditAnywhere, Category = "UI")
	int32 MenuIndex;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 FireShootCount;

	UPROPERTY(EditAnywhere,Category = "UI")
	TSubclassOf<UUserWidget> WeaponUpgradeMenuWidgetClass;

	class UUserWidget* WeaponUpgradeMenuWidget;

	bool IsShowMenu;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float HeadShotDamage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireShootRange;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float RifleFireShootSpeed;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireForce;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float StrongerBullet;

	bool IsStrongerBullet;

	int32 ShotgunFireCount;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int32 Upgrade1;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int32 Upgrade2;

	UPROPERTY(EditAnywhere, Category = "Upgrade")
	int32 Upgrade3;

	bool IsBleedUnlock;

	float BleedingTimer;

	bool IsBleeding;

	FTimerHandle TakeEnemyBleedingHandle;

	int32 RifleShootCount;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BleedingCoolDown;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 AmmoCount;


protected:
	virtual void BeginPlay() override;

	IInteractItem* InteractedObject;

	bool WeaponUpgradeButtonPressedHandle(int32 Cost,class UButton* Button,bool CanNextUpgrade);

	bool CanUpgradeNext2;

	bool CanUpgradeNext3;

private:
	

public:	
	virtual void Tick(float DeltaTime) override;

};
