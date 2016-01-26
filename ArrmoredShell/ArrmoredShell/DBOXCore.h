#ifndef DBOXCORE_H
#define DBOXCORE_H
 
#define DBOX_OK			(HRESULT)(0)
#define DBOX_ERROR		(HRESULT)(-1)

#include <WindowsX.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#define SAFE_DELETE(p) {if(p) {delete(p); (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[](p); (p)=NULL;}}
#define SAFE_RELEASE(p) { if(p) { (p)->Release();} (p) = NULL;}

#ifdef UNICODE
	//strings
	#define tstring std::wstring
	#define tstrlen wcslen
	//streams
	#define tstringstream std::wstringstream
	#define tostringstream std::wostringstream
	#define tistringstream std::wistringstream
	//files
	#define tfstream std::wftream
	#define tofstream std::woftream
	#define tifstream std::wiftream
#else//if not UNICODE
	//string
	#define tstring std::string
	#define tstrlen strlen
	//streams
	#define tstringstream std::stringstream
	#define tostringstream std::ostringstream
	#define tistringstream std::istringstream
	//files
	#define tfstream std::ftream
	#define tofstream std::oftream
	#define tifstream std::iftream
#endif

//convert varible to tstring
template<typename T>
tstring val2tstr(T val)
{
	tstringstream stream;
	stream << val;
	return stream.str();
}

//convert tstring to varible
template<typename T>
T tstr2val(tstring str)
{
	T retval;
	tstringstream stream(str);
	stream >> retval;
	return retval;
}

class  DBOXResourceManager {
	virtual void OnLostDevice() = 0;
	virtual void OnResetDevice() = 0;
	virtual void PushObject(void* pArg) = 0;
};


#endif