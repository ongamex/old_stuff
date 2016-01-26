#ifndef DBOXCORE_H
#define DBOXCORE_H
/*DBOX all technology is owned by Kostadin Petkov ! */

//select CHARSET
#ifdef UNICODE
#define CHARTYPE wchar_t
#define STRTYPE  wchar_t*
#else
#define CHARTYPE char
#define STRTYPE  char*
#endif

#define DBOX_OK			(HRESULT)(0)
#define DBOX_ERROR		(HRESULT)(-1)

#include <string>
#include <sstream>
template <typename T> std::string ToString(T obj){std::stringstream ss; ss << obj; return ss.str();}

#endif