#pragma once

#define WIN32_LEAN_AND_MEAN             // Excluir material rara vez utilizado de encabezados de Windows

#if defined _M_X64 || defined _M_AMD64
	#define _AMD64_
#endif
