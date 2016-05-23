#pragma once

class cModel;
class cObject;
class cGeometryObject;
class ObjLoader
{
public:
	enum eFaceType{
		FACE_TYPE_VERTEX,
		FACE_TYPE_VERTEX_TEXTURE,
		FACE_TYPE_VERTEX_TEXTURE_NORMAL,
		FACE_TYPE_VERTEX_NORMAL,
	};
	cObject* LoadOBJFromFile(const char* FileName);

	ObjLoader();
	~ObjLoader();
private:
	TCHAR MediaPath[MAX_PATH];
    bool   LoadMaterialsFromMTL(cModel* pMeshObj, const char* FileName);

};

