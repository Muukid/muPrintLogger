#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifndef MUPL_FILENAME
	#define MUPL_FILENAME "log.txt"
#endif

void mupl_init() {
	FILE* f = fopen(MUPL_FILENAME, "w");
	fclose(f);
}

void mupl_term() {
	FILE* f = fopen(MUPL_FILENAME, "a");
	fprintf(f, "[MUPL] Program closed successfully (if you don't see this message, the program crashed!)\n");
	fclose(f);
}

void mupl_printf(const char* p, ...) {
	va_list args;
	va_start(args, p);

	size_t size = vsnprintf(0, 0, p, args);
	char* s = malloc(size+1);
	vsprintf(s, p, args);
	s[size] = '\0';

	printf("%s", s);

	FILE* f = fopen(MUPL_FILENAME, "a");
	fprintf(f, (const char*)s);
	fclose(f);

	free(s);

	va_end(args);
}

#define mu_print mupl_printf
#define mu_printf mupl_printf