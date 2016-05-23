#include "StdAfx.h"
#include "Shader.h"
#include "Object.h"
#include "bgActor.h"
#include "bgProperty.h"
#include "Action.h"
#include "PropertiesWnd.h"
#include "bgGizmo.h"
#include "Scene.h"
#include "bgMovie.h"
#include "bgUtil.h"
#include <string>
using namespace std;

/////////////////////////////////////////////////////////////
//cAction
cAction::cAction()
{
	pObject = 0;
	pScene = 0;
	pMovie = 0;
}
cAction::~cAction()
{

}

/////////////////////////////////////////////////////////////////
//cObjectTreeAction
cObjectTreeAction::cObjectTreeAction(CTreeCtrl* _pObjectTree)
{
	pObjectTree= _pObjectTree;
	HTREEITEM treeItem = pObjectTree->InsertItem("background", 0, 0);
	treeItem = pObjectTree->InsertItem("renderer", 0, 0);
	treeItem = pObjectTree->InsertItem("video", 0, 0);
	
}
cObjectTreeAction::~cObjectTreeAction()
{

}
void cObjectTreeAction::Insert(bgActor* pObject)
{
	ASSERT(pObjectTree);
	ASSERT(pObject);

	std::string Name = pObject->name;
	HTREEITEM treeItem = pObjectTree->InsertItem(Name.c_str(), 0, 0);
	
	if (bgMaterial* pMaterial = pObject->getMaterial()){
		pObjectTree->InsertItem(_T("Material"), treeItem);
	}

	if (bgMapper* pMapper = pObject->getInput()){
		HTREEITEM hItem = pObjectTree->InsertItem(_T("Mapper"), treeItem);
		if (bgData* pData = pMapper->getInput()){
			pObjectTree->InsertItem(_T("Data"), hItem);
		}
	}

	TreeActorItem treeActorItem;
	treeActorItem.hItem = treeItem;
	treeActorItem.pActor = pObject;
	TreeItemList.push_back(treeActorItem);

	SortItem(TVI_ROOT);
}

void cObjectTreeAction::InsertVideoItem(const std::string& name)
{
	HTREEITEM hVideoItem = FindItem("video");
	ASSERT(hVideoItem);

	HTREEITEM hItem = pObjectTree->InsertItem(name.c_str(), hVideoItem);
	TreeActorItem treeActorItem;
	treeActorItem.hItem = hItem;
	TreeItemList.push_back(treeActorItem);

	SortItem(TVI_ROOT);
}

HTREEITEM cObjectTreeAction::FindItem(const std::string& name, HTREEITEM hRoot)
{
	std::string text = pObjectTree->GetItemText(hRoot);
	if (text.compare(name) == 0)
		return hRoot;

	HTREEITEM hSub = pObjectTree->GetChildItem(hRoot);
	while (hSub)
	{
		HTREEITEM hFound = FindItem(name, hSub);
		if (hFound)
			return hFound;

		hSub = pObjectTree->GetNextSiblingItem(hSub);
	}

	return NULL;
}

HTREEITEM cObjectTreeAction::FindItem(const std::string& name)
{
	HTREEITEM root = pObjectTree->GetRootItem();
	while (root)
	{
		if (HTREEITEM hitem = FindItem(name, root))
			return hitem;

		root = pObjectTree->GetNextSiblingItem(root);
	}

	return NULL;
}

void cObjectTreeAction::InitializeUI()
{
	CString VideoPath = CString("..\\Media\\Video\\*.*");

	CFileFind videoFind;
	BOOL bFinded = videoFind.FindFile(VideoPath);
	int index = 0;

	while (bFinded)
	{
		bFinded = videoFind.FindNextFile();
		if (videoFind.IsDirectory())
			continue;
		CString name = videoFind.GetFileName();
		if (name == _T(".") || name == _T(".."))
			continue;

		std::string extension = FindExtension(name.GetBuffer(0));
		if (extension.compare("avi") != 0)
			continue;

		CString title = videoFind.GetFileTitle();
		CString path = videoFind.GetFilePath();
		title.Append(".");
		title.Append(extension.c_str());

		InsertVideoItem(title.GetBuffer(MAX_PATH));
	}
	
}

void cObjectTreeAction::SortItem(HTREEITEM item)
{
	if (item != NULL)
	{
		if (item == TVI_ROOT || pObjectTree->ItemHasChildren(item))
		{
			HTREEITEM child = pObjectTree->GetChildItem(item);

			while (child != NULL)
			{
				SortItem(child);
				child = pObjectTree->GetNextItem(child, TVGN_NEXT);
			}

			pObjectTree->SortChildren(item);
		}
	}
}

//////////////////////////////////////////////////////////////////
//cShaderTreeAction
cSceneTreeAction::cSceneTreeAction(CTreeCtrl* _pShaderTree)
{
	pShaderTree= _pShaderTree;
}
cSceneTreeAction::~cSceneTreeAction()
{

}
void cSceneTreeAction::Insert(bgActor* _pObject)
{

}
void cSceneTreeAction::InitializeUI()
{	
	CString TexturePath = CString("..\\Media\\Video\\*.*");

	
}

//////////////////////////////////////////////////////////////////
//cDataPropertyAction
cDataPropertyAction::cDataPropertyAction(CPropertiesWnd* _pPropertyGrid)
{
	pPropertyGrid = _pPropertyGrid;
	pPhotoEffect = 0;
	pMovieEffect = 0;
}
cDataPropertyAction::~cDataPropertyAction()
{
	
}
void cDataPropertyAction::Insert(bgActor* _pObject)
{
	CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;
/*
	cGeometryObject* pGeoObject = dynamic_cast<cGeometryObject*>(_pObject);
	if(pGeoObject == 0)
		return;

	pObject = pGeoObject;
	glm::vec3 Translation =	 pGeoObject->Translation;
	glm::vec3 Rotation	 =	 pGeoObject->Rotation;
	glm::vec3 Scale		 =	 pGeoObject->Scale;

	PropertyGrid.RemoveAll();

	CMFCPropertyGridProperty* pTransformGroup = new CMFCPropertyGridProperty(STRING_OBJECT_TRANS);
		
		CMFCPropertyGridProperty* pTranslationGroup = new CMFCPropertyGridProperty(STRING_OBJECT_TRANSLATION);
			CMFCPropertyGridProperty* pTranslationX = new CMFCPropertyGridProperty(_T("x"), (_variant_t)Translation.x, _T("Translation X"));
			pTranslationX->SetData((int)TRANSFORM_TRANSLATION_X);
			pTranslationGroup->AddSubItem(pTranslationX);
			CMFCPropertyGridProperty* pTranslationY = new CMFCPropertyGridProperty(_T("y"), (_variant_t)Translation.y, _T("Translation Y"));
			pTranslationY->SetData((int)TRANSFORM_TRANSLATION_Y);
			pTranslationGroup->AddSubItem(pTranslationY);
			CMFCPropertyGridProperty* pTranslationZ = new CMFCPropertyGridProperty(_T("z"), (_variant_t)Translation.z, _T("Translation Z"));
			pTranslationZ->SetData((int)TRANSFORM_TRANSLATION_Z);
			pTranslationGroup->AddSubItem(pTranslationZ);
			pTransformGroup->AddSubItem(pTranslationGroup);


		CMFCPropertyGridProperty* pRotationGroup = new CMFCPropertyGridProperty(STRING_OBJECT_ROTATION);
			CMFCPropertyGridProperty* pRotationX = new CMFCPropertyGridProperty(_T("x"), (_variant_t)Rotation.x, _T("Rotation X"));
			pRotationX->SetData((int)TRANSFORM_ROTATION_X);
			pRotationGroup->AddSubItem(pRotationX);
			CMFCPropertyGridProperty* pRotationY = new CMFCPropertyGridProperty(_T("y"), (_variant_t)Rotation.y, _T("Rotation Y"));
			pRotationY->SetData((int)TRANSFORM_ROTATION_Y);
			pRotationGroup->AddSubItem(pRotationY);
			CMFCPropertyGridProperty* pRotationZ = new CMFCPropertyGridProperty(_T("z"), (_variant_t)Rotation.z, _T("Rotation Z"));
			pRotationZ->SetData((int)TRANSFORM_ROTATION_Z);
			pRotationGroup->AddSubItem(pRotationZ);
			pTransformGroup->AddSubItem(pRotationGroup);

		CMFCPropertyGridProperty* pScaleGroup = new CMFCPropertyGridProperty(STRING_OBJECT_SCALE);
			CMFCPropertyGridProperty* pScaleX = new CMFCPropertyGridProperty(_T("x"), (_variant_t)Scale.x, _T("Scale X"));
			pScaleX->SetData((int)TRANSFORM_SCALE_X);
			pScaleGroup->AddSubItem(pScaleX);
			CMFCPropertyGridProperty* pScaleY = new CMFCPropertyGridProperty(_T("y"), (_variant_t)Scale.y, _T("Scale Y"));
			pScaleY->SetData((int)TRANSFORM_SCALE_Y);
			pScaleGroup->AddSubItem(pScaleY);
			CMFCPropertyGridProperty* pScaleZ = new CMFCPropertyGridProperty(_T("z"), (_variant_t)Scale.z, _T("Scale Z"));
			pScaleZ->SetData((int)TRANSFORM_SCALE_Z);
			pScaleGroup->AddSubItem(pScaleZ);
			pTransformGroup->AddSubItem(pScaleGroup);

		PropertyGrid.AddProperty(pTransformGroup);	
*/
}
void cDataPropertyAction::Insert(bgProperty* _pProperty)
{
	CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;
	PropertyGrid.RemoveAll();

	std::string message;
	int value = std::stoi(_pProperty->messageMap["fill"]);
	eFillMode mode = eFillMode(value);
	if (mode == FILLMODE_WIREFRAME)
		message = std::string("Wire");
	else if (mode == FILLMODE_SOLID)
		message = std::string("Solid");

	CMFCPropertyGridProperty* pRenderOptionGroup = new CMFCPropertyGridProperty(STRING_SCENE_RENDERER);
		CMFCPropertyGridProperty* pFillMode = new CMFCPropertyGridProperty(_T("fill"), (_variant_t)message.c_str(), _T("Fill Mode"));
		pFillMode->AddOption("Wire");
		pFillMode->AddOption("Solid");
		pFillMode->SetData((int)SCENE_RENDERER_FILL);
		pRenderOptionGroup->AddSubItem(pFillMode);
		
		value = std::stoi(_pProperty->messageMap["shading"]);
		eShadingModel model = eShadingModel(value);
		if (mode == SHADING_MODEL_FLAT)
			message = std::string("Flat");
		else if (mode == SHADING_MODEL_SMOOTH)
			message = std::string("Smooth");
		
		CMFCPropertyGridProperty* pShadingMode = new CMFCPropertyGridProperty(_T("shading"), (_variant_t)message.c_str(), _T("Shading Mode"));
		pShadingMode->AddOption("Flat");
		pShadingMode->AddOption("Smooth");
		pShadingMode->SetData((int)SCENE_RENDERER_SHADING);
		pRenderOptionGroup->AddSubItem(pShadingMode);


	PropertyGrid.AddProperty(pRenderOptionGroup);

}
void cDataPropertyAction::Insert(cScene* _pScene)
{
	pScene = _pScene;
	CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;
	PropertyGrid.RemoveAll();
}
void cDataPropertyAction::Insert(bgGizmo* _pGizmo)
{
	CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;
	PropertyGrid.RemoveAll();
	bool enableMark = _pGizmo->enableMark;
	std::string message = enableMark == true ? std::string("on") : std::string("off");

	CMFCPropertyGridProperty* pCanvasOptionGroup = new CMFCPropertyGridProperty(STRING_CANVAS_NAME);
		CMFCPropertyGridProperty* pMarkMode = new CMFCPropertyGridProperty(_T("Mark"), (_variant_t)message.c_str(), _T("Enable or Disable Mark"));
		pMarkMode->AddOption("on");
		pMarkMode->AddOption("off");
		pMarkMode->SetData((int)GIZMO_CANVAS_ENABLE_MARK);
	pCanvasOptionGroup->AddSubItem(pMarkMode);

	bool enableGuideLine = _pGizmo->enableGuideLine;
	message = enableGuideLine == true ? std::string("on") : std::string("off");
		CMFCPropertyGridProperty* pGuideLineMode = new CMFCPropertyGridProperty(_T("GuideLine"), (_variant_t)message.c_str(), _T("Enable or Disable GuideLine"));
		pGuideLineMode->AddOption("on");
		pGuideLineMode->AddOption("off");
		pGuideLineMode->SetData((int)GIZMO_CANVAS_ENABLE_GUIDELINE);
	pCanvasOptionGroup->AddSubItem(pGuideLineMode);

	PropertyGrid.AddProperty(pCanvasOptionGroup);


}
void	cDataPropertyAction::Insert(bgCanvasActor* _pCanvasActor)
{
	CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;
	PropertyGrid.RemoveAll();

	eCanvasSize canvasSize = _pCanvasActor->canvasSize;
	std::string message = STRING_CANVAS_SIZE[(int)canvasSize];
	CMFCPropertyGridProperty* pCanvasOptionGroup = new CMFCPropertyGridProperty(STRING_CANVAS_NAME);
		CMFCPropertyGridProperty* pSizeMode = new CMFCPropertyGridProperty(_T("Size"), (_variant_t)message.c_str(), _T("Change Canvas Size"));

		for (int index = 0; index < (int)CANVAS_TYPE_NUM; index++)
			pSizeMode->AddOption(STRING_CANVAS_SIZE[index]);

		pSizeMode->SetData((int)WM_REQUEST_CANVAS_SIZE);
	pCanvasOptionGroup->AddSubItem(pSizeMode);

	eCanvasEffect canvasEffect = _pCanvasActor->canvasEffect;
	message = STRING_CANVAS_EFFECT;
	CMFCPropertyGridProperty* pEffectMode = new CMFCPropertyGridProperty(_T("Effect"), (_variant_t)message.c_str(), _T("Change Shader Effect"));

		pEffectMode->AddOption(message.c_str());
		pEffectMode->SetData((int)WM_REQUEST_CANVAS_EFFECT);
	pCanvasOptionGroup->AddSubItem(pEffectMode);

	eCanvasMorph canvasMorph = _pCanvasActor->canvasMorph;
	message = STRING_CANVAS_MORPH;
	CMFCPropertyGridProperty* pMorphMode = new CMFCPropertyGridProperty(_T("Morph"), (_variant_t)message.c_str(), _T("Change Morph&Warp Effect"));
		
		pMorphMode->AddOption(message.c_str());
		pMorphMode->SetData((int)WM_REQUEST_CANVAS_MORPH);
	pCanvasOptionGroup->AddSubItem(pMorphMode);


	PropertyGrid.AddProperty(pCanvasOptionGroup);

}

void cDataPropertyAction::InsertMovieEffect(int algorithmIndex)
{
	pMovieEffect = pMovie->GetMovieEffectAlgorithm(algorithmIndex);
	CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;
	PropertyGrid.RemoveAll();

	std::string message = std::to_string(pMovieEffect->BlurSizeX);
	CMFCPropertyGridProperty* pMovieEffectGroup = new CMFCPropertyGridProperty(STRING_MOVIE_EFFECT_NAME);
		CMFCPropertyGridProperty* pWidthSizeMode = new CMFCPropertyGridProperty(_T("Blur Width"), (_variant_t)message.c_str(), _T("Gaussian Blur X"));
		pWidthSizeMode->SetData((int)MOVIE_PROPERTY_BLUR_SIZE_X);
		pMovieEffectGroup->AddSubItem(pWidthSizeMode);

	message = std::to_string(pMovieEffect->BlurSizeY);
	CMFCPropertyGridProperty* pHeightSizeMode = new CMFCPropertyGridProperty(_T("Blur Height"), (_variant_t)message.c_str(), _T("Gaussian Blur Y"));
		pHeightSizeMode->SetData((int)MOVIE_PROPERTY_BLUR_SIZE_Y);
		pMovieEffectGroup->AddSubItem(pHeightSizeMode);
		
	PropertyGrid.AddProperty(pMovieEffectGroup);
}
void cDataPropertyAction::InsertPhotoEffect(int algorithmIndex)
{
	pPhotoEffect = NULL;
	bgPhotoEffectAlgorithm* pAlgo = pScene->GetPhotoEffect(algorithmIndex);
	if (pAlgo == NULL) return;
	pPhotoEffect = pAlgo;

	CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;
	PropertyGrid.RemoveAll();
	CMFCPropertyGridProperty* pPhotoEffectGroup = new CMFCPropertyGridProperty(STRING_PHOTO_EFFECT_NAME);

	if (pAlgo->GetEffectType() == PHOTO_EFFECT_ALPHA)
	{
		bgAlphaPhotoEffectAlgorithm* pAlpha = dynamic_cast<bgAlphaPhotoEffectAlgorithm*>(pAlgo);
		std::string message = std::to_string(pAlpha->GetAlpha());

		CMFCPropertyGridProperty* pAlphaMode = new CMFCPropertyGridProperty(_T("Alpha"), (_variant_t)message.c_str(), _T("Alpha(0.0~1.0)"));
		pAlphaMode->SetData((int)PHOTO_PROPERTY_ALPAH_VALUE);
		pPhotoEffectGroup->AddSubItem(pAlphaMode);
	}
	else if (pAlgo->GetEffectType() == PHOTO_EFFECT_BRIGHT_CONTRAST)
	{
		bgBrightContrastPhotoEffectAlgorithm* pBCAlgo = dynamic_cast<bgBrightContrastPhotoEffectAlgorithm*>(pAlgo);
		std::string message = std::to_string(pBCAlgo->GetBrightness());

		CMFCPropertyGridProperty* pBrightMode = new CMFCPropertyGridProperty(_T("Brightness"), (_variant_t)message.c_str(), _T("Brightness(0.0~1.0)"));
		pBrightMode->SetData((int)PHOTO_PROPERTY_BRIGHT_VALUE);
		pPhotoEffectGroup->AddSubItem(pBrightMode);

		message = std::to_string(pBCAlgo->GetContrast());
		CMFCPropertyGridProperty* pContrastMode = new CMFCPropertyGridProperty(_T("Contrast"), (_variant_t)message.c_str(), _T("Contrast(0.0~1.0)"));
		pContrastMode->SetData((int)PHOTO_PROPERTY_CONTRAST_VALUE);
		pPhotoEffectGroup->AddSubItem(pContrastMode);				
	}
	else if (pAlgo->GetEffectType() == PHOTO_EFFECT_GRAY)
	{
		bgGrayPhotoEffectAlgorithm* pGrayAlgo = dynamic_cast<bgGrayPhotoEffectAlgorithm*>(pAlgo);		
		int blendway = pGrayAlgo->GetBlendWay();
		CMFCPropertyGridProperty* pBlendingMode = new CMFCPropertyGridProperty(STRING_PHOTO_EFFECT_GRAY_NAME,(_variant_t)STRING_PHOTO_EFFECT_GRAY[blendway], STRING_PHOTO_EFFECT_GRAY_NAME);
		for (int index = 0; index < 6; index++)
			pBlendingMode->AddOption(STRING_PHOTO_EFFECT_GRAY[index]);

		pBlendingMode->SetData(pGrayAlgo->GetBlendWay() + (int)PHOTO_PROPERTY_GRAY_AVERAGE);
		pPhotoEffectGroup->AddSubItem(pBlendingMode);
	}


	PropertyGrid.AddProperty(pPhotoEffectGroup);

}
void cDataPropertyAction::DoFixedPipeProperty()
{
	if (pScene == 0)
		return;

	CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;
	/*
	cLight* pLight = pScene->LightList[0];
	glm::vec3 Position = pLight->Position;

	CMFCPropertyGridProperty* pLightGroup = new CMFCPropertyGridProperty(STRING_LIGHT_POSITON);
	CMFCPropertyGridProperty* pTranslationX = new CMFCPropertyGridProperty(_T("x"), (_variant_t)Position.x, _T("X"));
	pTranslationX->SetData((int)LIGHT_POSITION_X);
	pLightGroup->AddSubItem(pTranslationX);
	CMFCPropertyGridProperty* pTranslationY = new CMFCPropertyGridProperty(_T("y"), (_variant_t)Position.y, _T("Y"));
	pTranslationY->SetData((int)LIGHT_POSITION_Y);
	pLightGroup->AddSubItem(pTranslationY);
	CMFCPropertyGridProperty* pTranslationZ = new CMFCPropertyGridProperty(_T("z"), (_variant_t)Position.z, _T("Z"));
	pTranslationZ->SetData((int)LIGHT_POSITION_Z);
	pLightGroup->AddSubItem(pTranslationZ);

	PropertyGrid.AddProperty(pLightGroup);
	*/
}
void cDataPropertyAction::DoFixedPipeShadowMap()
{
	//CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;

	//cFixedShadowMapScene* pFixedShadowMap = dynamic_cast<cFixedShadowMapScene*>(pScene);
	//if(pFixedShadowMap == 0)
	//	return;

	//cLight* pLight = pFixedShadowMap->LightList[0];
	//glm::vec3 Position = pLight->Position;

	//CMFCPropertyGridProperty* pLightGroup = new CMFCPropertyGridProperty(STRING_LIGHT_POSITON);
	//	CMFCPropertyGridProperty* pTranslationX = new CMFCPropertyGridProperty(_T("x"), (_variant_t)Position.x, _T("X"));
	//	pTranslationX->SetData((int)LIGHT_POSITION_X);
	//	pLightGroup->AddSubItem(pTranslationX);
	//	CMFCPropertyGridProperty* pTranslationY = new CMFCPropertyGridProperty(_T("y"), (_variant_t)Position.y, _T("Y"));
	//	pTranslationY->SetData((int)LIGHT_POSITION_Y);
	//	pLightGroup->AddSubItem(pTranslationY);
	//	CMFCPropertyGridProperty* pTranslationZ = new CMFCPropertyGridProperty(_T("z"), (_variant_t)Position.z, _T("Z"));
	//	pTranslationZ->SetData((int)LIGHT_POSITION_Z);
	//	pLightGroup->AddSubItem(pTranslationZ);

	//PropertyGrid.AddProperty(pLightGroup);


	//CMFCPropertyGridProperty* pPolygonOffsetGroup = new CMFCPropertyGridProperty(STRING_SHADOW_POLYGON_OFFSET);
	//	CMFCPropertyGridProperty* pOffset = new CMFCPropertyGridProperty(_T("x"), (_variant_t) pFixedShadowMap->OffsetFactor, _T("Offset Factor"));
	//	pOffset->SetData((int)FIXED_PIPE_SHADOW_POLYGON_OFFSET);
	//	pPolygonOffsetGroup->AddSubItem(pOffset);

	//PropertyGrid.AddProperty(pPolygonOffsetGroup);

	//CMFCPropertyGridProperty* pDepthSizeGroup = new CMFCPropertyGridProperty(STRING_SHADOW_MAP_SIZE);
	//	CMFCPropertyGridProperty* pDepthSizeX = new CMFCPropertyGridProperty(_T("x"), (_variant_t) pFixedShadowMap->DepthMapSize.x, _T("Depth Size X"));
	//	pDepthSizeX->SetData((int)FIXED_PIPE_SHADOW_SIZE_X);
	//	pDepthSizeGroup->AddSubItem(pDepthSizeX);
	//	CMFCPropertyGridProperty* pDepthSizeY = new CMFCPropertyGridProperty(_T("y"), (_variant_t) pFixedShadowMap->DepthMapSize.y, _T("Depth Size Y"));
	//	pDepthSizeY->SetData((int)FIXED_PIPE_SHADOW_SIZE_Y);
	//	pDepthSizeGroup->AddSubItem(pDepthSizeY);

	//PropertyGrid.AddProperty(pDepthSizeGroup);

}

//////////////////////////////////////////////////////////////////
//cMaterialPropertyAction
cMaterialPropertyAction::cMaterialPropertyAction(CPropertiesWnd* _pPropertyGrid)
{
	pDiffuseProperty		= 0;
	pSpecularProperty	= 0;
	pAmbientProperty	= 0;
	pEmissiveProperty	= 0;

	pAlphaProperty = 0;
	pPropertyGrid = _pPropertyGrid;
}
cMaterialPropertyAction::~cMaterialPropertyAction()
{

}
void cMaterialPropertyAction::Insert(bgActor* _pObject)
{	
	/*
	cGeometryObject* pGeoObject = dynamic_cast<cGeometryObject*>(_pObject);
	if(pGeoObject == 0 )
		return;

	pObject = pGeoObject;
	//Geometry Object 는 여러개의  Model을 사용할 수 있으나 현재 버전에서는 한개의 모델만 사용하는 것으로 한다.
	cGeometryObject::_ModelList::const_iterator iModel = pGeoObject->ModelList.begin();
	if(iModel == pGeoObject->ModelList.end())
		return;

	const cModel* pModel = (*iModel);
	const bgMaterial& Material = pModel->Material;

	glm::vec4 Diffuse	= Material.Diffuse;
	glm::vec4 Specular  = Material.Specular;
	glm::vec4 Emissive  = Material.Emissive;
	glm::vec4 Ambient  = Material.Ambient;

	int Red, Green, Blue;
	Red = Green = Blue = 0;

	Red		= (int)(Diffuse.r*255.0f);
	Green		= (int)(Diffuse.g*255.0f);
	Blue		= (int)(Diffuse.b*255.0f);
	pDiffuseProperty->SetColor(RGB(Red, Green, Blue));

	Red		= (int)(Specular.r*255.0f);
	Green		= (int)(Specular.g*255.0f);
	Blue		= (int)(Specular.b*255.0f);
	pSpecularProperty->SetColor(RGB(Red, Green, Blue));

	Red		= (int)(Ambient.r*255.0f);
	Green		= (int)(Ambient.g*255.0f);
	Blue		= (int)(Ambient.b*255.0f);
	pAmbientProperty->SetColor(RGB(Red, Green, Blue));

	Red		= (int)(Emissive.r*255.0f);
	Green		= (int)(Emissive.g*255.0f);
	Blue		= (int)(Emissive.b*255.0f);
	pEmissiveProperty->SetColor(RGB(Red, Green, Blue));
*/
}
void cMaterialPropertyAction::InitializeUI()
{
	CMFCPropertyGridCtrl& PropertyGrid = pPropertyGrid->m_wndPropList;

	CMFCPropertyGridProperty* pColorGroup = new CMFCPropertyGridProperty(STRING_MATERIAL_COLOR);
	pDiffuseProperty		= new CMFCPropertyGridColorProperty(STRING_MATERIAL_DIFFUSE, RGB(0,0,0), NULL, _T("Material Diffuse Color"));
	pDiffuseProperty->EnableOtherButton(_T("Other..."));
	pDiffuseProperty->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pDiffuseProperty->SetData((int)DIFFUSE_EVENT);
	pColorGroup->AddSubItem(pDiffuseProperty);

	pSpecularProperty	= new CMFCPropertyGridColorProperty(STRING_MATERIAL_SPECULAR, RGB(0,0,0), NULL, _T("Material Specular Color"));
	pSpecularProperty->EnableOtherButton(_T("Other..."));
	pSpecularProperty->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pSpecularProperty->SetData((int)SPECULAR_EVENT);
	pColorGroup->AddSubItem(pSpecularProperty);

	pAmbientProperty	= new CMFCPropertyGridColorProperty(STRING_MATERIAL_AMBIENT, RGB(0,0,0), NULL, _T("Material Ambient Color"));	
	pAmbientProperty->EnableOtherButton(_T("Other..."));
	pAmbientProperty->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pAmbientProperty->SetData((int)AMBIENT_EVENT);
	pColorGroup->AddSubItem(pAmbientProperty);

	pEmissiveProperty	= new CMFCPropertyGridColorProperty(STRING_MATERIAL_EMISSIVE, RGB(0,0,0), NULL, _T("Material Emissive Color"));	
	pEmissiveProperty->EnableOtherButton(_T("Other..."));
	pEmissiveProperty->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pEmissiveProperty->SetData((int)EMISSIVE_EVENT);
	pColorGroup->AddSubItem(pEmissiveProperty);

	PropertyGrid.AddProperty(pColorGroup);

	CMFCPropertyGridProperty* pAlphaGroup = new CMFCPropertyGridProperty(STRING_MATERIAL_ALPHA);
	pAlphaProperty = new CMFCPropertyGridProperty(STRING_MATERIAL_ALPHA, (_variant_t)255, _T("Material Alpha Value"));
	pAlphaProperty->EnableSpinControl(TRUE, 0, 255);
	pAlphaGroup->SetData((int)ALPHA_EVENT);
	pAlphaGroup->AddSubItem(pAlphaProperty);

	PropertyGrid.AddProperty(pAlphaGroup);
}



/////////////////////////////////////////////////////////////
//cGizmoAction
cGizmoAction::cGizmoAction()
{
	pObject = 0;
	pScene = 0;
}
cGizmoAction::~cGizmoAction()
{

}

/////////////////////////////////////////////////////////////////
//cGizmoTreeAction
cGizmoTreeAction::cGizmoTreeAction(CTreeCtrl* _pObjectTree)
{
	pObjectTree = _pObjectTree;
	CanvasTreeItem = pObjectTree->InsertItem("canvas", 0, 0);
}
cGizmoTreeAction::~cGizmoTreeAction()
{

}
void cGizmoTreeAction::Insert(bgGizmo* pObject)
{
	ASSERT(pObjectTree);
	ASSERT(pObject);

	std::string Name = pObject->name;
	HTREEITEM treeItem = pObjectTree->InsertItem(Name.c_str(), 0, 0);

	//if (bgMaterial* pMaterial = pObject->getMaterial()){
	//	pObjectTree->InsertItem(_T("Material"), treeItem);
	//}

	//if (bgMapper* pMapper = pObject->getInput()){
	//	HTREEITEM hItem = pObjectTree->InsertItem(_T("Mapper"), treeItem);
	//	if (bgData* pData = pMapper->getInput()){
	//		pObjectTree->InsertItem(_T("Data"), hItem);
	//	}
	//}

	//TreeActorItem treeActorItem;
	//treeActorItem.hItem = CanvasTreeItem;
	//treeActorItem.pActor = pObject;
	//TreeItemList.push_back(treeActorItem);
}