#ifndef NEW_H
#define NEW_H

#include <stdlib.h>

#define NEW(p)                                  \
  do{                                           \
    p = malloc(sizeof(*(p)));                   \
    if (0==(p)){                                \
      printf ("allocation failed\n");           \
      exit(0);                                  \
    }                                           \
  }while(0);

#define LOSE(p)		\
	{				\
		free((p));	\
		p = 0;		\
	}

#endif