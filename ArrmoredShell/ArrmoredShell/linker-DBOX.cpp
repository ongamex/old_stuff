/*Used by the linker*/

#ifndef __DEBUG

	#pragma comment(lib , "d3d9.lib")
	#pragma comment(lib , "d3dx9.lib")
	#pragma comment(lib , "dinput8.lib")
	#pragma comment(lib , "dxguid.lib")
	#pragma comment(lib , "dxerr.lib")

	#pragma comment(lib, "winmm.lib" )
	#pragma comment(lib, "comctl32.lib")

#else
	#pragma comment(lib , "d3d9.lib")
	#pragma comment(lib , "d3dx9.lib")
	#pragma comment(lib , "dinput8.lib")
	#pragma comment(lib , "dxguid.lib")
	#pragma comment(lib , "dxerr.lib")

	#pragma comment(lib, "winmm.lib" )
	#pragma comment(lib, "comctl32.lib")

#endif


//warnings

