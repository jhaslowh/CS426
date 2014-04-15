#include "NPCSniperSoldier.h"

NPCSniperSoldier::NPCSniperSoldier(float x, float y) : NPCBasicCollision()
{
	// Basic NPC Properties
	stype = "sniper";
	locX = x;
	locY = yo = y;
	width = 50.0f;
	height = 50.0f;
	originX = 25.0f;
	originY = 50.0f;
	rotation = 0.0f;
	scale = 1.0f;
	if (rand() % 100 > 50)
		imageID = GI_NPC_SOLDIER_M1;
	else 
		imageID = GI_NPC_SOLDIER_F1;
	next = NULL;
	alive = true;

	// Movement properties
	direcX = 1.0f;
	speed = 100.0f;
	minDistanceTFlop = 100.0f;
	maxDistanceTFlop = 300.0f;
	distanceTillFlop = 200.0f;
	currentDistance = 0.0f;

	// Collision Values
	bounds.reset(0.0f,0.0f,50.0f,50.0f);
	waistHeight = 35.0f;
	depthForDepthCheck = 20.0f;

	// Animation 
	frames = 3;
	cframe = 0;
	frameRate = .1f;
	cframeTime = 0.0f;

	// Arm 
	armOffsetX = 23.0f;
	armOffsetY = 25.0f;
	armOriginX = 2.5f;
	armOriginY = 3.0f;
	armRotation = 30.0f;

	// Weapon properties 
	weapon = (Weapon*)new NPCSniper();
	weaponRange = 650.0f;
	playerInRange = false;
	caquireTime = 0.0f;
	aquireTime = 2.0f;
}

NPCSniperSoldier::~NPCSniperSoldier(){}

// Update movement of the NPC 
// This method should set the movement values for the npc.
void NPCSniperSoldier::updateMovement(float deltaTime, Handlers* handlers){
	NPCBasicCollision::updateMovement(deltaTime, handlers);

	if (alive && !beingAbducted){
		// Walk if not shooting 
		if (!playerInRange){
			nextX = locX + (speed * direcX * deltaTime);

			// Flop soldier
			currentDistance += speed * deltaTime;
			if (currentDistance > distanceTillFlop){
				distanceTillFlop = minDistanceTFlop + 
					(rand() % (int)(maxDistanceTFlop - minDistanceTFlop)); 
				currentDistance = 0.0f;
				direcX = -direcX;
			}

			// Update walk frames 
			cframeTime += deltaTime;
			if (cframeTime > frameRate){
				cframeTime = 0.0f;
				cframe++;
				if (cframe >= frames)
					cframe = 0;
			}
		}
		else 
			// Set current frame to 0 if not shooting so NPC
			// doesn't walk in place. 
			cframe = 0;
	}
}

// Update game state of the npc object
// Do any non movement or collision detection updates
// (weapons and the like) 
void NPCSniperSoldier::update(float deltaTime, Handlers* handlers){
	NPCBasicCollision::update(deltaTime, handlers);

	if (alive){
		// Allways update weapon
		weapon->update(deltaTime, 
			locX - originX + armOffsetX, 
			locY - originY + armOffsetY);

		if (!beingAbducted){
			// Update weapon
			Player* player = (Player*)handlers->player;

			// Try and attack player 
			if (!player->isInUFO() && 
				dist(locX, locY, player->getCenterX(), player->getCenterY()) < weaponRange &&
				((player->getX() > locX && direcX > 0.0f) || 
				(player->getX() < locX && direcX < 0.0f) )){

				// Aquire Target 
				playerInRange = true;
				caquireTime += deltaTime;

				// Fire when target aquired 
				if (caquireTime >= aquireTime){
					// Try to fire at player 
					weapon->npcFire(player->getCenterX(), player->getCenterY(), handlers);
				}
				else {
					// These need to be called, otherwise the weapon will glitch
					// while the npc is aquiring target. 
					weapon->setRotation(0.0f);
					weapon->setFacingDirec(direcX > 0.0f);
					weapon->setRotationByTarget(player->getCenterX(), player->getCenterY());
				}
			}
			else {
				// Reset gun if can't find target
				weapon->setRotation(0.0f);
				weapon->setFacingDirec(direcX > 0.0f);
				playerInRange = false;
				caquireTime = 0.0f;
			}

			armRotation = weapon->getRotation();
		}
	}
}

// Draw object to the screen
void NPCSniperSoldier::draw(GLHandler* mgl, GameAtlas* mGame){
	NPCBasicCollision::draw(mgl, mGame);
	
	if (alive){
		if (direcX > 0){
			// Draw body 
			mGame->draw(mgl, imageID+cframe,locX,locY,scale, rotation, originX, originY);
			// Draw weapon 
			weapon->draw(mgl, mGame);
			// Draw arm 
			mGame->draw(mgl, GI_NPC_SOLDIER_ARM,
				locX-originX+armOffsetX,
				locY-originY+armOffsetY,
				1.0f,armRotation,armOriginX,armOriginY);
		}
		else {
			glCullFace(GL_FRONT);
			// Draw body 
			mGame->drawScale2(mgl, imageID+cframe,locX,locY,-scale,scale, rotation, originX, originY);
			glCullFace(GL_BACK);

			// Draw weapon 
			weapon->draw(mgl, mGame);

			// Draw arm 
			glCullFace(GL_FRONT);
			mGame->drawScale2(mgl, GI_NPC_SOLDIER_ARM,
				locX-originX+(width-armOffsetX),
				locY-originY+armOffsetY,
				-1.0f,1.0f,armRotation,armOriginX,armOriginY);
			glCullFace(GL_BACK);
		}

	}
}


// Called when the NPC runs into a wall
void NPCSniperSoldier::hitWall(){
	NPCBasicCollision::hitWall();

	direcX = -direcX;
}
