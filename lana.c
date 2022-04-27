/********* Moteur de tournoi : LANA ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"
#include <time.h>


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 

    
    
    clock_t t1, t2;
    t1 = clock();//lancement du chrono


	int i; 
	octet o, d; 
	octet myColor = currentPosition.trait; 
    
    

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));

    //joue un coups de la couleur adverse sur la couleur adverse, mais seulement pour les tours de 1 dans le but de ne pas nous saboter
	//-----------------------------------------------------//
    for(i=0;i<listeCoups.nb; ++i) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 


        if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor)) 
        {
            printf("On choisit ce coup ! \n"); 
            ecrireIndexCoup(i);
            break;
        }
	}

    //cherche si c'est possible de faire des tours de 5
    //-----------------------------------------------------//
    for(i=0;i<listeCoups.nb; ++i) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 


        if ((currentPosition.cols[o].couleur == myColor) && ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 5 )) 
        {
            printf("On choisit ce coup ! \n"); 
            ecrireIndexCoup(i);
            break;
        }
	}

    //si un pion a 1 seul voisin de notre couleur, alors prendre la tours
    //-----------------------------------------------------//
    for(i=0;i<listeCoups.nb; ++i) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 


        if ((currentPosition.cols[o].couleur == myColor) && (nbVoisins(o) == 1)) 
        {
            printf("On choisit ce coup ! \n"); 
            ecrireIndexCoup(i);
            break;
        }
	}



    t2 = clock();
    printf("temps = %f\n", (float)(t2-t1)/CLOCKS_PER_SEC);//fin du chrono 


}