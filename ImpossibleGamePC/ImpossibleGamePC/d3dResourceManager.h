#pragma once

#include <string>
#include <map>
#include <d3dx9.h>

class d3dResourceHandler
{
public : 
	virtual bool OnLostDevice() = 0;
	virtual bool OnResetDevice() = 0;
	virtual bool OnRelease() = 0;
};

//Textures Resource manager 

class d3dTextureManager : public d3dResourceHandler{ 
	LPDIRECT3DDEVICE9 d3ddev;
	std::map<std::string , IDirect3DTexture9*> Buffer;
public :

	d3dTextureManager() : d3ddev(nullptr) {}
	d3dTextureManager(LPDIRECT3DDEVICE9 device) : d3ddev(device) {}

	bool Load(std::string TextureName);
	bool Unload(std::string TextureName);
	bool Reload(std::string TextureName);

	IDirect3DTexture9* Get(std::string TextureName);

	bool OnLostDevice();
	bool OnResetDevice();
	bool OnRelease();
};
