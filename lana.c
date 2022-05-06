/********* Moteur de tournoi : LANA ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"
#include <time.h>

// Structures
typedef struct 
{
    int initialDepth;
    int lanaCouleur;

} T_Data;


//prototype des fonctions
void precoups(T_Position *currentPosition,T_ListeCoups *listeCoups, T_Coup *coupFinal);
void jouerbonus(T_Position *currentPosition,T_ListeCoups *listeCoups);

T_Position refreshPosition(const T_Position currPos, T_Coup coup);
int getEvaluation(const T_Position currPos, T_ListeCoups liste, int lanaCouleur);
int minmax(T_Position pos, int depth, T_Data data);

T_Voisins getCurrentVoisins(const T_Position position, const octet posPion, octet color);


/*******************************FONCTION PRINCIPALE************************************/
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 

    
    
    clock_t t1, t2;
    t1 = clock();//lancement du chrono

    T_Coup coup;

    printf("\n\n --- [ TOUR %d - A LANA DE JOUER] --- \n\n", currentPosition.numCoup);

    if(currentPosition.numCoup <= 3)
    {
        jouerbonus(&currentPosition, &listeCoups);

    }
    if(currentPosition.numCoup > 3)
    {
        precoups(&currentPosition,&listeCoups, &coup);

        printf("[PRECOUP] Préselection du coup: %d -> %d\n", coup.origine, coup.destination);

        int initialDepth = 6;

         if(currentPosition.numCoup < 35)
             initialDepth = 8;

        printf("Commentcement du calcul... | Nombre de coups : %d | Depth : %d\n", listeCoups.nb, initialDepth);

        T_Data data;
        data.lanaCouleur = currentPosition.trait;
        data.initialDepth = initialDepth;
        int idCoup = minmax(currentPosition, initialDepth, data);

        printf("[MINMAX] id du Coup : %d\n", idCoup);
        ecrireIndexCoup(idCoup);
    }





    t2 = clock();
    printf("\nTemps écoulé pour ce tour: %f\n\n --- [ FIN DE TOUR ] ---\n\n", (float)(t2-t1)/CLOCKS_PER_SEC);//fin du chrono 


}

//******************************************************************************/




/***************************FONCTION AUTRE****************************************/


//Choisit un coups avant le gros traitement
//
void precoups(T_Position *currentPosition,T_ListeCoups *listeCoups, T_Coup *coupFinal)
{


   
    int i; 
	octet o, d; 
	octet myColor = currentPosition->trait;

    octet tmpmalus;

    if(myColor==ROU)
        tmpmalus=currentPosition->evolution.malusJ;
    else
        tmpmalus=currentPosition->evolution.malusR;

    
    
    //joue un coups de la couleur adverse sur la couleur adverse, mais seulement pour les tours de 1 dans le but de ne pas nous saboter
	//-----------------------------------------------------//
    for(i=0;i<listeCoups->nb; ++i) {
		o = listeCoups->coups[i].origine; 
		d = listeCoups->coups[i].destination;  
		 

        if ((currentPosition->cols[o].couleur != myColor) && (currentPosition->cols[d].couleur != myColor)
            && (currentPosition->cols[o].nb==1) && (currentPosition->cols[d].nb==1)) 
        {
           
            // printf("On choisit ce coup:");
            // printf("Coup %d : ", i); 
            // printf("%d (%d - %s) ->", o, currentPosition->cols[o].nb, COLNAME(currentPosition->cols[o].couleur));
            // printf("%d (%d - %s) \n", d, currentPosition->cols[d].nb, COLNAME(currentPosition->cols[d].couleur));
            ecrireIndexCoup(i);
            *coupFinal = listeCoups->coups[i];
            break;
        }
	}
    
    //joue un pion de notre couleur sur la couleur adverse
    //-----------------------------------------------------//
	for(i=0;i<listeCoups->nb; ++i) {
        o = listeCoups->coups[i].origine; 
        d = listeCoups->coups[i].destination;  
        
        

        if ((currentPosition->cols[o].couleur == myColor) && (currentPosition->cols[d].couleur != myColor)
            && (currentPosition->cols[o].nb==1) && (currentPosition->cols[d].nb==1) && (d != tmpmalus))
        {
            // printf("On choisit ce coup:");
            // printf("Coup %d : ", i); 
            // printf("%d (%d - %s) ->", o, currentPosition->cols[o].nb, COLNAME(currentPosition->cols[o].couleur));
            // printf("%d (%d - %s) \n", d, currentPosition->cols[d].nb, COLNAME(currentPosition->cols[d].couleur));
            ecrireIndexCoup(i);
            *coupFinal = listeCoups->coups[i];
            break;
        }
    }

  


    //enpiler un pion rouge sur 2 pions jaune
    //-----------------------------------------------------//
    for(i=0;i<listeCoups->nb; ++i) {
		o = listeCoups->coups[i].origine; 
		d = listeCoups->coups[i].destination;  
		 

        if ((currentPosition->cols[o].couleur == myColor) && (currentPosition->cols[d].couleur != myColor)
            && (currentPosition->cols[o].nb==1) && (currentPosition->cols[d].nb==2) && (d != tmpmalus)) 
        {
            // printf("On choisit ce coup:");
            // printf("Coup %d : ", i); 
            // printf("%d (%d - %s) ->", o, currentPosition->cols[o].nb, COLNAME(currentPosition->cols[o].couleur));
            // printf("%d (%d - %s) \n", d, currentPosition->cols[d].nb, COLNAME(currentPosition->cols[d].couleur));
            ecrireIndexCoup(i);
            *coupFinal = listeCoups->coups[i];
            break;
        }
	}


    //cherche si c'est possible de faire des tours de 5
    //-----------------------------------------------------//
    for(i=0;i<listeCoups->nb; ++i) {
		o = listeCoups->coups[i].origine; 
		d = listeCoups->coups[i].destination;  
		
    
        if ((currentPosition->cols[o].couleur == myColor) && ((currentPosition->cols[o].nb+currentPosition->cols[d].nb) == 5 )  && (d != tmpmalus)) 
        {
            // printf("On choisit ce coup:");
            // printf("Coup %d : ", i); 
            // printf("%d (%d - %s) ->", o, currentPosition->cols[o].nb, COLNAME(currentPosition->cols[o].couleur));
            // printf("%d (%d - %s) \n", d, currentPosition->cols[d].nb, COLNAME(currentPosition->cols[d].couleur));
            ecrireIndexCoup(i);
            *coupFinal = listeCoups->coups[i];
            break;
        }
	}

    //si un pion a 1 seul voisin de notre couleur, alors prendre la tours
    //-----------------------------------------------------//
    // for(i=0;i<listeCoups->nb; ++i) {
	// 	o = listeCoups->coups[i].origine; 
	// 	d = listeCoups->coups[i].destination;  

    //     octet nbVoisins = getCurrentVoisins(*currentPosition, d, 0).nb;
		

    //     if ((currentPosition->cols[o].couleur == myColor) && (nbVoisins == 1) && (d != tmpmalus)) 
    //     {
    //         // printf("On choisit ce coup:");
    //         // printf("Coup %d : ", i); 
    //         // printf("%d (%d - %s) ->", o, currentPosition->cols[o].nb, COLNAME(currentPosition->cols[o].couleur));
    //         // printf("%d (%d - %s) \n", d, currentPosition->cols[d].nb, COLNAME(currentPosition->cols[d].couleur));
    //         ecrireIndexCoup(i);
    //         *coupFinal = listeCoups->coups[i];
    //         break;
    //     }
	// }
 

}



void jouerbonus(T_Position *currentPosition,T_ListeCoups *listeCoups)
{
    int i; 
	octet o, d; 
    printf("fonction jouerbonus\n");


    for(i=0;i<listeCoups->nb; ++i)
    {
        o = listeCoups->coups[i].origine; 
        d = listeCoups->coups[i].destination;  
    

        //SI NOTRE COULEUR = JAUNE

        //placement bonus jaune
        if (currentPosition->numCoup == 0 && o == 28 && d == 28 ) 
        {
            ecrireIndexCoup(i);
            break;
        }
        //placement malus jaune
        if (currentPosition->numCoup == 2) 
        {
            T_Voisins Voisins = getVoisins(o);
            
            // printf(" %d le nombre de Voisin est:%d\n",i,Voisins.nb);

            for (int j = 0; j < Voisins.nb; ++j)
            {                
                if ( Voisins.cases[j]==currentPosition->evolution.bonusR)
                {
                    // printf("Coup %d : ", i); 
                    // printf("%d (%s) ->", o, COLNAME(currentPosition->cols[o].couleur));
                    ecrireIndexCoup(i);
                    break;
                }
            }
        }

    



        //SI NOTRE COULEUR = ROUGE

        //placement bonus rouge
        if (currentPosition->numCoup == 1 && o == 14 && d == 14 ) 
        {
    
            ecrireIndexCoup(i);
            break;
        }
        //placement malus rouge
        if (currentPosition->numCoup == 3) 
        {
            T_Voisins Voisins = getVoisins(o);
            
            // printf(" %d le nombre de Voisin est:%d\n",i,Voisins.nb);

            for (int j = 0; j < Voisins.nb; ++j)
            {                
                if ( Voisins.cases[j]==currentPosition->evolution.bonusJ)
                {
                    // printf("Coup %d : ", i); 
                    // printf("%d (%s) ->", o, COLNAME(currentPosition->cols[o].couleur));
                    ecrireIndexCoup(i);
                    break;
                }
            }
        }
	}



}

// Minmax oskour
int minmax(T_Position pos, int depth, T_Data data) {
    int eval = 0;
    int optiCoup;
    T_ListeCoups liste = getCoupsLegaux(pos);
    T_Position child;

    if(depth == 0 || liste.nb == 0) {
        eval += getEvaluation(pos, liste, data.lanaCouleur);
        return eval;
    }

    if(pos.trait == data.lanaCouleur) {
        // MAXIMIZING
        eval = -1000;
        for(int idCoup = 0; idCoup < liste.nb; idCoup++) {
            child = refreshPosition(pos, liste.coups[idCoup]);
            int value = minmax(child, depth-1, data);
            if(value > eval) {
                optiCoup = idCoup;
                eval = value;
            }
        }
        if(depth == data.initialDepth) {
            printf("Depth : %d | Eval : %d | Lana : %d\n", depth, eval, data.lanaCouleur == child.trait);
            return optiCoup;
        }
    } else {
        // MINIMIZING
        eval = 1000;
        for(int idCoup = 0; idCoup < liste.nb; idCoup++) {
            child = refreshPosition(pos, liste.coups[idCoup]);
            int value = minmax(child, depth-1, data);
            if(value < eval) {
                eval = value;
                // if(depth == data.initialDepth-1)
                    //printf("Depth : %d | Eval : %d\n", depth, eval);
            }
        }
        // if(depth == data.initialDepth) {
        //     printf("Depth : %d | Eval : %d | Lana : %d\n", depth, eval, data.lanaCouleur == child.trait);
        //     return optiCoup;
        // }
    }

    // printf("Depth : %d | Eval : %d | Lana : %d\n", depth, eval, data.lanaCouleur == child.trait);

    return eval;

}

/////// Fonctions utilitaires

// Permet d'évaluer le gain partant de currentPosition selon le coup joué.
// int getGain(const T_Position currentPosition, T_Coup coup) {
//     int origColor = currentPosition.cols[coup.origine].couleur;
//     int destColor = currentPosition.cols[coup.destination].couleur;

//     int origBonus = 0;
//     int destBonus = 0;

//     origBonus += ((coup.origine == currentPosition.evolution.bonusJ) + (coup.origine == currentPosition.evolution.bonusR));
//     origBonus -= ((coup.origine == currentPosition.evolution.malusJ) + (coup.origine == currentPosition.evolution.malusR));

//     destBonus += ((coup.destination == currentPosition.evolution.bonusJ) + (coup.destination == currentPosition.evolution.bonusR));
//     destBonus -= ((coup.destination == currentPosition.evolution.malusJ) + (coup.destination == currentPosition.evolution.malusR)); 

//     //origBonus *= -(origColor != currentPosition.trait);
//     //destBonus *= -(destColor != currentPosition.trait);

//     int gain = 0;

//     gain += 1-2*(currentPosition.trait == destColor) + origBonus + destBonus;

//     gain -= (1+origBonus)*-(origColor != currentPosition.trait) + (1+destBonus)*-(destColor != currentPosition.trait);

//     printf("Le gain du coup choisi est évalué à  %d", gain);

//     return gain;
// }

// Permet de réaliser une évaluation
int getEvaluation(const T_Position currPos, T_ListeCoups liste, int lanaCouleur)
{
    int eval = 0;

    if(currPos.cols[currPos.evolution.bonusJ].couleur == lanaCouleur) {
        eval += 1;
    } else {
        eval -= 1;
    }
    if(currPos.cols[currPos.evolution.bonusR].couleur == lanaCouleur) {
        eval += 1;
    } else {
        eval -= 1;
    }
    if(currPos.cols[currPos.evolution.malusJ].couleur == lanaCouleur) {
        eval -= 1;
    } else {
        eval += 1;
    }
    if(currPos.cols[currPos.evolution.malusR].couleur == lanaCouleur) {
        eval -= 1;
    } else {
        eval += 1;
    }

    if(currPos.evolution.bonusR == currPos.evolution.bonusJ) {
        if(currPos.cols[currPos.evolution.bonusJ].couleur == lanaCouleur) {
            eval += 10;
        } else {
            eval -= 10;
        }
    }

    for(int i = 0; i < 48; i++) {
        T_Colonne col = currPos.cols[i];
        T_Voisins voisins = getCurrentVoisins(currPos, col.nb, 0);  

        if(col.nb == 0) {
            continue;
        }

        if(col.couleur == lanaCouleur) {
            eval += 10;
        } else {
            eval += -10;
        } 

        if(voisins.nb == 0) {
            if(col.couleur == lanaCouleur) {
                eval += 18;
            } else {
                eval += -18;
            }
        } 

        // if(voisins.nb == 1) {
        //     if(col.couleur != lanaCouleur) {
        //         if(col.nb + voisins.cases[0] > 5) {
        //             eval += 22;
        //         }
        //     }
        // } 

        if(col.nb == 5) {
            if(col.couleur == lanaCouleur) {
                eval += 5;
            } else {
                eval += -5;
            }
        }
    }

    return eval;
}

// Permet de créer une nouvelle position où le coup a été joué.
T_Position refreshPosition(T_Position currPos, T_Coup coup)
{
    octet ori = coup.origine;
    octet dest = coup.destination;

    // Déplacement du pion joué
    currPos.cols[dest].nb += currPos.cols[ori].nb;
    currPos.cols[ori].nb = 0;
    currPos.cols[dest].couleur = currPos.cols[ori].couleur;

    // Update de la couleur

    currPos.trait = (currPos.trait == ROU ? JAU : ROU);

    // Update des pions évolution
    if (currPos.evolution.bonusJ == ori)
    {
        currPos.evolution.bonusJ = dest;
    }
    else
    {
        currPos.evolution.bonusJ = currPos.evolution.bonusJ;
    }

    if (currPos.evolution.bonusR == ori)
    {
        currPos.evolution.bonusR = dest;
    }
    else
    {
        currPos.evolution.bonusR = currPos.evolution.bonusR;
    }

    if (currPos.evolution.malusJ == ori)
    {
        currPos.evolution.malusJ = dest;
    }
    else
    {
        currPos.evolution.malusJ = currPos.evolution.malusJ;
    }

    if (currPos.evolution.malusR == ori)
    {
        currPos.evolution.malusR = dest;
    }
    else
    {
        currPos.evolution.malusR = currPos.evolution.malusR;
    }

    return currPos;
}

// Permet d'obtenir les différents voisins d'un pion, mais s'il n'y aucun pion sur la position, cela ne le comptera pas comme un 
// voisin, à l'instar de getVoisins().
/// posPion : position du pion dont on veut les voisins
/// color : Couleur des voisins souhaitée (0: tout ; 1: jaune; 2: rouge)
T_Voisins getCurrentVoisins(const T_Position position, const octet posPion, octet color) {
    T_Voisins voisins = getVoisins(posPion);

    for(octet i=0; i<voisins.nb; i++) {
        if(position.cols[voisins.cases[i]].nb == 0 || position.cols[voisins.cases[i]].nb == 5 || (color != 0 && position.cols[voisins.cases[i]].couleur != color)) {
            voisins.cases[i] = voisins.cases[voisins.nb];
            voisins.nb--; 
            i--;  
        }
    }

    return voisins;
}
