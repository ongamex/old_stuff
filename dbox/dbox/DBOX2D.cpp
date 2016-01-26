#include "DBOX2D.h"

//Create Class

LPDBOX2D DBOXCreate2D(LPDBOX3D p3D)
{
	LPDBOX2D retval = new DBOX2D ;
	retval->SetSprite(p3D->GetSprite()) ;
	retval->hWnd = p3D->GetPresentParameters().hDeviceWindow;
	//Null the data
	retval->CamPos = D3DXVECTOR3(0,0,0);
	retval->CamZRotation = 0.0f;
	D3DXMatrixIdentity(&retval->CamTransl);
	D3DXMatrixIdentity(&retval->CamTranslInv);
	return retval;
}


//Class definition 

const DWORD DBOX2D::SpriteFlags = D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK ;

DBOX2D::DBOX2D()
{
	pSprite = 0;
	ScreenWidth = ScreenHeight = 0;
}

DBOX2D::~DBOX2D()
{
	pSprite = 0;
}

//Set

void DBOX2D::SetUp(long Width , long Height , float ScaleX ,float ScaleY)
{
	this->ScreenWidth = Width ;
	this->ScreenHeight = Height ; 
	this->ScaleX = ScaleX;
	this->ScaleY = ScaleY;

	AdjustCameraMatrix();//set up the camera
	return ;
}

void DBOX2D::SetSprite(LPD3DXSPRITE pSprite){this->pSprite = pSprite ;return ;}

//Get
LPD3DXSPRITE DBOX2D::GetSprite(){return pSprite;}

//Camera Render

void DBOX2D::AdjustCameraMatrix()
{
	//Adjust
	CamPos.z = 0;
	D3DXMatrixTranslation(&CamTransl , CamPos.x - (float)(ScreenWidth)/2.0f , CamPos.y - (float)(ScreenHeight)/2.0f, 0);
	D3DXMatrixInverse(&CamTranslInv , 0 , &CamTransl);

	return ;
}

void DBOX2D::MoveCameraTo(D3DXVECTOR3* pVector)
{
	if(pVector)
	CamPos = *pVector;
	else CamPos = D3DXVECTOR3(0,0,0);//move to Center of the world

	AdjustCameraMatrix();	return ;

}
void DBOX2D::MoveCameraBy(D3DXVECTOR3* pVector)
{
	if(pVector)
	CamPos += *pVector;
	else CamPos = D3DXVECTOR3(0,0,0);//move to Center of the world
	CamPos.z = 0;
	AdjustCameraMatrix();	return ;
}


HRESULT DBOX2D::DrawToScreen(LPDIRECT3DTEXTURE9 Texture ,D3DXMATRIX *Transform ,RECT* Rect , long CenterType , D3DXVECTOR3* pCenter , DWORD ColorFilter)
{
	if( ! Texture ) return DBOX_ERROR;
	/*
		if (! Transform ) {  } 
	*/
	D3DXVECTOR3 Center(0,0,0);

	switch ( CenterType )
	{
	case DBOX_CENTER_LEFTTOP : {
		//do nothing
		//Center = D3DXVECTOR3(0,0,0);
								}break ;

	case DBOX_CENTER_CENTER :{
		if(! Rect ) // if Rect is null use the center of the texture
		{
			D3DSURFACE_DESC desc;
			if(FAILED(Texture->GetLevelDesc(0 , &desc)))
				return DBOX_ERROR;
			Center.x = (float)( desc.Width/2 );
			Center.y = (float)( desc.Height/2 );
		}
		else
		{
			Center.x =(float) ( Rect->right - Rect->left ) / 2.0f ;
			Center.y =(float) ( Rect->bottom - Rect->top ) / 2.0f ;
		}
		if(Center.x < 0 || Center.y < 0)return DBOX_ERROR ; //if center is invalid
		}break;

	case DBOX_CENTER_CENTERBOTTOM :{
		if(! Rect ) // if Rect is null
		{
			D3DSURFACE_DESC desc;
			if(FAILED(Texture->GetLevelDesc(0 , &desc)))
				return DBOX_ERROR;
			Center.x = (float)( desc.Width/2 );
			Center.y = (float)( desc.Height  );
		}
		else
		{
			Center.x =(float) ( Rect->right - Rect->left ) / 2.0f ;
			Center.y =(float) ( Rect->bottom - Rect->top ) ;
		}
	}break;

	case DBOX_CENTER_CUSTOM : 
	{
		if(! pCenter ) return DBOX_ERROR;
		//if(pCenter->x < 0 || pCenter->y < 0)return DBOX_ERROR;//if center is invalid value 
		Center = *pCenter;
	}break;

	default : 
		CenterType = DBOX_CENTER_LEFTTOP ; 
		//Center = D3DXVECTOR3(0,0,0);

	}//end 

	pSprite->Begin(SpriteFlags);
	pSprite->SetTransform(Transform);
	pSprite->Draw(Texture , (RECT*)Rect , &Center , &D3DXVECTOR3(0,0,0) , ColorFilter);
	pSprite->SetTransform(&DBOXMatIdent);
	pSprite->End();
	return DBOX_OK;
}

HRESULT DBOX2D::DrawToWorld(LPDIRECT3DTEXTURE9 Texture ,D3DXMATRIX *Transform ,RECT* Rect , long CenterType , D3DXVECTOR3* pCenter , DWORD ColorFilter)
{
	if( ! Texture ) return DBOX_ERROR;

	D3DXMATRIX ObjT , ObjRS;//object Translation and RotationAndScale

	D3DXMatrixIdentity(&ObjT);
	D3DXMatrixIdentity(&ObjRS);

	D3DXVECTOR3 ObjWorldPos(0,0,0);

	if( Transform )
	{
		
		//get the translation
		ObjWorldPos.x = ObjT._41 = Transform->_41;
		ObjWorldPos.y = ObjT._42 = Transform->_42;
		ObjWorldPos.z = ObjT._43 = Transform->_43;

		//get the rotation

		//copy the all data and null(identity) the other
		ObjRS = *Transform ;

		ObjRS._41 = 0;
		ObjRS._42 = 0;
		ObjRS._43 = 0;
		
		ObjRS._14 = 0;
		ObjRS._24 = 0;
		ObjRS._34 = 0;

		//ObjRS._44 = 1.0f;//SetUp by identity
	}
	else{
		//Use the Identity ObjT ObjRS 
	}

	//SetUp the center


	D3DXVECTOR3 Center(0,0,0);

	switch ( CenterType )
	{
	case DBOX_CENTER_LEFTTOP : {
		//do nothing
		//Center = D3DXVECTOR3(0,0,0);
								}break ;

	case DBOX_CENTER_CENTER :{
		if(! Rect ) // if Rect is null use the center of the texture
		{
			D3DSURFACE_DESC desc;
			if(FAILED(Texture->GetLevelDesc(0 , &desc)))
				return DBOX_ERROR;
			Center.x = (float)( desc.Width/2 );
			Center.y = (float)( desc.Height/2 );
		}
		else
		{
			Center.x =(float) ( Rect->right - Rect->left ) / 2.0f ;
			Center.y =(float) ( Rect->bottom - Rect->top ) / 2.0f ;
		}
		if(Center.x < 0 || Center.y < 0)return DBOX_ERROR ; //if center is invalid
		}break;

	case DBOX_CENTER_CUSTOM : 
	{
		if(! pCenter ) return DBOX_ERROR;
		if(pCenter->x < 0 || pCenter->y < 0)return DBOX_ERROR;//if center is invalid value 
		Center = *pCenter;
	}break;

	default : 
		CenterType = DBOX_CENTER_LEFTTOP ; 
		//Center = D3DXVECTOR3(0,0,0);

	}//end 

	

	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale , this->ScaleX , this->ScaleY , 1.f);
	
	D3DXMATRIX CamRotationZ;
	D3DXMatrixRotationZ(&CamRotationZ , -this->CamZRotation);

	D3DXMATRIX mObjCamPosition , mObjCamPositionInv;
	D3DXMatrixTranslation(&mObjCamPosition , (ObjWorldPos.x - CamPos.x), (ObjWorldPos.y - CamPos.y) , 0);//the translation needed for camera rotation
	D3DXMatrixInverse(&mObjCamPositionInv , NULL , &mObjCamPosition);

	pSprite->Begin(SpriteFlags);
	pSprite->SetTransform(&(ObjRS*mObjCamPosition*CamRotationZ*((Scale*ObjT*CamTranslInv))*mObjCamPositionInv));
	pSprite->Draw(Texture , Rect , &Center , &D3DXVECTOR3(0,0,0) , ColorFilter);
	pSprite->SetTransform(&DBOXMatIdent);
	pSprite->End();

	return DBOX_OK;
}


/*2D Animation*/


void Animation2DAnimate(Animation2D* pAnimation, Animation2DAnimInfo* pInfo, float DeltaTime, LPDBOX2D Engine2D, D3DXMATRIX* pTransf, bool ResetElapsedTime)
{
	float PassedTime = pInfo->m_ElapsedTime + DeltaTime;
	float TotalTime = pAnimation->GetTotalTime();
	while(PassedTime > pAnimation->GetTotalTime())
	{
		PassedTime -= pAnimation->GetTotalTime();
	}

	if(ResetElapsedTime)
	{
		pInfo->m_ElapsedTime = PassedTime;
	}
	else
	{
		pInfo->m_ElapsedTime += DeltaTime;
	}

	int FrameToDraw = 0;

	float Total = 0.f;
	for(int i = 0; i < pAnimation->m_FramesDuration.size(); i++)
	{
		Total += pAnimation->m_FramesDuration[i];
		if(PassedTime <= Total)break;
		FrameToDraw++;
	}

	Engine2D->DrawToWorld(pAnimation->m_Animations[FrameToDraw]->m_pTexture, 
							pTransf, 
							NULL,
							pInfo->m_CenterType,
							pInfo->m_Pivot,
							0xffffffff);
	

	return;
}
