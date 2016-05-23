#include "StdAfx.h"
#include <Strsafe.h>
#include "bgMaterial.h"
#include "Object.h"
#include "ObjLoader.h"


ObjLoader::ObjLoader()
{

}


ObjLoader::~ObjLoader()
{

}

cObject* ObjLoader::LoadOBJFromFile(const char* FileName)
{
	if(FileName == 0)
		return NULL;

	std::vector<glm::vec3>  VertexList;
	std::vector<glm::vec2>  TexCoordList;
	std::vector<glm::vec3>  NormalList;
	std::vector<bgMaterial>  MatrialList;

    char Command[256] = {0};
    ifstream InFile(FileName);
	if(!InFile){ 
		return NULL;
	}

	char MaterialFilename[MAX_PATH] = {0};
	StringCchCopy(MediaPath, MAX_PATH -1, FileName);	
	if(char* pch = _tcsrchr(MediaPath, _T('\\'))){
		 StringCchCopy(MediaPath, pch+2 - MediaPath, MediaPath);
	}
	
	cGeometryObject *pOBJModel = new cGeometryObject();
	cModel* pModel = new cModel;
	pOBJModel->ModelList.push_back(pModel);

	cStringHelper StringHelper;
	size_t NumFace  = 0;
	char TitleName[MAX_PATH] = {0};
	cFileHelper FileHelper;
	FileHelper.GetFileNameTitle(FileName,TitleName, MAX_PATH -1);
	pOBJModel->Name = TitleName ; //StringHelper.charToString(TitleName);
	
	for(;;)
    {
        InFile >> Command;
        if( !InFile )
            break;
        
        if( 0 == strcmp(Command, "#"))
        {
            // Comment
        }
        else if(0 == strcmp(Command, "v"))
        {
            // Vertex Position
            float x, y, z;
            InFile >> x >> y >> z;
			VertexList.push_back(glm::vec3(x, y, z)); 			
        }
        else if(0 == strcmp(Command, "vt"))
        {
            // Vertex TexCoord
            float u, v;
            InFile >> u >> v;
			TexCoordList.push_back(glm::vec2(u,v));
        }
        else if( 0 == strcmp(Command, "vn"))
        {
            // Vertex Normal
            float x, y, z;
            InFile >> x >> y >> z;
			NormalList.push_back(glm::vec3(x,y,-z));
        }
        else if( 0 == strcmp(Command, "f"))
        {
			//cVertex vertex;
			bool InvalidData = false;
			char Buffer[256] = {0, };
			memset(Buffer, 0, sizeof(char)*256);
			InFile.getline(Buffer,100);
			std::string Data(Buffer);			
			//Data.append(StringHelper.charToString(Buffer));
			
			std::vector<std::string> Tokens = StringHelper.GetTokens(Data, std::string(" "));
			NumFace = Tokens.size();
			
            for(UINT iFace=0; iFace<NumFace; iFace++)
            {
				std::vector<std::string> Tokens = StringHelper.GetTokens(Data, std::string(" "));
				for(size_t iVertex =0; iVertex < Tokens.size(); iVertex++)
				{
					cVertex vertex;
					std::string vData = Tokens[iVertex];
				
					std::vector<std::string> Properties = StringHelper.GetTokens(vData, std::string("/"));
					for(size_t iOrder =0; iOrder< Properties.size(); iOrder++){
						
						std::string Value = Properties[iOrder];						
						int Index = atoi(Value.c_str());

						if(iOrder == 0){
							if((size_t)Index <= VertexList.size())
								vertex.Position = VertexList[Index-1];
							else
								vertex.Position = VertexList[0];							
						}
						else if(iOrder == 1){
							if((size_t)Index <= TexCoordList.size())
								vertex.Texel = TexCoordList[Index-1];						
						}
						else if(iOrder == 2){
							if((size_t)Index <= NormalList.size())
								vertex.Normal = NormalList[Index-1];
						}
					
					}
					pModel->VertexList.push_back(vertex);		
				}
			}
        }
        else if(0==strcmp(Command, "mtllib"))
        {
            // Material library
            InFile >> MaterialFilename;
        }
	        else if(0==strcmp(Command,"usemtl"))
        {
            // Material         
        }
        else
        {
            // Unimplemented or unrecognized command
			// 잘못된 한줄을 무시하기
		//	char Buffer[200];
		//	InFile.getline(Buffer, sizeof(Buffer));
        }

        //InFile.ignore( 1000, '\n' );
    }

    // Cleanup
    InFile.close();

	if(NumFace == 3)
		pModel->DrawMode = GL_TRIANGLES;
	else
		pModel->DrawMode = GL_TRIANGLE_STRIP; 

	if(MaterialFilename[0]){
		LoadMaterialsFromMTL(pModel, MaterialFilename);		
	}

	glm::vec4 AmbientColor = pModel->Material.Ambient;
	if(AmbientColor.r == 0.0f &&AmbientColor.g == 0.0f && AmbientColor.b == 0.0f){
		pModel->Material.Ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		pModel->Material.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pModel->Material.Emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}


	return pOBJModel;
}


bool ObjLoader::LoadMaterialsFromMTL(cModel* pMeshObj, const char* FileName)
{
	char MediaPath[MAX_PATH];
	char OldDir[MAX_PATH] = {0};
    GetCurrentDirectory(MAX_PATH, OldDir);
    SetCurrentDirectory(MediaPath);

	char Command[256] = {0};

	char FilePath[MAX_PATH]  = {0};
	StringCchCopy(FilePath, MAX_PATH, MediaPath);
	_tcscat(FilePath, FileName);

    ifstream InFile(FilePath);
    if(!InFile)
		return false;
    	
    SetCurrentDirectory(OldDir);
    
    while(true)
    {
        InFile >> Command;
        if(!InFile)
            break;
        
        if(0 == strcmp(Command, "newmtl"))
        {
        
        }
        
        if(0 == strcmp(Command, "#"))
        {
            // Comment
        }
        else if(0 == strcmp(Command, "Ka"))
        {
            // Ambient color
            float r, g, b;
            InFile >> r >> g >> b;
			pMeshObj->Material.Ambient = glm::vec4(r, g, b, 1);
        }
        else if(0==strcmp(Command, "Kd"))
        {
            // Diffuse color
            float r, g, b;
            InFile >> r >> g >> b;
			pMeshObj->Material.Diffuse = glm::vec4(r, g, b, 1);
        }
        else if(0 == strcmp(Command, "Ks"))
        {
            // Specular color
            float r, g, b;
            InFile >> r >> g >> b;
			pMeshObj->Material.Specular = glm::vec4(r, g, b, 1); 
            
        }
        else if(0 == strcmp(Command, "d" ) ||
                0 == strcmp(Command, "Tr"))
        {
            // Alpha
            //InFile >> 
        }
        else if(0 == strcmp(Command, "Ns"))
        {
            // Shininess
            int Shininess;
            InFile >> Shininess;
			pMeshObj->Material.Power = (float)Shininess;
        }
        else if(0 == strcmp(Command, "illum"))
        {
            // Specular on/off
            int illumination;
            InFile >> illumination;
        }
        else if(0 == strcmp(Command, "map_Kd"))
        {
			char TextureFileName[MAX_PATH];
            InFile >> TextureFileName;	
			pMeshObj->Material.LoadTexture(TextureFileName);
			
        }        
        else
        {
            // Unimplemented or unrecognized command
			
        }
       
        InFile.ignore(1000, '\n');
    }

    InFile.close();

	return true;
}