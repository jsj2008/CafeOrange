#pragma once

#include "GlobalEnum.h"
#include "Renderer.h"
#include <string>
using namespace std;

class bgObject
{
public:
	std::string name;

	bgObject();
	virtual ~bgObject();
	
	bool		isModified() { return modified; }
	void		setModified(bool _modified) { modified = _modified; }
	void		setId(std::string& _id) { id = _id;  }
	std::string getId(){ return id;  }

	virtual bool initialize();
	virtual bool update();
	virtual bool build(cRenderer& Renderer);

protected:
	bool modified;
	std::string id;

};



