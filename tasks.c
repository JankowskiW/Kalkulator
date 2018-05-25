#include "tasks.h"

// NIE ZAPOMINAJ O ZNAKU NOWEJ LINII NA KONCU
// OSTATNIEJ LINIJKI W PLIKU Z LICZBAMI!

// Z - oznaczenie calkowitych
// F - Fractional Part, czesc ulamkowa
void sum(char* path)
{   lli longestNum=0, longestZ=0, longestF=0, numOfLines=0, lengthLine, actualZ, actualF, tmp, i, j,
        **numbers=NULL, *sumTable, sumLen=0;
    char line[BUFLEN], *dotPtr;
    FILE *fp;
    ofr(fp,path);

    /* ################################ MODUL PRZEPISYWANIA LICZB ############################ */
    // ===================================== ILE LICZB =========================================
    // PETLA SPRAWDZA ILE W PLIKU JEST LICZB
    // I JAKA JEST NAJDLUZSZA CZESC CALKOWITA
    // I ULAMKOWA Z TYCH LICZB
    // ZNAJDUJE SIE TU TAKZE OBSLUGA BLEDOW
    while(fgets(line,BUFLEN,fp)!=NULL)
    {   numOfLines++;
        dotPtr=strchr(line,'.');
        lengthLine = strlen(line)-1;
        if(dotPtr!=NULL) // ROZWAZANY JESLI AKTUALNIE CZYTANA JEST LICZBA TYPU 11.24 (Z KROPKA)
        {   if((dotPtr==line)||(dotPtr==(line+lengthLine-1))) // JESLI UZYTKOWNIK WPROWADZI .123 LUB 123.
            {   fprintf(stderr,"Bledne dane wejsciowe.\nLinia nr. %d",numOfLines);
                return;
            }
            tmp=(lli)dotPtr-(lli)line;
            if(tmp>longestZ) longestZ = tmp;
            tmp=lengthLine-tmp-1;
            if(tmp>longestF) longestF = tmp;
        }else // ROZWAZANE JESLI AKTUALNIE CZYTANA JEST LICZBA TYPU 11 (BEZ KROPKI)
        {   if(lengthLine>longestZ) longestZ = lengthLine;
        }
    }
    longestNum = longestZ + longestF; // CZY POTRZEBNE?
    // JESLI UZYTKOWNIK WPROWADZI MNIEJ NIZ DWIE LICZBY
    if (numOfLines<2)
    {   fprintf(stderr,"Bledne dane wejsciowe.\nNalezy podac conajmniej dwie liczby.");
        return;
    }
    fseek(fp,0,SEEK_SET); // USTAWIA WSKAZNIK NA POCZATU PLIKU

    // ============================= ALOKACJA TABLICY LICZB =====================================
    numbers = (lli**)malloc(numOfLines*sizeof(lli));
    for(i=0; i<numOfLines; i++)
    {   *(numbers+i)=(lli*)malloc(longestNum*sizeof(lli));
        for(j=0; j<longestNum; j++) numbers[i][j] = 0;
    }

    // ==================== PRZEPISYWANIE LICZB Z PLIKU DO TABLICY numbers =======================
    j=0;
    while(fgets(line,BUFLEN,fp)!=NULL)
    {   dotPtr = strchr(line,'.');
        lengthLine = strlen(line)-1;
        if(dotPtr!=NULL)
        {   actualZ = (lli)dotPtr - (lli)line;
            actualF = lengthLine-actualZ-1;
        }else
        {   actualZ = lengthLine;
            actualF = 0;
        }
        for(i=(longestZ-1); i>=(longestZ-actualZ); i--) // WYPISUJE CZESC CALKOWITA
            numbers[j][i]=line[i-(longestZ-actualZ)]-'0';
        for(i=longestZ; i<(longestZ+actualF); i++) // WYPISUJE CZESC ULAMKOWA !!!!!!!! TA PETLA MA BLAD
            numbers[j][i]=line[i+actualZ-longestZ+1]-'0';
        j++;
    }
    // ================================ ZAMYKANIE PLIKU =========================================
    if(fclose(fp)!=0)
        fprintf(stderr,"Plik %s nie zostal poprawnie zamkniety.",path);

    printf("\n\n LICZBY W TABLICY:\n");
    for(i=0; i<numOfLines; i++)
    {
        printf("\n");
        for(j=0; j<longestNum; j++)
        {
            printf(" %d",numbers[i][j]);
        }
    }
    printf("\n");
    /* ##################################### MODUL SUMOWANIA #################################### */
    // ================================ ALOKOWANIE TABLICY DO SUMOWANIA ============================
    sumTable = (lli*)malloc(longestNum*sizeof(lli));
    sumLen=longestNum;
    for(i=0; i<longestNum; i++) sumTable[i]=numbers[numOfLines-1][i]; // PRZEPISYWANIE OST. LICZBY
    printf("\nSUM%d = ",sumLen-1);
    for(i=0; i<sumLen; i++) printf("%d ",sumTable[i]);
    // ================================== PETLA SUMUJACA ============================================
    // -2 ZE WZGLEDU NA TO ZE OST INDEKS TO NUMOFLINES-1 A DRUGIE -1, BO OD PRZEDOST. WIERSZA
    for(i=(numOfLines-2); i>=0; i--)
    {   for(j=(sumLen-1); j>(sumLen-longestNum); j--)
        {   tmp=j-(sumLen-longestNum);
            sumTable[j]+=numbers[i][tmp];
            if(sumTable[j]>9)
            {   sumTable[j]-=10;
                sumTable[j-1]+=1;
            }
        }
        // DLA OSTATNIEGO ELEMENTU DZIALA INACZEJ, BO GDY PRZENOSIMY 1 TO MUSIMY POWIEKSZYC TABLICE SUM
        sumTable[j]+=numbers[i][0];
        for(;j>0; j--)
        {   if(sumTable[j]>9)
            {   sumTable[j]-=10;
                sumTable[j-1]+=1;
            }
        }
        if(sumTable[0]>9)
        {   sumLen++;
            sumTable = (lli*)realloc(sumTable,(sumLen*sizeof(lli)));
            for(j=(sumLen-1); j>0; j--) sumTable[j]=sumTable[j-1];
            sumTable[1]-=10;
            sumTable[0]=1;
        }

        printf("\nSUM%d = ",i);
        for(j=0; j<sumLen; j++) printf("%d ",sumTable[j]);
    }

    /* ########################## MODUL FORMATOWANIA I ZAPISYWANIA DO PLIKU ##########################*/
    // =================================== USUWANIE NIEZNACZACYCH ZER =================================
    // Z POCZATKU
    i=0;
    while((sumTable[i]==0)&&(i<longestZ-1)) i++;
    longestZ-=i;
    for(j=0; j<(sumLen-i); j++) sumTable[j]=sumTable[j+i];
    sumLen-=i;
    sumTable=(lli*)realloc(sumTable,(sumLen*sizeof(lli)));

    // Z KONCA
    printf("\nLONGESTF = %d",longestF);
    if(longestF>0)
    {
        i=sumLen-1;
        j=0;
        while((sumTable[i]==0)&&(j<longestF))
        {   i--; j++; }
        longestF-=j;
        sumLen -= j;
        sumTable=(lli*)realloc(sumTable,(sumLen*sizeof(lli)));
            printf("\nSUMbez0 = ");
            for(j=0; j<sumLen; j++) printf("%d ",sumTable[j]);
    }

    // =================================== ZAPISYWANIE WYNIKU DO PLIKU ================================
    ofw(fp,"suma.txt");
    for(i=0; i<(sumLen-longestF); i++)
    {   printf("\nZapisuje: %d",sumTable[i]);
        fprintf(fp,"%d",sumTable[i]);

    }

    if(longestF>0)
        fprintf(fp,".");
    for(i=(sumLen-longestF);i<sumLen; i++)
    {

       printf("\nZapisuje: %d",sumTable[i]);
        fprintf(fp,"%d",sumTable[i]);
    }
    if(fclose(fp)!=0)
    {   fprintf(stderr,"Plik wynikowy nie zostal poprawnie zamkniety\nzmiany nie zostana zapisane.");
    }else
    {   printf("\n\naWynik zostal zapisany w pliku suma.txt"); }

}

int menu()
{   size_t task;
    system("CLS");
    printf("+-------------------- M E N U ------------------+\n");
    printf("|  1. Dodawanie pisemne bardzo duzych liczb     |\n");
    printf("|  2. Odejmowanie pisemne bardzo duzych liczb   |\n");
    printf("| 10. Instrukcja obslugi programu               |\n");
    printf("+-----------------------------------------------+\n");
    printf("\nPodaj numer zadania: "); scanf("%ld",&task);
    system("CLS");
    return task;
}

void instructions()
{   printf("\nInstrukcja obslugi programu.\n");
    printf("NA KONCU PLIKU W OSTATNIEJ LINIJCE ZAWSZE ENTER.");
}

