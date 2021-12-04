#include "Player.h"

Player::Player(void) {
	myChar = new Carver();

	heading = Vector3D(-1.0f, 0.0f, 0.0f);
	airHeading = Vector3D(-1.0f, 0.0f, 0.0f);

	speed = 0.0f;
	lastFallSpeed = 0.0f;
	flying = 0.0f;
	totalAngle = 0.0f;
	stopped = false;
	standUpTime = 0.0f;
	jumpBonus = 0.0f;
	useJumpBonus = oldUseJumpBonus = 0;
	newBonus = false;
	name = "";
	lanReady = false;
}

Player::Player(Player* copy) {
	if(copy->myChar->getType() == AVATAR_TYPE_CARVER)
		myChar = new Carver(copy->myChar);
	else
		myChar = new Boarder(copy->myChar);

	heading = copy->heading;
	airHeading = copy->airHeading;
	speed = copy->speed;
	lastFallSpeed = copy->lastFallSpeed;
	flying = copy->flying;
	jumpBonus = copy->jumpBonus;
	useJumpBonus = copy->useJumpBonus;
	oldUseJumpBonus = copy->oldUseJumpBonus;
	newBonus = copy->newBonus;
	totalAngle = 0.0f;
	name = copy->name;

	stopped = copy->stopped;
	camRot = copy->camRot;
	pos = copy->pos;
	rot = copy->rot;
	radius = copy->radius;
	lanReady = copy->lanReady;
}

Player::~Player(void) {
#ifdef _DEBUG
	std::cout << "Loesche Spieler \"" << name << "\"\n";
#endif
	if(myChar)	delete myChar;
}


void Player::setName(const std::string newName) {
	name = newName;
}

void Player::turnPlayer(float angle) {
	Matrix3D mat = Matrix3D();
	mat.createRotationMatrix(0.0f, angle, 0.0f);
	//if(!stopped) {
	heading = mat * heading;
	rotate(0.0f, angle, 0.0f);
	//}

	totalAngle += angle;
	if(totalAngle > 360.0f)
		totalAngle -= 360.0f;
	if(totalAngle < 0.0f)
		totalAngle += 360.0f;
}

void Player::accPlayer(float factor) {
	if(standUpTime <= 0.0f)		// not crashed
		speed += factor;
	if(speed < 0) speed = 0.0f;
	if(speed > MATTERHORN_MAX_SPEED) speed = MATTERHORN_MAX_SPEED;
}

void Player::render() const {
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rot.z, 0.0f, 0.0f, 1.0f);

	float angle = airHeading.getAngle(Vector3D(airHeading.x, 0.0f, airHeading.z));
	if(airHeading.y > 0)
		angle *= -1.0f;
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	if(standUpTime > 0.0f) {
		float fallAngle = 90.0f * sin((standUpTime / STAND_UP_DELAY) * PI / 2.0f);
		glRotatef(fallAngle, 1.0f, 0.0f, 0.0f);
	}

	myChar->render();

#ifdef _DEBUG
	// render heading and airHeading vectors
	glRotatef(-rot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(-rot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(-rot.z, 0.0f, 0.0f, 1.0f);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(heading.x, heading.y, heading.z);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(airHeading.x, airHeading.y, airHeading.z);
	glEnd();
#endif

	glPopMatrix();
}

void Player::crashAgainst(Vector3D otherPos) {
	if(!isStopped()) {		//  && standUpTime <= 0.0f
		Vector3D diff = getPosition() - otherPos;
		diff.normalize();
		if(standUpTime <= 0.0f)
			diff *= speed;
		move(diff);

		standUpTime = STAND_UP_DELAY;
		speed = 0.0f;
		setJumpBonus(0.0f);

#ifdef _DEBUG
		std::clog << name << ": *CRASH!* (diff.len: " << diff.length() << ") \n";
#endif
	}
}

void Player::switchPlayerChar() {
	int oldType = myChar->getType();
	Avatar* toBeDeleted = myChar;
	switch(oldType) {
	case AVATAR_TYPE_BOARDER:
		myChar = new Carver(toBeDeleted);
		break;
	case AVATAR_TYPE_CARVER:
	default:
		myChar = new Boarder(toBeDeleted);
		break;
	}
	delete toBeDeleted;
}

void __fastcall Player::setSpeed(float s) {
	if(s >= 0.0f && !isStopped())
		speed = s;
	else
		speed = 0.0f;
}

void Player::update(float timeDiff, Vector3D normal) {
	if(!normal.isNull()) {
		// normal == (0/0/0) genau am Startpunkt.
		Vector3D downAxis = Vector3D(normal, Vector3D(0.0f, -1.0f, 0.0f));
		downAxis.normalize();
		Vector3D downhill = Vector3D(&normal);
		Matrix3D mat = Matrix3D();
		mat.createRotationMatrix(-90.0f * downAxis.x, -90.0f * downAxis.y, -90.0f * downAxis.z);
		downhill = mat * downhill;

		float hAngle = Vector3D(downhill.x, 0.0f, downhill.z).getAngle(Vector3D(heading.x, 0.0f, heading.z));
		heading.y = downhill.y * (1.0f - (hAngle / 90.0f));
		heading.normalize();
	}
	Vector3D change;
	if(stopTimer > 0.0f) {
		stopTimer -= timeDiff;
		if(stopTimer <= 0.0f) {
			stopTimer = 0.0f;
			stop();
		}
	}
	if(standUpTime > 0.0f) {
		standUpTime -= timeDiff;
		if(standUpTime <= 0.0f) {
			standUpTime = 0.0f;
			accPlayer(1.4f);
		}
	}

	if(!stopped) {
		if(!isFlying()) {
			accPlayer(timeDiff * -heading.y);
			airHeading = heading;
		}

		change = airHeading;
		if(isFlying())
			change.y = 0.0f;
		change = change * (timeDiff * speed);
		move(change.x, change.y, change.z);
	}
}

void Player::setJumpBonus(float newBonusValue) {
	lastFallSpeed += newBonusValue;
	jumpBonus = newBonusValue;
	useJumpBonus = 0;
	newBonus = false;
}

void Player::jump() {
	if(jumpBonus >= 0.0f && flying <= 0.0f && speed >= 1.4f) {
		useJumpBonus = -1;
		oldUseJumpBonus = useJumpBonus;
		jumpBonus = 0.0f;
        //jumpBonus += MATTERHORN_MAX_JUMP_BONUS;
	}
}

float __fastcall Player::getCurrentJumpBonus() const {
	return (jumpBonus / MATTERHORN_MAX_JUMP_BONUS);
}

void __fastcall Player::fall(float timeDiff, float height) {
	float fallSpeed = (timeDiff * MATTERHORN_GRAVITY) + lastFallSpeed;
	if(fallSpeed > MATTERHORN_MAX_SPEED)
		fallSpeed = MATTERHORN_MAX_SPEED;
	float change = fallSpeed * timeDiff;

	if (useJumpBonus != 0) {
		jumpBonus += (timeDiff * MATTERHORN_GRAVITY * useJumpBonus);
		if(jumpBonus < MATTERHORN_MAX_JUMP_BONUS) {
			jumpBonus = MATTERHORN_MAX_JUMP_BONUS;
			useJumpBonus = +1;
		}
		if (jumpBonus > 0.0f) {
			jumpBonus = 0.0f;
			useJumpBonus = 0;
		}
		oldUseJumpBonus = useJumpBonus;
	}

	if(fallSpeed < 0.0f && isFlying()) {
		if(fallSpeed < -MATTERHORN_MAX_SPEED)
			fallSpeed = -MATTERHORN_MAX_SPEED;
		fallSpeed /= 2.0f;
		if(abs(fallSpeed) > getSpeed())
			fallSpeed = getSpeed();
	}
	lastFallSpeed = fallSpeed;

	if (height < change) {
		change = height;
		if(isFlying()) {
			// just landet, adjust speed to landing
			float speedAngle = heading.getAngle(Vector3D(heading.x, 0.0f, heading.z));	// [0; 90]
			speedAngle /= 90.0f;	// [0.0f; 1.0f]
			float newSpeed = getSpeed() * (1.0f - speedAngle);
			if(heading.y > 0.0f)
				speedAngle *= -1.0f;
			newSpeed += lastFallSpeed * speedAngle / 2.0f;
			setSpeed(newSpeed);

			// if we turned to much in air, we'll crash now
			Vector3D a(&airHeading);
			Vector3D b(&heading);
			a.y = 0.0f;
			b.y = 0.0f;
			float landingAngle = a.getAngle(b);
			if(landingAngle > 90.0f)
				crashAgainst(pos - airHeading);

			jumpBonus = 0.0f;
			useJumpBonus = 0;
			oldUseJumpBonus = useJumpBonus;
			newBonus = false;
		}

		lastFallSpeed = height / (timeDiff);
		if(lastFallSpeed >= MATTERHORN_MAX_SPEED)
			lastFallSpeed = MATTERHORN_MAX_SPEED;
		flying = 0.0f;
		if(useJumpBonus != oldUseJumpBonus)
			useJumpBonus = oldUseJumpBonus;	// We seemed to start flying, but we didn't take off.
	}
	else if(!isFlying()) {
		// we're just starting to fly
		if(flying <= 0.0f && useJumpBonus != 0) {
			lastFallSpeed += jumpBonus;
			//jumpBonus = 0.0f;
			oldUseJumpBonus = useJumpBonus;
			useJumpBonus = 0;
			newBonus = true;
		}
		flying += timeDiff;
		airHeading = heading;		//Vector3D(heading.x, 0.0f, heading.z);
	}
	move(0.0f, -change, 0.0f);

	// if we fell down, keep us on ground (but allow some bumping)
	if(standUpTime > 0.0f && lastFallSpeed < -1.0f)
		lastFallSpeed = -1.0f;
}

bool Player::isNewBonus() {
	if(newBonus) {
		newBonus = false;
		return true;
	}
	return false;
}

void __fastcall Player::stop(float delay) {
	if(delay <= 0.0f) {
		stopped = true;
		speed = 0.0f;
		lastFallSpeed = 0.0f;
		flying = 0.0f;
		standUpTime = 0.0f;
		jumpBonus = 0.0f;
		newBonus = false;
		useJumpBonus = 0;
		oldUseJumpBonus = useJumpBonus;
		stopTimer = 0.0f;
		//lanReady = false;
#ifdef _DEBUG
		std::cout << name << " has stopped.\n";
#endif
	}
	else {
		stopTimer = delay;
#ifdef _DEBUG
		std::cout << name << ": Stop in " << stopTimer << " seconds...\n";
#endif
	}
}

void __fastcall Player::restart(float delay) {
	if(delay == 0.0f) {
		stopped = false;
		airHeading = heading;
		speed = 0.0f;
		lastFallSpeed = 0.0f;
		flying = 0.0f;
		stopTimer = 0.0f;
		standUpTime = 0.0f;
		jumpBonus = 0.0f;
		newBonus = false;
		useJumpBonus = 0;
		oldUseJumpBonus = useJumpBonus;
#ifdef _DEBUG
		std::cout << name << " was restarted.\n";
#endif
	}
}

#ifdef _DEBUG
void Player::fullReport() const {
	// Vorbereitung
	std::clog << name;
	if(isStopped())
		std::clog << " (stopped)";
	std::clog << ":\t";

	// Daten ab hier:

	// Position
	std::clog << "Pos(" << pos.x << "/" << pos.y << "/" << pos.z << ") ";
	// Speed
	std::clog << "Speed(" << speed << ") Falling(" << lastFallSpeed << ") ";

	// Ende
	std::clog << '\n';
}
#endif
