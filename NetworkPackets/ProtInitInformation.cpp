#include ".\protinitinformation.h"

ProtInitInformation::ProtInitInformation(void)
{
	prot = PROT_INIT_INFORMATION_ID;
}

ProtInitInformation::~ProtInitInformation(void)
{
}

void ProtInitInformation::giveInformation(unsigned int newPlayerNum, const char *newName, int newAvatarType, Color newHair, Color newSkin, Color newShirt, Color newGloves, Color newTrousers, Color newBoots)
{
	playerNum = newPlayerNum;
	avatarType = newAvatarType;
	hair = newHair;
	skin = newSkin;
	shirt = newShirt;
	gloves = newGloves;
	trousers = newTrousers;
	boots = newBoots;
	
	// copy name
	unsigned int length = 0;
	while (newName[length] != '\0') {
		length++;
	}
	name = new char[length+1];
	memcpy(name, newName, (length+1)*sizeof(char));

	createString();
}

char* ProtInitInformation::getName()
{
	return name;
}

int ProtInitInformation::getAvatarType()
{
	return avatarType;
}

Color ProtInitInformation::getHair()
{
	return hair;
}

Color ProtInitInformation::getSkin()
{
	return skin;
}

Color ProtInitInformation::getShirt()
{
	return shirt;
}

Color ProtInitInformation::getGloves()
{
	return gloves;
}

Color ProtInitInformation::getTrousers()
{
	return trousers;
}

Color ProtInitInformation::getBoots()
{
	return boots;
}

void ProtInitInformation::restString()
{
	unsigned int counter = counterBegin();

	// Name
	unsigned int runner = MAX_NAME_LENGTH * sizeof(char);
    for (unsigned int i = 0; i < runner; i++) {
		memcpy(packetString+counter+i, name+i, sizeof(char));
        if (name[i] == '\0') break;
	}
    counter += runner;

    // Type (Carver/Boarder)
    memcpy(packetString+counter, &avatarType, sizeof(int));
    counter += sizeof(int);

    // Colors:

    // Hair Color
	memcpy(packetString+counter, &hair.r, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &hair.g, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &hair.b, sizeof(float));
	counter += sizeof(float);
		
	// Skin Color
	memcpy(packetString+counter, &skin.r, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &skin.g, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &skin.b, sizeof(float));
	counter += sizeof(float);

	// Shirt Color
	memcpy(packetString+counter, &shirt.r, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &shirt.g, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &shirt.b, sizeof(float));
	counter += sizeof(float);

	// Gloves Color
	memcpy(packetString+counter, &gloves.r, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &gloves.g, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &gloves.b, sizeof(float));
	counter += sizeof(float);

	// Trousers Color
	memcpy(packetString+counter, &trousers.r, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &trousers.g, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &trousers.b, sizeof(float));
	counter += sizeof(float);

	// Boots Color
	memcpy(packetString+counter, &boots.r, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &boots.g, sizeof(float));
	counter += sizeof(float);
	memcpy(packetString+counter, &boots.b, sizeof(float));
	counter += sizeof(float);
}

void ProtInitInformation::restInformation()
{
	unsigned int counter = counterBegin();

	// Name
	unsigned int length = 0;
	while (packetString[counter+length] != '\0') length++;
	name = new char[length+1];
	unsigned int runner = MAX_NAME_LENGTH * sizeof(char);
    for (unsigned int i = 0; i < runner; i++) {
		memcpy(name+i, packetString+counter+i, sizeof(char));
        if (packetString[counter+i] == '\0') break;
	}
    counter += runner;

    // Type (Carver/Boarder)
    memcpy(&avatarType, packetString+counter, sizeof(int));
    counter += sizeof(int);

    // Colors:

    // Hair Color
	memcpy(&hair.r, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&hair.g, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&hair.b, packetString+counter, sizeof(float));
	counter += sizeof(float);
		
	// Skin Color
	memcpy(&skin.r, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&skin.g, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&skin.b, packetString+counter, sizeof(float));
	counter += sizeof(float);

	// Shirt Color
	memcpy(&shirt.r, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&shirt.g, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&shirt.b, packetString+counter, sizeof(float));
	counter += sizeof(float);

	// Gloves Color
	memcpy(&gloves.r, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&gloves.g, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&gloves.b, packetString+counter, sizeof(float));
	counter += sizeof(float);

	// Trousers Color
	memcpy(&trousers.r, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&trousers.g, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&trousers.b, packetString+counter, sizeof(float));
	counter += sizeof(float);

	// Boots Color
	memcpy(&boots.r, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&boots.g, packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&boots.b, packetString+counter, sizeof(float));
	counter += sizeof(float);
}
