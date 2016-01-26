#ifndef DBOXCORE_H
#define DBOXCORE_H
/*DBOX all technology is owned by Kostadin Petkov ! */

#ifndef _address
#define _address(v) ((void*)(&v))
#endif 

#define DBOX_OK			(HRESULT)(0)
#define DBOX_ERROR		(HRESULT)(-1)

#include <string>
#include <sstream>

template <typename T> std::string ToString(T obj){std::stringstream ss; ss << obj; return ss.str();}

#endif