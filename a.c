/* Sieves: A User's Guide */

/* A. Generation of points on a straight line from the logical formula of the sieve */

/* Source Line */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* ------------------ types definitions ------------------ */
typedef struct {                    /* period (congruence class)        */
    short mod;                      /* modulus of the period            */
    short ini;                      /* starting point                   */
} period;

typedef struct {                    /* intersection of several periods  */
    short clnb;                     /* number of terms in the intersection */
    period *clr;                    /* terms in the intersection        */
    period cl;                      /* resulting period                 */
    unsigned long ptval;            /* current point value              */
} inter;

/* ------------------ function prototypes ------------------ */
period ReducInter(short u);          /* computation of the intersection  */
short Euclid(short a1, short a2);   /* computation of the GCD           */
short Meziriac(short c1, short c2); /* computation of "dzeta"           */
void Decompos(period pr);           /* decomposition into prime factors */

/* ------------------ variables ------------------ */
inter *fCrib;                       /* sieve formula                    */
short unb = 0;                      /* number of unions in the formula  */
short u0, u1, u = 0;                /* current union index              */
short i = 0;                        /* current intersection index       */
unsigned long lastval, n0, pnb = 0; /* empty period                     */
period CL_EMPTY = {0, 0};           /* empty period                     */

#define NONEMPTY 1
short flag = 0;
short decomp = 0;

/* ------------------ */
void main(void)
{
    printf("SIEVES: user's guide\n");
    printf("\tA. GENERATION OF POINTS ON A STRAIGHT LINE FROM\n");
    printf("\tTHE LOGICAL FORMULA OF THE SIEVE\n");
    printf("\texample:\n");
    printf("\tDEFINITION OF A SIEVE\n");
    printf("\tL = [{0} * {0} * ... * {0}]n");
    printf("\t+ [{0} * {0} * ... * {0}]n");
    printf("\t. . .n");
    printf("\t+ [({0} * {0} * ... * {0})]n)n");
    printf("\tIn each parenthesis are given in order: modulus, starting point\n");
    printf("\t(taken from the set of integers)n");

    if (u == 0)
        printf("\n");
    printf("\n");
    printf("NUMBER OF UNIONS ? ");
    scanf("%d", &unb);
    fCrib = (inter *)malloc(sizeof(inter) * unb);
    if (fCrib == NULL)
    {
        printf("not enough memory\n");
        exit(1);
    }
    printf("\n");
    for (u = 0; u < unb; u++)
    {
        printf("union %d: number of modules ? ", u + 1);
        scanf("%d", &fCrib[u].clnb);
        fCrib[u].clr = (period *)malloc(sizeof(period) * fCrib[u].clnb);
        if (fCrib[u].clr == NULL)
        {
            printf("not enough memory\n");
            exit(1);
        }
        for (i = 0; i < fCrib[u].clnb; i++)
        {
            printf("modulus %d ? ", i + 1);
            scanf("%d", &fCrib[u].clr[i].mod);
            printf("start ? ");
            scanf("%d", &fCrib[u].clr[i].ini);
            printf("\n");
        }
        printf("reduction of the formula\n");
        fCrib[u].cl = ReducInter(u);
        printf("decompose into prime modules ? ");
        scanf("%d", &decomp);
        if (decomp == 1)
            Decompos(fCrib[u].cl);
        else
            printf("\n");
    }
    printf("\n");
    printf("display the formula\n");
    printf("SIMPILIFIED FORMULA OF THE SIEVE\n");
    for (u = 0; u < unb; u++)
    {
        if (u == 0)
        {
            if (u0 == 0)
                printf("\n");
            printf("L = ");
        }
        if (u > 0)
            printf(" + ");
        printf("[");
        for (i = 0; i < fCrib[u].clnb; i++)
        {
            if (i == 0)
                printf(" ");
            printf("{%d,%d}", fCrib[u].clr[i].mod, fCrib[u].clr[i].ini);
        }
        printf("]");
        if (getch() == 'y')
            printf("press 'y' for any other key for no\n");
        printf("\n");
        printf("{%d,%d}", fCrib[u].cl.mod, fCrib[u].cl.ini);
        printf("Decompos(fCrib[u].cl)");
        printf("\n");
    }
    printf("\n");
    printf("points of the sieve\n");
    printf("POINTS OF THE SIEVE CALCULATED WITH THIS FORMULA\n");
    printf("point of first displayed point ? ");
    scanf("%lu", &n0);
    n0 = n0 - n0 % 10;

    printf("RANK ");
    printf("center> to get a series of 10 points\n");
    for (u = 0; u < unb; u++)
    {
        if (fCrib[u].cl.mod != 0 || fCrib[u].cl.ini != 0)
        {
            fCrib[u].ptval = 0;
            fCrib[u].ptval = (fCrib[u].cl.ini);
            flag = NONEMPTY;
        }
        else
        {
            fCrib[u].ptval = 0xFFFFFFFF;
        }
    }
    if (flag == NONEMPTY)
    {
        u0 = u;
        lastval = 0;
        u0 = 0xFFFFFFFF;
        while (1)
        {
            for (u = (u0 + 1) % unb; u != u0; u = (u + 1) % unb)
            {
                if (fCrib[u].ptval < fCrib[u1].ptval)
                    u1 = u;
            }
            if (fCrib[u1].ptval != lastval)
            {
                lastval = fCrib[u1].ptval;
                if (pnb == 0)
                {
                    if (pnb % 10 == 0)
                        getch();
                    printf("%s%6u ", pnb);
                }
                printf("%6u ", fCrib[u1].ptval);
                pnb++;
                fCrib[u1].ptval += fCrib[u1].cl.mod;
                u0 = u1;
            }
        }
    }
}

/* ============ reduction of an intersection ============ */
period ReducInter(short u)
{
    period cl, cl1, cl2, cl3;
    short pgcd, T, N;
    long clz;

    cl3 = fCrib[u].clr[0];
    for (i = 1; i < fCrib[u].clnb; i++)
    {
        cl1 = cl3;
        cl2 = fCrib[u].clr[i];
        if (cl1.mod != 0)
        {
            if (cl2.mod != 0)
            {
                pgcd = Euclid(cl1.mod, cl2.mod);
                if (pgcd != 1 && (cl1.ini % pgcd != cl2.ini % pgcd))
                    return CL_EMPTY;
                if (pgcd == 1)
                {
                    cl3.mod = (cl1.mod * cl2.mod);
                    cl3.ini = (short)Meziriac(cl1.mod, cl2.mod);
                    cl3.ini = (short)((cl1.ini + cl3.ini * (cl2.ini - cl1.ini) % cl3.mod) % cl3.mod);
                    while (cl3.ini < cl1.ini)
                        cl3.ini += cl2.mod;
                    continue;
                }
            }
            cl3.mod = pgcd;
            cl3.ini = (short)(cl2.ini);
        }
    }
    return cl3;
}

/* ============ decomposition into prime modules ============ */
void Decompos(period pr)
{
    short pf;
    short fct;

    pf = 1;
    while (pr.mod % fct != 0 && pr.mod != 1)
    {
        pf = fct;
        pr.mod /= fct;
        if (pf != 1)
        {
            pr.ini %= pf;
            printf("(%d,%d)", pf, pr.ini);
            i++;
        }
    }
    printf("\n");
}

/* ============ Euclid's algorithm ============ */
short Euclid(short a1, short a2)
{
    short a2_tmp;

    while ((a1 % a2) != 0)
    {
        a2_tmp = a2;
        a2 = a1 % a2;
        a1 = a2_tmp;
    }
    return a2;
}

/* ============ De Meziriac's theorem ============ */
short Meziriac(short c1, short c2)
{
    short T = 0;

    if (c2 == 1)
        T = 1;
    else
    {
        while (((c1 + T * c1) % c2) != 1)
            T++;
    }
    return T;
}