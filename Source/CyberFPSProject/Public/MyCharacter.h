#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseWeapon.h"
#include "InteractItems/InteractItem.h"
#include "MyCharacter.generated.h"

class UInputAction;
struct FInputActionValue;


UCLASS()
class CYBERFPSPROJECT_API AMyCharacter : public ACharacter, public IInteractItem
{
	GENERATED_BODY()

public:
	AMyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DodgeForce;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpForce;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UNiagaraSystem* GunFireEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UMaterialInterface* BulletHole;


	class ABaseWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* WeaponKeeper;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxHealth;


	void GetDamage(float Amount);

	TArray<class ABaseWeapon*> WeaponsInventory;

	class ABaseWeapon* CurrentlyGun;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PlayerUIWidgetClass;

	virtual void SetGold(int32 GoldAmount) override;

	virtual int32 GetGold() override;

	UPROPERTY(EditAnywhere,Category = "UI")
	UTexture2D* ShotgunCrossImage;

	UPROPERTY(EditAnywhere, Category = "UI")
	UTexture2D* NormalCrossImage;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UCameraShakeBase> BP_DamageCameraShake;

	virtual void SetAmmo(int32 AmmoAmount) override;

	virtual void SetHealth(int32 HealthAmount) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxStamina;

	float CurrentStamina;


protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue &Value);

	void Look(const FInputActionValue& Value);

	void StartSprint();

	void StopSprint();

	void Dodge();

	void Jump();

	void Fire();

	UFUNCTION()
	void OnWeaponReloaded();


	int32 GoldCount;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	float WalkSpeed;

	void MiddleLineTrace();

	APlayerController* PlayerController;

	void PickUpWeapon();

	ABaseWeapon* WeaponCloned;

	bool IsFire;

	FTimerHandle FireHandle;

	FTimerHandle FireEffectHandle;

	FRotator WeaponInitialRotation;

	FRotator WeaponTargetRotation;

	class UNiagaraComponent* GunFireEffectComponent;

	void FireLineTrace();

	FHitResult FireResult;

	FTimerHandle ReloadHandle;

	bool IsReloaded;

	void GunAnimation(FRotator TargetRotation,float RotationTimer,float DeltaTime);


	void EquippedWeapon(int32 Index);

	void SelectWeapon(FKey PressedKey);

	void ShotgunTrace();

	EWeaponType UsedWeaponType;

	void AssultRifleFireStart();

	void AssultRifleFireStop();

	FTimerHandle AutoFireHandle;

	

	void HandleHitResult(FVector Start,FVector End);

	float CurrentHealth;

	void ClimbWall();

	FTimerHandle ClimbWallHandle;

	IInteractItem* Interacted;

	bool IsGunAnimated;

	FTimerHandle FireDelayHandle;

	int32 FireCount;

	int32 ShotgunFireCount;

	class ABaseEnemy* BaseEnemyShoot;

	class UUserWidget* PlayerUIWidget;

	class UImage* CrossImage;

	class UTextBlock* AmmoText;

	class UTextBlock* AmmoCapacitytText;

	class UProgressBar* HealthBarProgress;

	void AmmoTextUpdate();

	void HealthBarUpdate();

	bool IsClimbing;

	//Dash Deneme
	void InterpDashTest(float DeltaTime);

	bool IsDashing;

	FTimerHandle DashTestHandle;

	class UProgressBar* StaminaBar;

	bool IsShowStaminaBar;

	FTimerHandle StaminaBarHandle;

	class UTextBlock* GoldCountText;

	class UTextBlock* InteractText;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
