#include "OBB3D.h"
#include "DBOXCore.h"

OBB3D::OBB3D()
{
	D3DXMatrixIdentity(&TransformMatrix);
}

OBB3D::OBB3D(float sx , float sy , float sz)
{
	sx_ = sx;
	sy_ = sy;
	sz_ = sz;
	//top
	Edge[0] = D3DXVECTOR3(-sx/2 ,	sy/2 , sz/2);
	Edge[1] = D3DXVECTOR3(sx/2 ,	sy/2 , sz/2);
	Edge[2] = D3DXVECTOR3(-sx/2 ,	sy/2 , -sz/2);
	Edge[3] = D3DXVECTOR3(sx/2 ,	sy/2 , -sz/2);
	//bottom
	Edge[4] = D3DXVECTOR3(-sx/2 ,	-sy/2 ,	sz/2);
	Edge[5] = D3DXVECTOR3(sx/2 ,	-sy/2 ,	sz/2);
	Edge[6] = D3DXVECTOR3(-sx/2 ,	-sy/2 ,	-sz/2);
	Edge[7] = D3DXVECTOR3(sx/2 ,	-sy/2 ,	-sz/2);

	D3DXMatrixIdentity(&TransformMatrix);
}
void OBB3D::SetTransform(D3DXMATRIX& t)
{
	TransformMatrix = t;
}

bool LineOverLap1D(float a1 ,float a2, float b1, float b2)
{
	float ad = fabsf(a1 - a2);
	float bd = fabsf(b1 - b2);

	if(ad < bd)
	{
		if(a1 <= b1 && a1 >= b2 || a2 <= b1 && a2 >= b2)
			return true;
	}else
	{
		if(b1 <= a1 && b1 >= a2 || b2 <= a1 && b2 >= a2)
			return true;
	}
	return false;
}

bool OBB3D::Overlap(OBB3D& other)
{
	
	D3DXMATRIX OtherMatrixInv = other.TransformMatrix;
	D3DXMatrixInverse(&OtherMatrixInv , 0 , &OtherMatrixInv);
	D3DXMATRIX FinalTransform = this->TransformMatrix*OtherMatrixInv;

	D3DXVECTOR3 OtherTransformedEdges[8];

	for(int i = 0 ; i < 8 ; ++i)
	{
		D3DXVec3TransformCoord(&OtherTransformedEdges[i] , &other.Edge[i] , &FinalTransform);
	}

	float	maxX = OtherTransformedEdges[0].x, minX = OtherTransformedEdges[0].x ,
			maxY = OtherTransformedEdges[0].y, minY = OtherTransformedEdges[0].y , 
			maxZ = OtherTransformedEdges[0].z, minZ = OtherTransformedEdges[0].z ;

	for(int i = 0 ; i < 8 ; ++i)
	{
		if(maxX < OtherTransformedEdges[i].x)maxX = OtherTransformedEdges[i].x;
		if(minX > OtherTransformedEdges[i].x)minX = OtherTransformedEdges[i].x;

		if(maxY < OtherTransformedEdges[i].y)maxY = OtherTransformedEdges[i].y;
		if(minY > OtherTransformedEdges[i].y)minY = OtherTransformedEdges[i].y;

		if(maxZ < OtherTransformedEdges[i].z)maxZ = OtherTransformedEdges[i].z;
		if(minZ > OtherTransformedEdges[i].z)minZ = OtherTransformedEdges[i].z;
	}

	if(	LineOverLap1D(maxX,minX,this->sx_/2,-this->sx_/2) && 
		LineOverLap1D(maxY,minY,this->sy_/2,-this->sy_/2) && 
		LineOverLap1D(maxZ,minZ,this->sz_/2,-this->sz_/2))
		return true;
	

	return false;
}

bool OBB3DCollide(OBB3D& b1 , OBB3D& b2)
{
	bool c1 = b1.Overlap(b2) , c2 = false;
	if(c1){ c2 = b2.Overlap(b1); }
	return c1 && c2;
}