/* Source Line */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------ types definitions ------------------ */
typedef struct {                    /* period (congruence class) */
    short mod;                      /* modulus of the period */
    short ini;                      /* starting point */
    short couv;                     /* number of covered points */
} period;

/* ------------------ function prototypes ------------------ */
unsigned long Euclid(unsigned long a1, unsigned long a2); /* computation of the GCD */

/* ------------------ variables and constants ------------------ */
period *perCrib;                     /* periods of the sieve */
short perTotNb = 0;                 /* number of periods in the formula */
long *ptCrib;                       /* points of the cribble */
long *ptReste;                      /* points outside the periods */
short ptTotNb = 0;                  /* number of points in the sieve */
short p, ptnb;
long ptval;
unsigned long percrib;

period *per;
#define NON_REDUNDANT 0
#define REDUNDANT 1
#define COVERED -1L
short flag;

/* ------------------ */
void main(void)
{
    printf("B. GENERATION OF THE LOGICAL FORMULA OF THE SIEVE FROM\n");
    printf("A SERIES OF POINTS ON A STRAIGHT LINE\n\n");
    printf("Example:\n");
    printf("\n");
    printf("Given a series of points, find the starting point\n");
    printf("with their moduli (periods),\n");
    printf("----entry of the points of the sieve and their sorting----\n");
    while (ptTotNb == 0)
    {
        printf("NUMBER OF POINTS ? -");
        scanf("%d", &ptTotNb);
    }
    ptCrib = (long *)malloc(ptTotNb * sizeof(long));
    ptReste = (long *)malloc(ptTotNb * sizeof(long));
    perCrib = (period *)malloc(ptTotNb * sizeof(period));
    if (perCrib == NULL || ptReste == NULL || ptCrib == NULL)
    {
        printf("not enough memory\n");
        exit(1);
    }
    printf("abscissa of the points:\n");
    for (p = 0; p < ptTotNb; p++)
    {
        if (p % 4 == 0)
        {
            printf("\n");
            printf("point %2d = ", p + 1);
            scanf("%ld", &ptval);
        }
        ptCrib[p] = ptval;
    }
    /* Sort points and remove duplicates (simplified logic) */
    for (p = 0; p < ptTotNb - 1; p++)
    {
        for (ptnb = p + 1; ptnb < ptTotNb; ptnb++)
        {
            if (ptCrib[p] > ptCrib[ptnb])
            {
                long temp = ptCrib[p];
                ptCrib[p] = ptCrib[ptnb];
                ptCrib[ptnb] = temp;
            }
        }
    }
    ptnb = 0;
    for (p = 1; p < ptTotNb; p++)
    {
        if (ptCrib[p] == ptCrib[ptnb])
        {
            memmove(&ptCrib[ptnb + 1], &ptCrib[ptnb + 2], sizeof(long) * (ptTotNb - ptnb - 2));
            ptTotNb--;
        }
        else
        {
            ptnb++;
        }
    }
    ptTotNb++;
    printf("\n");
    printf("------points of the sieve------\n");
    printf("POINTS OF THE SIEVE (ordered by their increasing abscissa)\n");
    printf("Rank ");
    for (p = 0; p < ptTotNb; p++)
    {
        if (p % 10 == 0)
        {
            printf("\n");
        }
        printf("%6ld ", ptCrib[p]);
    }
    printf("\n");
    printf("------compute the periods of the sieve------\n");
    memmove(ptReste, ptCrib, ptTotNb * sizeof(long));
    for (p = 0; p < ptTotNb; p++)
    {
        if (ptReste[p] == COVERED)
            continue;
        /* compute a period starting at current point */
        per = perCrib + perTotNb;
        per->mod = 0;
        do
        {
            per->mod++;
            per->ini = (short)(ptCrib[p] % (long)per->mod);
            per->couv = 0;
            ptval = per->ini;
            for (ptnb = 0; ptnb < ptTotNb && ptval <= ptCrib[ptnb]; ptnb++)
            {
                if (ptval == ptCrib[ptnb])
                {
                    per->couv++;
                    ptval += per->mod;
                }
            }
        } while (ptnb < ptTotNb);
        /* check the redundancy of the period */
        ptval = per->ini;
        flag = NON_REDUNDANT;
        for (ptnb = 0; ptnb < ptTotNb; ptnb++)
        {
            if (ptval == ptCrib[ptnb])
            {
                ptReste[ptnb] = COVERED;
                flag = NON_REDUNDANT;
                ptval += per->mod;
            }
        }
        if (flag == NON_REDUNDANT)
        {
            perCrib[perTotNb++] = *per;
        }
    }
    /* compute the period of the sieve */
    percrib = perCrib[0].mod;
    for (p = 1; p < perTotNb; p++)
    {
        if ((long)perCrib[p].mod != percrib)
        {
            percrib *= (long)perCrib[p].mod / Euclid((long)perCrib[p].mod, percrib);
            percrib *= (long)perCrib[p].mod / Euclid(percrib, (long)perCrib[p].mod);
        }
    }
    /* display the formula of the sieve */
    printf("FORMULA OF THE SIEVE:\n");
    printf("In each parenthesis are given in order:\n");
    printf("(modulus, starting point, number of covered points)\n\n");
    printf("L = ");
    for (p = 0; p < perTotNb; p++)
    {
        if (p != 0)
        {
            if (p % 3 == 0)
                printf("\n");
            printf("+ ");
        }
        printf("(%5d,%5d,%5d) ", perCrib[p].mod, perCrib[p].ini, perCrib[p].couv);
    }
    printf("\n\n period of the sieve: P = %6lu\n", percrib);
}

/* ============ Euclid's algorithm ============ */
unsigned long Euclid(unsigned long a1, unsigned long a2)
{
    unsigned long tmp;

    while ((tmp = a1 % a2) != 0)
    {
        a1 = a2;
        a2 = tmp;
    }
    return a2;
}