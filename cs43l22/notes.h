#ifndef __NOTES_H
#define __NOTES_H

#include "main.h"
#include "tim.h"

typedef enum {
	DO,
	RE,
	MI,
	FA,
	SOL,
	LA,
	SI,
	NOTES_SIZE
} NOTES_t;

void note_do_func(void);
void note_re_func(void);
void note_mi_func(void);
void note_fa_func(void);
void note_sol_func(void);
void note_la_func(void);
void note_si_func(void);

#endif /* __NOTES_H */
