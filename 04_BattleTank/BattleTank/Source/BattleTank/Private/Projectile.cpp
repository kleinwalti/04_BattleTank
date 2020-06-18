// Copyright Michael Waltersdorf.

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Adds a Projectile Movement Component to the Projectile-Class, so that the editor can not delete it accidently in blueprint
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->bAutoActivate = false;

	// Adds a Static Mesh 'CollisionMesh' as a component to the Projectile Class
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision Mesh"));
	// Forces the Collision Mesh to be the root component of the Projectile Class
	SetRootComponent(CollisionMesh);
	// Defaults the Details->Collision->'Simulation Generats Hit Events' Setting to true
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	// Defaults the Details->Rendering->Visible Setting to false (because usually we only see the smoke of the projectile, because it is so fast -.-)
	CollisionMesh->SetVisibility(false);

	// Adds a Particle System 'Launch Blast' as a component to the Projectile Class
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Launch Blast"));
	// Forces the LaunchBlast to be attached to the Collision Mesh
	LaunchBlast->AttachTo(CollisionMesh);

	// Adds a Particle System 'Impact Blast' as a component to the Projectile Class
	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Impact Blast"));
	// Forces the LaunchBlast to be attached to the Collision Mesh
	ImpactBlast->AttachTo(CollisionMesh);
	// Do not auto-activate the Impact Blast when the Projectile spawns
	ImpactBlast->bAutoActivate = false;

	// Adds a Radial Force Component 'Explosion Force'
	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Explosion Force"));
	// Forces it to be attached to the Collision Mesh which also brings a transform and lets the force not start at the center of the world
	ExplosionForce->AttachTo(CollisionMesh);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	// Register delegate at begin play like this:
    CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Activate the Impact Blast
	ImpactBlast->Activate();

	// TFire the Impulse, and then Activate the component so the impulse can affect the world
	ExplosionForce->FireImpulse();

	// Change the Root Component not to be the Collision Mesh and then delete the Collision Mesh
	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	// Cause some damage, Radial, which will call TakeDamage() on Actors
	if (
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionForce->Radius, UDamageType::StaticClass(), TArray<AActor*>() )
	)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Damage will be applied"));
	}

	// After a Time Deloy, destroy (delete) the projectile from the game
	FTimerHandle MyTimerHandle;	// just used as a container
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &AProjectile::OnTimerExpire, DestroyDelay);
}

void AProjectile::LaunchProjectile(float LaunchSpeed)
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
	ProjectileMovementComponent->Activate();
}

void AProjectile::OnTimerExpire()
{
	Destroy();
}