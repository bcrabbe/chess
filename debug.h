#define iprint(expr)	printf(#expr" = %i \n",expr)
#define fprint(expr)	printf(#expr" = %f \n",expr)
#define sprint(expr)	printf(#expr "= %s \n",expr)
#define cprint(expr)	printf(#expr "= %c \n",expr)
#define pprint(expr)	printf(#expr "= %p \n",expr)
#define bprint(expr)	printf(#expr "= %b \n",expr)
#define lfprint(expr)	printf(#expr "= %lf \n",expr)
#define pNL()           printf("\n");
#define	cL		while(getchar() != '\n') {}
