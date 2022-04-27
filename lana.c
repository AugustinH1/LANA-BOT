/********* Moteur de tournoi : LANA ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"
#include <time.h>


//prototype des fonctions
void precoups(T_Position *currentPosition,T_ListeCoups *listeCoups);
void jouerbonus(T_Position *currentPosition,T_ListeCoups *listeCoups);


/*******************************FONCTION PRINCIPALE************************************/
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 

    
    
    clock_t t1, t2;
    t1 = clock();//lancement du chrono

    printf("la coups est %d\n", currentPosition.numCoup);

    if(currentPosition.numCoup <= 3)
    {
        jouerbonus(&currentPosition,&listeCoups);

    }
    if(currentPosition.numCoup > 3)
    {
        precoups(&currentPosition,&listeCoups);

    }





    t2 = clock();
    printf("temps = %f\n", (float)(t2-t1)/CLOCKS_PER_SEC);//fin du chrono 


}

//******************************************************************************/




/***************************FONCTION AUTRE****************************************/


//Choisit un coups avant le gros traitement
//
void precoups(T_Position *currentPosition,T_ListeCoups *listeCoups)
{
    int i; 
	octet o, d; 
	octet myColor = currentPosition->trait; 
    
    

	for(i=0;i<listeCoups->nb; ++i) {
        o = listeCoups->coups[i].origine; 
        d = listeCoups->coups[i].destination;  
        
        

        if ((currentPosition->cols[o].couleur == myColor) && (currentPosition->cols[d].couleur != myColor))
        {
            printf("On choisit ce coup:");
            printf("Coup %d : ", i); 
            printf("%d (%d - %s) ->", o, currentPosition->cols[o].nb, COLNAME(currentPosition->cols[o].couleur));
            printf("%d (%d - %s) \n", d, currentPosition->cols[d].nb, COLNAME(currentPosition->cols[d].couleur));
            ecrireIndexCoup(i);
            break;
        }
    }

    //joue un coups de la couleur adverse sur la couleur adverse, mais seulement pour les tours de 1 dans le but de ne pas nous saboter
	//-----------------------------------------------------//
    for(i=0;i<listeCoups->nb; ++i) {
		o = listeCoups->coups[i].origine; 
		d = listeCoups->coups[i].destination;  
		 

        if ((currentPosition->cols[o].couleur != myColor) && (currentPosition->cols[d].couleur != myColor)
            && (currentPosition->cols[o].nb==1) && (currentPosition->cols[d].nb==1)) 
        {
            printf("On choisit ce coup:");
            printf("Coup %d : ", i); 
            printf("%d (%d - %s) ->", o, currentPosition->cols[o].nb, COLNAME(currentPosition->cols[o].couleur));
            printf("%d (%d - %s) \n", d, currentPosition->cols[d].nb, COLNAME(currentPosition->cols[d].couleur));
            ecrireIndexCoup(i);
            break;
        }
	}

    //cherche si c'est possible de faire des tours de 5
    //-----------------------------------------------------//
    for(i=0;i<listeCoups->nb; ++i) {
		o = listeCoups->coups[i].origine; 
		d = listeCoups->coups[i].destination;  
		

        if ((currentPosition->cols[o].couleur == myColor) && ((currentPosition->cols[o].nb+currentPosition->cols[d].nb) == 5 )) 
        {
            printf("On choisit ce coup:");
            printf("Coup %d : ", i); 
            printf("%d (%d - %s) ->", o, currentPosition->cols[o].nb, COLNAME(currentPosition->cols[o].couleur));
            printf("%d (%d - %s) \n", d, currentPosition->cols[d].nb, COLNAME(currentPosition->cols[d].couleur));
            ecrireIndexCoup(i);
            break;
        }
	}

    //si un pion a 1 seul voisin de notre couleur, alors prendre la tours
    //-----------------------------------------------------//
    for(i=0;i<listeCoups->nb; ++i) {
		o = listeCoups->coups[i].origine; 
		d = listeCoups->coups[i].destination;  
		

        if ((currentPosition->cols[o].couleur == myColor) && (nbVoisins(o) == 1)) 
        {
            printf("On choisit ce coup:");
            printf("Coup %d : ", i); 
            printf("%d (%d - %s) ->", o, currentPosition->cols[o].nb, COLNAME(currentPosition->cols[o].couleur));
            printf("%d (%d - %s) \n", d, currentPosition->cols[d].nb, COLNAME(currentPosition->cols[d].couleur));
            ecrireIndexCoup(i);
            break;
        }
	}
 

}



void jouerbonus(T_Position *currentPosition,T_ListeCoups *listeCoups)
{
    int i; 
	octet o, d; 
	octet myColor = currentPosition->trait; 
    printf("fonction jouerbonus\n");


    for(i=0;i<listeCoups->nb; ++i)
    {
        o = listeCoups->coups[i].origine; 
        d = listeCoups->coups[i].destination;  
    

        if (currentPosition->numCoup == 0 && o == 28 && d == 28 ) 
        {
            ecrireIndexCoup(i);
            break;
        }
        if (currentPosition->numCoup == 1 && o == 14 && d == 14 ) 
        {
    
            ecrireIndexCoup(i);
            break;
        }

        /*if (currentPosition->numCoup == 2 && o == 14 && d == 14 ) 
        {
    
            ecrireIndexCoup(i);
            break;
        }

        if (currentPosition->numCoup == 2 && o == 14 && d == 14 ) 
        {
            ecrireIndexCoup(i);
            break;
        }
        */

	}



}



/////// Fonctions utilitaires

// Permet d'évaluer le gain partant de currentPosition selon le coup joué.
octet getGain(const T_Position currentPosition, T_Coup coup) {
    octet origColor = currentPosition.cols[coup.origine].couleur;
    octet destColor = currentPosition.cols[coup.destination].couleur;

    octet origBonus = 0;
    octet destBonus = 0;

    origBonus += ((coup.origine == currentPosition.evolution.bonusJ) + (coup.origine == currentPosition.evolution.bonusR));
    origBonus -= ((coup.origine == currentPosition.evolution.malusJ) + (coup.origine == currentPosition.evolution.malusR));

    destBonus += ((coup.destination == currentPosition.evolution.bonusJ) + (coup.destination == currentPosition.evolution.bonusR));
    destBonus -= ((coup.destination == currentPosition.evolution.malusJ) + (coup.destination == currentPosition.evolution.malusR)); 

    //origBonus *= -(origColor != currentPosition.trait);
    //destBonus *= -(destColor != currentPosition.trait);

    octet gain = 0;

    gain += 1-2*(currentPosition.trait == destColor) + origBonus + destBonus;

    gain -= (1+origBonus)*-(origColor != currentPosition.trait) + (1+destBonus)*-(destColor != currentPosition.trait);

    printf("Le gain du coup choisi est évalué à  %d", gain);

    return gain;
}
