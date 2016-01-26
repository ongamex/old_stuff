#define BLACKBOX_DEFINE

#include "BlackBox.h"

namespace Resources{
	namespace Textures{	
		IDirect3DTexture9* LoadTexture(const std::string& Name)
		{
			auto itr = Buffer.find(Name);

			if(itr != Buffer.end()){
			//texture exist 
				return itr->second;
			}

			IDirect3DTexture9* pTexture = NULL;

			//D3DXCreateTextureFromFile(System::Ri3D->GetDevice(), Name.c_str(), &pTexture);
			D3DXCreateTextureFromFileExA(System::Ri3D->GetDevice(), Name.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, 0,
				0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0xffff00ff, 0,0, &pTexture);
			Buffer[Name] = pTexture;

			return pTexture;
		}

		IDirect3DTexture9* GetTexture(const std::string& Name)
		{
			auto itr = Buffer.find(Name);

			if(itr != Buffer.end()){
				return itr->second;
			}
			return NULL;
		}
	};

	namespace Animations{

		ASAnimation* LoadAnimation(const std::string& FileName)
		{
			char* c_str = const_cast<char*>(FileName.c_str());
			CIniReader Reader( c_str );

			ASAnimation* result = new ASAnimation();

			char* Name = Reader.ReadString("info", "name", "invalid-file");
			int NumFrames = Reader.ReadInteger("info", "frames-cnt", 0);
			int CustomPivot = Reader.ReadInteger("info", "custom-pivot", 0);

			//read the info and fill it in the result value
			result->mName = std::string(Name);SAFE_DELETE_ARRAY(Name);
			result->mFrames.resize(NumFrames);
			result->mZ = Reader.ReadFloat("info", "z", 0.10f);
			
			result->mOffset.x = Reader.ReadFloat("info", "ofx", 0.f);
			result->mOffset.y = Reader.ReadFloat("info", "ofy", 0.f);
			result->mOffset.z = 0;

			if(CustomPivot == 1){
				result->mPivotType = DBOX_CENTER_CUSTOM;
				result->mPivot.x = Reader.ReadFloat("info", "pvx", 0.f);
				result->mPivot.y = Reader.ReadFloat("info", "pvy", 0.f);
				result->mPivot.z = 0.f;
			}
			else result->mPivotType = DBOX_CENTER_CENTER;

			// loop trought all frames like hell
			for(int i = 0; i < NumFrames; i++){
				std::string Section = "f" + val2tstr(i);
				
				int d = Reader.ReadInteger(const_cast<char*>(Section.c_str()), "left", -1); 
				if(d < 0){ // if left is invalid value (there is no -1 pixel) set frame area to NULL
					result->mFrames[i].mpFrame = NULL;
				}
				else{
					int left = d;
					int top = Reader.ReadInteger(const_cast<char*>(Section.c_str()), "top", -1);
					int right = Reader.ReadInteger(const_cast<char*>(Section.c_str()), "right", -1);
					int bottom = Reader.ReadInteger(const_cast<char*>(Section.c_str()), "bottom", -1);
					result->mFrames[i].mpFrame = new lRect(left, top, right, bottom);
				}
				result->mFrames[i].mTime = Reader.ReadFloat(const_cast<char*>(Section.c_str()), "time", 0.f);
				char* lpszTexName = Reader.ReadString(const_cast<char*>(Section.c_str()), "texture", "invalid-tex");
				std::string FileName(lpszTexName);SAFE_DELETE_ARRAY(lpszTexName);
				result->mFrames[i].mTexture = Resources::Textures::LoadTexture(FileName);
			}//end for

			Buffer[result->mName] = result;

			return result ;
		}

		ASAnimation* GetAnimation(const std::string& Name){
			auto itr = Buffer.find(Name);
			if(itr != Buffer.end())return itr->second;
			return NULL;
		}
	};

};

namespace Settings{

	void Import()
	{
		CIniReader Reader(CONFING_FILENAME);

		Width = Reader.ReadInteger("settings", "swidth", 1280);
		Height = Reader.ReadInteger("settings", "sheight", 720);
		Windowed = (bool)Reader.ReadInteger("settings", "windowed", 1);
		vSync = (bool)Reader.ReadInteger("settings", "vsync", 0);
		return;
	}

	void Export(){
		CIniWriter Writer(CONFING_FILENAME);
		Writer.WriteInteger("settings", "swidth", Width);
		Writer.WriteInteger("settings", "sheight", Height);
		Writer.WriteInteger("settings", "windowed", (int)Windowed);
		Writer.WriteInteger("settings", "vsync", (int)vSync);
		Writer.WriteString("settings", "GAME", "Arrmored Shell");
		return;
	}
};
