
#include "Matterhorn.h"
#include "data_big.h"

Matterhorn::Matterhorn(void)
{
	createHorn();
}

Matterhorn::Matterhorn(unsigned int* newQuadRenderList, unsigned int* newQuadRenderNumber)
{
	quadRenderList = newQuadRenderList;
	quadRenderNumber = newQuadRenderNumber;
	createHorn();		
	generateQuads();
}

Matterhorn::~Matterhorn(void)
{
	int idx;
	while(idx = (int)textureArray.size()) {
		delete textureArray[idx - 1];
		textureArray.pop_back();
	}

	glDeleteLists(dlIndex, 1);
}

void Matterhorn::createHorn()
{
	Point3D* horn = new Point3D[(M_LENGTH+1)*(M_LENGTH+1)];
	Vector3D pos, norm, center;
	Vector3D normHelp1, normHelp2;
	Color col, colSnow, colRock, colGrass;
	float height;

	colSnow  = Color(1.0f, 1.0f, 1.0f);
	colRock  = Color(0.7f, 0.7f, 0.7f);
	colGrass = Color(0.2f, 1.0f, 0.2f);
	center = Vector3D(617050.0f, 4478.0f, 91675.0f);

	// max: (618000/4478  /92700)
	// min: (616000/3036.8/90700)
	// "center": (617050.0f, 4478.0f, 91675.0f)

	// loading coordinates into m_points
	for(int i = 0; i < (M_LENGTH+1); i++) {
		for(int j = 0; j < (M_LENGTH+1); j++) {
			// Position
			height = m_data[i][j][2];
			pos = Vector3D(m_data[i][j][0], height, m_data[i][j][1]);
			pos = pos - center;

			// Color
			if(height >= MATTERHORN_SNOW_BORDER)
				col = colSnow;
			else if(height >= MATTERHORN_ROCK_BORDER)
				col = colRock;
			else
				col = colGrass;

			// Normal
			if(i > 0 && i < M_LENGTH) {
				if(j > 0 && j < M_LENGTH) {
					// Mitten im Feld
					normHelp1 = Vector3D(m_data[i+1][j][0] - m_data[i-1][j][0],
						m_data[i+1][j][2] - m_data[i-1][j][2],
						m_data[i+1][j][1] - m_data[i-1][j][1]);
					normHelp2 = Vector3D(m_data[i][j+1][0] - m_data[i][j-1][0],
						m_data[i][j+1][2] - m_data[i][j-1][2],
						m_data[i][j+1][1] - m_data[i][j-1][1]);
					norm = Vector3D(normHelp1, normHelp2);
				}
				else {
					// am Rand von j
					norm = Vector3D(0.0f, 0.0f, 0.0f);
				}
			}
			else {
				// am Rand von i
				norm = Vector3D(0.0f, 0.0f, 0.0f);
			}
			norm.normalize();

			horn[i*(M_LENGTH+1) + j] = Point3D(pos, norm, col);
		}
	}
	horn[0].setNormal(Vector3D(0.0f, 1.0f, 0.0f));	// dass die erste Reihe für OGL auch Normalen hat

#ifdef _DEBUG
	horn[0].setColor(Color(1.0f, 0.0f, 0.0f));
	horn[1].setColor(Color(1.0f, 1.0f, 0.0f));
#endif

	setAllPoints(horn, (M_LENGTH+1)*(M_LENGTH+1));

	delete [] horn;

	// loading textures into textureArray
	loadTexture(TEXTURE_GRASS_FILE);	// index:	0
	loadTexture(TEXTURE_ROCK_FILE);		//			1
	loadTexture(TEXTURE_SNOW_FILE);		//			2

	// prepare and precompile OpenGL display list
	generateDisplayList();
}

void Matterhorn::generateDisplayList() {
	if(glIsList(dlIndex))
		glDeleteLists(dlIndex, 1);
	else {
		dlIndex = glGenLists(1);
		if(dlIndex == 0) {
#ifdef _DEBUG
			std::cout << "\n\t!!!!!\nKrass, keine DL's mehr frei?!?\n\t!!!!!\n\n";
#endif
			return;
		}
	}

	glNewList(dlIndex, GL_COMPILE);
		int texId, oldTexId;
		Color col;

		oldTexId = -1;
		for(unsigned int z = 0; z < M_LENGTH; z++) {
			glBegin(GL_TRIANGLE_STRIP);
			for(unsigned int x = 0; x < M_LENGTH; x++) {
				// preparing for textures
				col = m_points[x + (z+1)*(M_LENGTH+1)].getColor();
				if(col == Color(1.0f, 1.0f, 1.0f))
					texId = 2;	// snow
				else if(col == Color(0.7f, 0.7f, 0.7f))
					texId = 1;	// rock
				else
					texId = 0;	// grass
				if(texId != oldTexId) {
					//activate other texture
					glEnd();					// must deactivate render-mode to change texture,
					activateTexture(texId);		// but keep it as long as possible for performance reasons
					oldTexId = texId;
					glBegin(GL_TRIANGLE_STRIP);
					if(x > 0) {		// repaint last two vertices
						glTexCoord2f(((z % 5)+1) / 4.0f, ((x-1) % 5) / 4.0f);
						m_points[x + (z+1)*(M_LENGTH+1)].vertexNormalColor();
						glTexCoord2f((z % 5) / 4.0f, ((x-1) % 5) / 4.0f);
						m_points[x + z*(M_LENGTH+1)].vertexNormalColor();
					}
				}

				// now draw the points
				glTexCoord2f(1.0f, (float)(x % 2));
				m_points[x + (z+1)*(M_LENGTH+1)].vertexNormalColor();
				glTexCoord2f(0.0f, (float)(x % 2));
				m_points[x + z*(M_LENGTH+1)].vertexNormalColor();
			}
			glEnd();
		}

	glEndList();
}

void Matterhorn::generateQuads()
{
	int counter = 0;
	int tempDLindex = 0;

	for(unsigned int z = 0; z < M_LENGTH; z++) {
		for(unsigned int x = 0; x < M_LENGTH; x++) {
			if(glIsList(tempDLindex)) glDeleteLists(tempDLindex, 1);
			else {
				tempDLindex = glGenLists(1);
				if(tempDLindex == 0) {
                    #ifdef _DEBUG
						std::cout << "\n\t!!!!!\nKrass, keine DL's mehr frei?!?\n\t!!!!!\n\n";
                    #endif
                    return;
				}
			}

			// preparing for textures
			int texId = 0;
			Color col = m_points[x+1 + (z+1)*(M_LENGTH+1)].getColor();
			if(col == Color(1.0f, 1.0f, 1.0f))
				texId = 2;	// snow
			else if(col == Color(0.7f, 0.7f, 0.7f))
				texId = 1;	// rock
			else
				texId = 0;	// grass

			// generating DL
			glNewList(tempDLindex, GL_COMPILE);
				activateTexture(texId);

				glBegin(GL_TRIANGLE_STRIP);
				//glTexCoord2f(((z % 5)+1) / 4.0f, (x % 5) / 4.0f);
				glTexCoord2f(0.0f, 0.0f);
				m_points[x + (z+1)*(M_LENGTH+1)].vertexNormalColor();
				//glTexCoord2f((z % 5) / 4.0f, (x % 5) / 4.0f);
				glTexCoord2f(0.0f, 1.0f);
				m_points[x + z*(M_LENGTH+1)].vertexNormalColor();

				//glTexCoord2f((z % 5) / 4.0f, ((x % 5)+1) / 4.0f);
				glTexCoord2f(1.0f, 0.0f);
				m_points[(x+1) + (z+1)*(M_LENGTH+1)].vertexNormalColor();
				//glTexCoord2f(((z % 5)+1) / 4.0f, ((x % 5)+1) / 4.0f);
				glTexCoord2f(1.0f, 1.0f);
				m_points[(x+1) + z*(M_LENGTH+1)].vertexNormalColor();
				glEnd();

			glEndList();

			quads[counter] = hQuad(m_points[x + (z+1)*(M_LENGTH+1)], m_points[x + z*(M_LENGTH+1)], m_points[(x+1) + z*(M_LENGTH+1)], m_points[(x+1) + (z+1)*(M_LENGTH+1)], tempDLindex);
			tempDLindex = 0;
			counter++;
		}
	}
}

Vector3D* Matterhorn::getCenter() const {
	unsigned int idx = (int)((M_LENGTH+1) / 2);
	return new Vector3D(m_points[idx].getPosition().x, m_points[idx].getPosition().y, m_points[idx].getPosition().z);
}

bool Matterhorn::loadTexture(const char* fileName, int index) {
	if((unsigned int)index > textureArray.size() || index < 0) {
		// add new item
		CAMP::Bitmap* temp = new CAMP::Bitmap();
		temp->load(fileName);
		textureArray.push_back(temp);
	}
	else {
		// overwrite old item
		textureArray[index]->load(fileName);
	}
	return true;
}

void Matterhorn::renderWithQuads(Vector3D player)
{
	clippingTest(player);
	for (unsigned int i = 0; i < *quadRenderNumber; i++) {
		quads[quadRenderList[i]].render(i);
	}
}

void Matterhorn::clippingTest(Vector3D player)
{
	unsigned int startQuad = findQuadForPoint(player);
	calcViewVolumePlanes();
	
	// if quadRenderNumber == 0 then currently 0 quads are visisble -> sync-loss
	if (*quadRenderNumber == 0) {
		quadRenderList[0] = startQuad;
		*quadRenderNumber = 1;
	}

	unsigned int *neighbours = getNeighbours(startQuad);
	for (unsigned int k = 0; k < 8; k++) {
		bool found = false;
		for (unsigned int j = 0; j < *quadRenderNumber; j++) {
			if (neighbours[k] == quadRenderList[j]) found = true;
		}
		if (!found) {
			quadRenderList[*quadRenderNumber] = neighbours[k];
			*quadRenderNumber = *quadRenderNumber + 1;
		}
	}
	delete neighbours;

	// checking for neighbours till in one complete round no new neightbour comes visible
	bool change = true;
	std::vector<unsigned int> alreadyTested;
	while (change) {
		change = false;
		bool kill = false;
		for(unsigned int i = 0; i < *quadRenderNumber; i++) {
			if (kill == true) break;
			// checking neighbours for current NOTCLIPPED quad
			unsigned int quadNum = quadRenderList[i];
			unsigned int *neighbours = getNeighbours(quadNum);
			for (unsigned int k = 0; k < 8; k++) {
                unsigned int toTest = neighbours[k];
				bool found = false;
				// test if already tested clipped
				for(unsigned int j = 0; j < alreadyTested.size(); j++) {
					if (alreadyTested[j] == toTest) {
						found = true;
						break;
					}
				}
				// test if part of quadRenderList
				if (found == false) {
                    for(unsigned int j = 0; j < *quadRenderNumber; j++) {
						if (quadRenderList[j] == toTest) {
							found = true;
							break;
						}
					}
				}
				// not yet tested: test if clipped or unclipped
				if (found == false) {
					quads[toTest].culled(&planeEqs);
					// adding to alreadyTested array
					alreadyTested.push_back(toTest);
				}
				// now knowing if neightbour is clipped
				// if not clipped -> added to renderlist
				if ((found == false) && (quads[toTest].clipped == 0)) {
					change = true;
					quadRenderList[*quadRenderNumber] = toTest;
					*quadRenderNumber = *quadRenderNumber + 1;
					if(*quadRenderNumber > 100) {
						//std::cerr << "Fehler: Quad-Update Ueberlauf: " << *quadRenderNumber << "\n";
						//for (unsigned int m = 0; m < *quadRenderNumber; m++) {
						//	std::cerr << quadRenderList[m] << " -------------------\n";
						//	quads[quadRenderList[m]].culledp(&planeEqs);
						//	std::cerr << " CL: " << quads[quadRenderList[m]].clipped << std::endl;
						//	std::cerr << "---------------------------------------------\n";
						//}
						//std::cerr << "---------------------------------------------\n";
						//change = false;
						//kill = true;
						break;
					}
				}
			}
			delete neighbours;
		}
	}
	// returned from while, so no changes anymore, that means:
	// no more neighbours found anymore, that are not clipped

	// check all quads if they became clipped
	for(unsigned int i = 0; i < *quadRenderNumber; i++) {
		quads[quadRenderList[i]].culled(&planeEqs);
		if ((quads[quadRenderList[i]].clipped == 1) && (quadRenderList[i] != startQuad)) {
			quadRenderList[i] = quadRenderList[*quadRenderNumber - 1];
			*quadRenderNumber = *quadRenderNumber - 1;
			i--;
		}
	}
	
	neighbours = getNeighbours(startQuad);
	for (unsigned int k = 0; k < 8; k++) {
		bool found = false;
		for (unsigned int j = 0; j < *quadRenderNumber; j++) {
			if (neighbours[k] == quadRenderList[j]) found = true;
		}
		if (!found) {
			quadRenderList[*quadRenderNumber] = neighbours[k];
			*quadRenderNumber = *quadRenderNumber + 1;
		}
	}
	delete neighbours;

	//std::vector<unsigned int> addOutsiders;
	//for (unsigned int i = 0; i < *quadRenderNumber; i++) {
	//	neighbours = getNeighbours(quadRenderList[i]);
	//	for (unsigned int j = 0; j < 8; j++) {
	//		bool found = false;
	//		for (unsigned int k = 0; k < addOutsiders.size(); k++) {
	//			if (neighbours[j] == addOutsiders[k]) {
	//				found = true;
	//				break;
	//			}
	//		}
	//		if (found == false) {
	//			for (unsigned int l = 0; l < *quadRenderNumber; l++) {
	//				if (neighbours[j] == quadRenderList[l]) {
	//					found = true;
	//					break;
	//				}
	//			}
	//		}
	//		// if found is true, dont add result
	//		if (found == false) {
	//			addOutsiders.push_back(neighbours[j]);
	//		}
	//	}
	//	delete neighbours;
	//}
	//// now all invisible quads in 1 row outside of visible ones are in addOutsiders
	//while (addOutsiders.size() > 0) {
	//	quadRenderList[*quadRenderNumber] = addOutsiders[addOutsiders.size() - 1];
	//	*quadRenderNumber = *quadRenderNumber + 1;
	//	addOutsiders.pop_back();
	//}
}

#ifdef _DEBUG
void Matterhorn::printQuadsClipped()
{
	int clipped = 0;
	for (int i = 0; i < QUADS_NUM; i++)
		if (quads[i].clipped == 1) clipped++;
	std::cout << "Quads visible: " << QUADS_NUM - clipped << " // clipped: " << clipped << std::endl;
}
#endif

float Matterhorn::getHeightOverGround(Vector3D checkPoint)
{
	return (checkPoint.y - quads[findQuadForPoint(checkPoint)].getHeightOfPoint(checkPoint.x, checkPoint.z));
}

Vector3D Matterhorn::getNormal(Vector3D checkPoint)
{
	if ((abs(checkPoint.x) < 0.001f) && (abs(checkPoint.y) < 0.001f) && (abs(checkPoint.z) < 0.001f)) {
		return Vector3D(0.0f, 0.0f, 0.0f);
	}
	return quads[findQuadForPoint(checkPoint)].getNormal(checkPoint);
}

unsigned int Matterhorn::findQuadForPoint(Vector3D checkPoint)
{
	// minX = -1050; maxX =  950; => dis = 2000 (25 per quad)
	// minZ =  -975; maxZ = 1025; => dis = 2000 (25 per quad)
	// calc: get x and z quad, then merge
	int xQuad = (int)((checkPoint.x + 1050.0f) / 25.0f);
	int zQuad = (int)(-(checkPoint.z - 1025.0f) / 25.0f);
	// checking if out of range
	if (xQuad < 0) xQuad = 0;
	if (xQuad >= M_LENGTH) xQuad = M_LENGTH - 1;
	if (zQuad < 0) zQuad = 0;
	if (zQuad >= M_LENGTH) zQuad = M_LENGTH - 1;
	// merging to fit array
	int merge = (xQuad + (zQuad * M_LENGTH));
	return (unsigned int)merge;
}

void Matterhorn::activateTexture(int idx) const {
	if((unsigned int)idx < textureArray.size()) {
		int texWidth, texHeight;
		texWidth  = textureArray[idx]->getWidth();
		texHeight = textureArray[idx]->getHeight();
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureArray[idx]->getData());
	}
}

void Matterhorn::getCornerPoints(Vector3D *returnPosA, Vector3D *returnPosB, Vector3D *returnPosC, Vector3D *returnPosD) const {
	*returnPosA = m_points[0].getPosition();
	*returnPosB = m_points[M_LENGTH].getPosition();
	*returnPosC = m_points[(M_LENGTH+1)*(M_LENGTH+1) - 1].getPosition();
	*returnPosD = m_points[(M_LENGTH+1)*M_LENGTH].getPosition();
}

void Matterhorn::moveToCoords(Vector3D newPos) {
	Vector3D change = m_points[(int)(((M_LENGTH+1)*(M_LENGTH+1) - 1) / 2)].getPosition();
	change = change - newPos;
	this->move(change.x, change.y, change.z);
}

unsigned int Matterhorn::neighbourCorrect(int current)
{
	if (current >= QUADS_NUM) current -= QUADS_NUM;
	if (current < 0) current += QUADS_NUM;
	return (unsigned int)current;
}

unsigned int* Matterhorn::getNeighbours(unsigned int current)
{
	unsigned int *erg = new unsigned int[8];
	erg[0] = neighbourCorrect((int)current + 1);
	erg[1] = neighbourCorrect((int)current - 1);
	erg[2] = neighbourCorrect((int)current + 79);
	erg[3] = neighbourCorrect((int)current + 80);
	erg[4] = neighbourCorrect((int)current + 81);
	erg[5] = neighbourCorrect((int)current - 79);
	erg[6] = neighbourCorrect((int)current - 80);
	erg[7] = neighbourCorrect((int)current - 81);
	return erg;
}

void Matterhorn::calcViewVolumePlanes()
{
    GLfloat clip[16], modl[16], proj[16];

    /* Get the modelview and projection matrices */
    glGetFloatv(GL_MODELVIEW_MATRIX, modl);
    glGetFloatv(GL_PROJECTION_MATRIX, proj);

	//modl[12] = 0.0f;
	//modl[13] = 0.0f;
	//modl[14] = 0.0f;
	//modl[15] = 1.0f;

    clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

	//std::cerr << "Clip" << std::endl;
 //   for (int i = 0; i < 16; i++) {
	//	std::cerr << i << ": " << clip[i] << " -- ";
	//}
	//std::cerr << std::endl << "modl" << std::endl;
	//for (int i = 0; i < 16; i++) {
	//	std::cerr << i << ": " << modl[i] << " -- ";
	//}
	//std::cerr << std::endl << "proj" << std::endl;
	//for (int i = 0; i < 16; i++) {
	//	std::cerr << i << ": " << proj[i] << " -- ";
	//}
	//std::cerr << std::endl;
	//std::cerr << std::endl;
	//std::cerr << std::endl;

	// ind: modl[12], modl[13], modl[14], modl[15]

	// right
    planeEqs[0][0] = clip[ 3] - clip[ 0];
    planeEqs[0][1] = clip[ 7] - clip[ 4];
    planeEqs[0][2] = clip[11] - clip[ 8];
    planeEqs[0][3] = clip[15] - clip[12];

	// left
    planeEqs[1][0] = clip[ 3] + clip[ 0]; 
    planeEqs[1][1] = clip[ 7] + clip[ 4]; 
    planeEqs[1][2] = clip[11] + clip[ 8]; 
    planeEqs[1][3] = clip[15] + clip[12]; 

	// bottom
    planeEqs[2][0] = clip[ 3] + clip[ 1]; 
    planeEqs[2][1] = clip[ 7] + clip[ 5]; 
    planeEqs[2][2] = clip[11] + clip[ 9]; 
    planeEqs[2][3] = clip[15] + clip[13]; 

	// top
    planeEqs[3][0] = clip[ 3] - clip[ 1]; 
    planeEqs[3][1] = clip[ 7] - clip[ 5]; 
    planeEqs[3][2] = clip[11] - clip[ 9]; 
    planeEqs[3][3] = clip[15] - clip[13]; 

	// front
    planeEqs[4][0] = clip[ 3] + clip[ 2]; 
    planeEqs[4][1] = clip[ 7] + clip[ 6]; 
    planeEqs[4][2] = clip[11] + clip[10]; 
    planeEqs[4][3] = clip[15] + clip[14]; 

	// back
    planeEqs[5][0] = clip[ 3] - clip[ 2]; 
    planeEqs[5][1] = clip[ 7] - clip[ 6]; 
    planeEqs[5][2] = clip[11] - clip[10]; 
    planeEqs[5][3] = clip[15] - clip[14]; 
}
