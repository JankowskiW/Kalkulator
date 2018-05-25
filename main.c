#include "tasks.h"

int main(int argc, char *argv[])
{   char again='t';
    int task;
    while(again=='t')
    {   task = menu();
        switch(task)
        {   case 1:
            {   printf("\nPAMIETAJ, ZE KURSOR | W PLIKU MUSI ZNALEZC JEDNA LINIJKE POD OSTATNIA LICZBA.\n");
                char path[PTHLEN];
                if(argc>1)
                { int i=0;
                  while(path[i]=argv[1][i]) i++;
                }else
                { printf("Podaj sciezke do pliku: ");
                  fscanf(stdin,"%s",path);
                }
                sum(path);
            }break;
            case 2:
            {
            }break;
            case 10:
            { instructions();
            }break;
          default:
            { printf("\nNie ma takiego zadania.\n");
              printf("Wybierz numer 1-10\n");
            }
        }
        printf("\nCzy chcesz powrocic do menu? [t/n]: ");
        fflush(stdin);
        scanf("%c",&again);
    }
    return 0;
}
