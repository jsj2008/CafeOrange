// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.



//TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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

