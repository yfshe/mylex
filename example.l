${
#include <stdio.h>
}$

$$

\s+                     {;}
if                      {printf("find 'if'\n");}
else                    {printf("find 'else'\n");}
while                   {printf("find 'while'\n");}
[_a-zA-Z][_a-zA-Z0-9]*  {printf("find id\n");}
[1-9][0-9]*             {printf("find integer\n");}
<                       {printf("find '<'\n");}
<=                      {printf("find '<='\n");}
>                       {printf("find '>'\n");}
>=                      {printf("find '>='\n");}
=                       {printf("find '='\n");}
==                      {printf("find '=='\n");}

$$

main()
{
    yy_lex();
}
