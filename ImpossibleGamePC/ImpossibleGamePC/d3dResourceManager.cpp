#include "d3dResourceManager.h"

bool d3dTextureManager::Load(std::string TextureName){

	//Check is the texture already in the buffer
	if( Buffer.find(TextureName) != Buffer.end()) 
	{ 
		//Texture already exist
		return false;
	}

	HRESULT hr = D3DXCreateTextureFromFileExA(d3ddev , TextureName.c_str() , D3DX_DEFAULT , D3DX_DEFAULT , 0 , 0 , D3DFMT_A8R8G8B8 , D3DPOOL_MANAGED , 0 , 0 , 
		0xff00aaff , nullptr , nullptr , &Buffer[TextureName]);
	
	if(FAILED(hr))
	{
		//failed loading texture
		Buffer[TextureName] = nullptr;
		return false;
	}

	return true;
}

bool d3dTextureManager::Unload(std::string TextureName)
{
	if( Buffer.find(TextureName) == Buffer.end())
	{
		//there is no texture
		return false;
	}

	if(Buffer[TextureName]  == NULL)
	{
		return false;
	}
	
	//Texture is valid
	//Unlod it
	Buffer[TextureName]->Release();
	Buffer[TextureName] = NULL;
}

bool d3dTextureManager::Reload(std::string TextureName)
{
	this->Unload(TextureName); 
	return this->Load(TextureName);
}

IDirect3DTexture9* d3dTextureManager:: Get(std::string TextureName)
{
	return Buffer[TextureName];
}

bool d3dTextureManager::OnLostDevice()
{
	return true;
}
bool d3dTextureManager::OnResetDevice()
{
	return true;
}
bool d3dTextureManager::OnRelease()
{
	for(auto i = Buffer.begin() ; i != Buffer.end() ; ++i)
	{
		if(i->second)
		{
			i->second->Release();
			i->second = NULL;
		}
	}

	return true;
}