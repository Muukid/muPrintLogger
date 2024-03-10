/*
muGraphicsAPI - Muukid
Public domain single-file C library for creating cross-graphics-API applications.
No warranty implied; use at your own risk.

Licensed under MIT License or public domain, whichever you prefer.
More explicit license information at the end of the file.
*/

#ifndef MUPL_H
	#define MUPL_H

	#if !defined(MU_SECURE_WARNINGS) && !defined(_CRT_SECURE_NO_WARNINGS)
		#define _CRT_SECURE_NO_WARNINGS
	#endif
	
	#ifdef __cplusplus
		extern "C" {
	#endif

	#define MUPL_VERSION_MAJOR 1
	#define MUPL_VERSION_MINOR 0
	#define MUPL_VERSION_PATCH 0

	#if !defined(MU_FILE)       || \
		!defined(mu_fopen)      || \
		!defined(mu_fclose)     || \
		!defined(mu_fprintf)    || \
		!defined(mu_std_printf) || \
		!defined(mu_vsprintf)   || \
		!defined(mu_vsnprintf)

		#include <stdio.h>

		#ifndef MU_FILE
			#define MU_FILE FILE
		#endif

		#ifndef mu_fopen
			#define mu_fopen fopen
		#endif

		#ifndef mu_fclose
			#define mu_fclose fclose
		#endif

		#ifndef mu_fprintf
			#define mu_fprintf fprintf
		#endif

		#ifndef mu_std_printf
			#define mu_std_printf printf
		#endif

		#ifndef mu_vsprintf
			#define mu_vsprintf vsprintf
		#endif

		#ifndef mu_vsnprintf
			#define mu_vsnprintf vsnprintf
		#endif

	#endif

	#if !defined(mu_va_list)  || \
		!defined(mu_va_start) || \
		!defined(mu_va_end)

		#include <stdarg.h>

		#ifndef mu_va_list
			#define mu_va_list va_list
		#endif

		#ifndef mu_va_start
			#define mu_va_start va_start
		#endif

		#ifndef mu_va_end
			#define mu_va_end va_end
		#endif

	#endif

	#if !defined(mu_malloc) || \
		!defined(mu_free)

		#include <stdlib.h>

		#ifndef mu_malloc
			#define mu_malloc malloc
		#endif

		#ifndef mu_free
			#define mu_free free
		#endif

	#endif

	#if !defined(size_m)

		#include <stddef.h>

		#ifndef size_m
			#define size_m size_t
		#endif

	#endif

	#ifndef MUDEF
		#ifdef MU_STATIC
			#define MUDEF static
		#else
			#define MUDEF extern
		#endif
	#endif

	/* Functions */

	MUDEF void mupl_init(void);
	MUDEF void mupl_term(void);
	MUDEF void mupl_printf(const char* p, ...);
	MUDEF void mupl_log(const char* p, ...);

	/* Macros */

	#define mu_print mupl_printf
	#define mu_printf mupl_printf

	#ifdef __cplusplus
		}
	#endif
#endif


#ifdef MUPL_IMPLEMENTATION

	#ifdef __cplusplus
		extern "C" {
	#endif

	#ifndef MUPL_FILENAME
		#define MUPL_FILENAME "log.txt"
	#endif

	MUDEF void mupl_init(void) {
		MU_FILE* f = mu_fopen(MUPL_FILENAME, "w");
		if (f == 0) {
			return;
		}
		mu_fclose(f);
	}

	MUDEF void mupl_term(void) {
		MU_FILE* f = mu_fopen(MUPL_FILENAME, "a");
		if (f == 0) {
			return;
		}
		mu_fprintf(f, "[MUPL] Program closed successfully (if you don't see this message, the program crashed!)\n");
		mu_fclose(f);
	}

	MUDEF void mupl_printf(const char* p, ...) {
		mu_va_list args;
		mu_va_start(args, p);

		size_m size = mu_vsnprintf(0, 0, p, args);
		char* s = (char*)mu_malloc(size+1);
		if (s == 0) {
			mu_va_end(args);
			return;
		}
		mu_vsprintf(s, p, args);
		s[size] = '\0';

		mu_std_printf("%s", s);

		MU_FILE* f = mu_fopen(MUPL_FILENAME, "a");
		if (f == 0) {
			mu_free(s);
			mu_va_end(args);
			return;
		}
		mu_fprintf(f, "%s", s);
		mu_fclose(f);

		mu_free(s);

		mu_va_end(args);
	}

	MUDEF void mupl_log(const char* p, ...) {
		mu_va_list args;
		mu_va_start(args, p);

		size_m size = mu_vsnprintf(0, 0, p, args);
		char* s = (char*)mu_malloc(size+1);
		if (s == 0) {
			mu_va_end(args);
			return;
		}
		mu_vsprintf(s, p, args);
		s[size] = '\0';

		MU_FILE* f = mu_fopen(MUPL_FILENAME, "a");
		if (f == 0) {
			mu_free(s);
			mu_va_end(args);
			return;
		}
		mu_fprintf(f, "%s", s);
		mu_fclose(f);

		mu_free(s);

		mu_va_end(args);
	}

	#ifdef __cplusplus
		}
	#endif

#endif /* MUPL_IMPLEMENTATION */

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2024 Hum
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
