#include "ProjectileHandler.h"
//almost did ifndef/endif out of habit

ProjectileHandler::ProjectileHandler()
{
	// Make new projectile list
	projectiles = std::list<Projectile*>();
	// Fill with dead projectiles 
	for (int i = 0; i < 100; i++)
		projectiles.push_back(new Projectile());
	
	addIndex = projectiles.begin();

	UIDIterator = 0;
}
ProjectileHandler::~ProjectileHandler()
{
	cleanUp();
}
//Constructor and Deconstructor


// Returns the projectile list
std::list<Projectile*> ProjectileHandler::getProjList(){return projectiles;}

//Pass in a constructor Projectile to be contained by the list
void ProjectileHandler::addNewProjectile(Projectile* newProjectile)
{
	int skipped = 0;

	// Add until dead or limit reached 
	while (skipped < (int)projectiles.size()){
		// If current projectile is dead, add in place
		if (!(*addIndex)->getAlive()){
			(*addIndex)->clone(newProjectile);
			delete newProjectile;
			addIndex++;
			if (addIndex == projectiles.end())
				addIndex = projectiles.begin();
			return;
		}
		// If slot filled, check next slot 
		else {
			skipped++;
			addIndex++;
			if (addIndex == projectiles.end())
				addIndex = projectiles.begin();
		}
	}

	// If list filled, add to end 
	newProjectile->setUID(UIDIterator);
	projectiles.push_back(newProjectile);
	UIDIterator++;
}

void ProjectileHandler::addProjectile(short ProjectileType, float CurrentX, float CurrentY, int Mass, int Size, float xLocation, float yLocation, int speed, bool doesExplode)
{
	// TODO implement
}

void ProjectileHandler::updateProjectiles(float deltaTime, Handlers* handlers)
{
	for(std::list<Projectile*>::iterator myIterator = projectiles.begin(); myIterator != projectiles.end(); myIterator++)
	{
		// Null check 
		if (*myIterator != NULL && (*myIterator)->getAlive()){
			(*myIterator)->determineNegligance();
			if((*myIterator)->getNegligence())
			{
				//std::cout << "Negligable";
				(*myIterator)->updateNegligableProjectile(deltaTime);
			}
			else
			{
				//std::cout << "Physicsable";
				(*myIterator)->updateProjectile(deltaTime, handlers);
			}
		}
	}
}

void ProjectileHandler::removeProjectile(Projectile* removeProjectile)
{
	for(std::list<Projectile*>::iterator myIterator = projectiles.begin(); myIterator != projectiles.end(); myIterator++)
	{
		if(*myIterator != NULL && *myIterator == removeProjectile && (*myIterator)->getUID() ==removeProjectile->getUID())
			projectiles.erase(myIterator);
	}
}

// Draw all projectiles 
void ProjectileHandler::draw(GLHandler* mgl, TextureAtlas* mAtlas){
	for(std::list<Projectile*>::iterator myIterator = projectiles.begin(); myIterator != projectiles.end(); myIterator++)
	{
		if(*myIterator != NULL)
			(*myIterator)->draw(mgl, mAtlas);
	}

	// Reset flat color here instead of every time a projectile draws
	mgl->setFlatColor(1.0f,1.0f,1.0f,1.0f);
}

// Draw all projectiles lights 
void ProjectileHandler::drawLight(GLHandler* mgl, TextureAtlas* mAtlas){
	for(std::list<Projectile*>::iterator myIterator = projectiles.begin(); myIterator != projectiles.end(); myIterator++)
	{
		if(*myIterator != NULL)
			(*myIterator)->drawLight(mgl, mAtlas);
	}

	// Reset flat color here instead of every time a projectile draws
	mgl->setFlatColor(1.0f,1.0f,1.0f,1.0f);
}

void ProjectileHandler::cleanUp()
{
	projectiles.clear();
}