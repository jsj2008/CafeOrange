// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.



//TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <afxwin.h> 
//OpenGL Mathmatics Library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

//Glew-1.12.0
#include "GL/glew.h"
#include "GL/wglew.h"
//Glut
#include <GL/GL.h>
#include <GL/GLU.h>

//OpenCV version3.0 (2015-06-04 release)
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"

//SOIL , http://www.lonesock.net/soil.html
#include "SOIL.h"

#include "DefineEvent.h"
#include "FileHelper.h"
#include "StringHelper.h"
#include "bgUtil.h"

#include <tchar.h>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

