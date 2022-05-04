#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

#define couleurAdverse(couleur) (((couleur)==(2)) ? (1) : (2)) 
#define MalusTour(malus1, malus2, origine, destination) ((malus1==origine || malus1==destination || malus2==origine || malus2==destination) ? 0 : 1)
#define BonusTour(bonus1, bonus2, origine, destination) ((bonus1==origine || bonus1==destination || bonus2==origine || bonus2==destination) ? 0 : 1)

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
    // Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
    // Pour sélectionner l'index d'un coup à jouer dans la liste l 

    octet myColor=currentPosition.trait;
    octet o,
    d;
    int pos;
    int ok=1;

    //Placement des bonus et malus

    if (myColor==1) {
        if (currentPosition.numCoup==0) //Le joueur jaune doit placer son bonus

            {
            //currentPosition.evolution.bonusJ=32;
            ecrireIndexCoup(16);
            return;
        }

        if (currentPosition.numCoup==2) //Le joueur doit placer son malus

            {
            //currentPosition.evolution.malusJ=15;
            ecrireIndexCoup(7);
            return;
        }
    }

    else {
        if (currentPosition.numCoup==1) //Le joueur rouge doit placer son bonus

            {
            T_Voisins voisin_bonus_jaune=getVoisins(currentPosition.evolution.bonusJ);

            //currentPosition.evolution.bonusR=voisin_bonus_jaune.cases[0]; //bonusR voisin du bonusJ

            for(int i=0; i<listeCoups.nb; i++) {
                o=listeCoups.coups[i].origine;

                for(int j=0; j<voisin_bonus_jaune.nb; j++) {
                    if(o==voisin_bonus_jaune.cases[j]) {
                        ecrireIndexCoup(i);
                        return;
                    }
                }
            }
        }

        if (currentPosition.numCoup==3) //Le joueur rouge doit placer son malus

            {
            if (currentPosition.evolution.bonusJ<24) {

                //currentPosition.evolution.malusR = currentPosition.evolution.bonusJ +24;
                for(int i=0; i<listeCoups.nb; i++) {
                    o=listeCoups.coups[i].origine;

                    if(o==(currentPosition.evolution.bonusJ+24)) {
                        ecrireIndexCoup(i);
                        return;
                    }
                }
            }

            else {

                //currentPosition.evolution.malusR = currentPosition.evolution.bonusJ -24;
                for(int i=0; i<listeCoups.nb; i++) {
                    o=listeCoups.coups[i].origine;

                    if(o==(currentPosition.evolution.bonusJ-24)) {
                        ecrireIndexCoup(i);
                        return;
                    }
                }
            }
        }
    }

    // Premier coup à jouer selon notre couleur
    if (listeCoups.nb >=283) {
        if(myColor==2) {
            if (estValide(currentPosition, 7, 3)==1) // Si le coup n'a pas déjà été joué par le joueur jaune

                {
                pos=0;
                o=listeCoups.coups[pos].origine;
                d=listeCoups.coups[pos].destination;

                while ( !(o==7 && d==3)) {
                    pos++;
                    o=listeCoups.coups[pos].origine;
                    d=listeCoups.coups[pos].destination;
                }

                ecrireIndexCoup(pos);
                return;
            }

            else {
                pos=listeCoups.nb;
                o=listeCoups.coups[pos].origine;
                d=listeCoups.coups[pos].destination;

                while ( !(o==46 && d==44)) {
                    o=listeCoups.coups[pos].origine;
                    d=listeCoups.coups[pos].destination;
                    pos--;
                }

                ecrireIndexCoup(pos);
                return;
            }
        }

        else // Si on est jaune, on joue en premier donc le coup 0-4 est forcèment disponible et en 3ème position

            {
            ecrireIndexCoup(3);
            //ecrireCoup(0,4);
            return;
        }
    }

    T_Voisins voisins_destination;
    T_Voisins voisins_origine;
    T_Voisins voisins_du_voisin_origine;

    octet somme_tours;

    for(int i=0; i<listeCoups.nb; i++) {

        o=listeCoups.coups[i].origine;
        d=listeCoups.coups[i].destination;
        voisins_destination=getVoisins(d);
        voisins_origine=getVoisins(o);
        somme_tours=currentPosition.cols[d].nb+currentPosition.cols[o].nb;



        // Si je peux gagner une colonne, je la prends 
        if ((currentPosition.cols[o].couleur==myColor) && (somme_tours==5) && MalusTour(currentPosition.evolution.malusJ, currentPosition.evolution.malusR, o, d)) {
            printf("On prend une tour de 5 ! \n");
            ecrireIndexCoup(i);
            return; // on quitte la fonction 
        }

        if ((currentPosition.cols[o].couleur==myColor) && (somme_tours==5) && !MalusTour(currentPosition.evolution.malusJ, currentPosition.evolution.malusR, o, d) && !BonusTour(currentPosition.evolution.bonusJ, currentPosition.evolution.bonusR, o, d)) {
            printf("On prend une tour de 5 ! (avec bonus et malus) \n");
            ecrireIndexCoup(i);
            return; // on quitte la fonction 
        }

        if((voisins_origine.nb==1 && voisins_destination.nb==1) && (currentPosition.cols[o].couleur==myColor && currentPosition.cols[d].couleur==couleurAdverse(myColor)) && MalusTour(currentPosition.evolution.malusJ, currentPosition.evolution.malusR, o, d)) {
            printf("Je prend la tour isolée\n");
            ecrireIndexCoup(i);
            return;
        }
    }

    for(int i=0; i<listeCoups.nb; i++) {
        o=listeCoups.coups[i].origine;
        d=listeCoups.coups[i].destination;
        ok=1;
        somme_tours=currentPosition.cols[d].nb+currentPosition.cols[o].nb;

        voisins_destination=getVoisins(d);
        voisins_origine=getVoisins(o);

        // règle2 cas particulier
        for(int k=0; k < voisins_destination.nb; k++) {
            if ((currentPosition.cols[o].couleur==couleurAdverse(myColor)) || (((somme_tours + currentPosition.cols[voisins_destination.cases[k]].nb)<=5) && (currentPosition.cols[voisins_destination.cases[k]].couleur==myColor)) || (((somme_tours + currentPosition.cols[voisins_destination.cases[k]].nb)<=5) && (currentPosition.cols[voisins_destination.cases[k]].couleur==couleurAdverse(myColor)))) {
                ok=0;
                break;
            }
        }

        if(ok && MalusTour(currentPosition.evolution.malusJ, currentPosition.evolution.malusR, o, d)) {
            printf("On crée une tour (de 5 ou moins) qui ne pourra pas être directement prise par l'adversaire \n");
            ecrireIndexCoup(i);
            return;
        }

        if(ok && !MalusTour(currentPosition.evolution.malusJ, currentPosition.evolution.malusR, o, d) && !BonusTour(currentPosition.evolution.bonusJ, currentPosition.evolution.bonusR, o, d)) {
            printf("On crée une tour (de 5 ou moins) qui ne pourra pas être directement prise par l'adversaire avec bonus et malus \n");
            ecrireIndexCoup(i);
            return;
        }

        // règle2 + règle5
        ok=1;
        int coup_tampon=((voisins_origine.nb==1 && voisins_destination.nb==1) && (currentPosition.cols[o].couleur==currentPosition.cols[d].couleur));

        for(int k=0; k < voisins_destination.nb; k++) {
            if ((currentPosition.cols[voisins_destination.cases[k]].couleur==couleurAdverse(myColor) && currentPosition.cols[voisins_destination.cases[k]].nb !=5) || coup_tampon) {
                ok=0;
                break;
            }
        }

        if(ok && MalusTour(currentPosition.evolution.malusJ, currentPosition.evolution.malusR, o, d)) {
            printf("On crée une tour (peu importe la hauteur) avec que des voisins de notre couleur \n");
            ecrireIndexCoup(i);
            return;
        }

        if(ok && !MalusTour(currentPosition.evolution.malusJ, currentPosition.evolution.malusR, o, d) && !BonusTour(currentPosition.evolution.bonusJ, currentPosition.evolution.bonusR, o, d)) {
            printf("On crée une tour (peu importe la hauteur) avec que des voisins de notre couleur \n");
            ecrireIndexCoup(i);
            return;
        }

        //règle 3
        if(currentPosition.cols[o].couleur==couleurAdverse(myColor) && currentPosition.cols[d].couleur==couleurAdverse(myColor)) {
            int nb_voisin_couleur_adverse;

            for (int k=0; k < voisins_origine.nb; k++) {
                if(currentPosition.cols[voisins_origine.cases[k]].couleur==myColor && currentPosition.cols[voisins_origine.cases[k]].nb !=5) 
                {
                    voisins_du_voisin_origine=getVoisins(voisins_origine.cases[k]);
                    nb_voisin_couleur_adverse=0;

                    for (int l=0; l < voisins_du_voisin_origine.nb; l++) 
                    {
                        if (currentPosition.cols[voisins_du_voisin_origine.cases[l]].couleur==couleurAdverse(myColor) && currentPosition.cols[voisins_du_voisin_origine.cases[l]].nb !=5) 
                        {
                            nb_voisin_couleur_adverse++;
                        }
                    }
                    if (nb_voisin_couleur_adverse==1) {
                        printf("On bouge un pion/tour de la couleur adverse pour isoler une de nos tours et créer une zone d'avantage \n");
                        ecrireIndexCoup(i);
                        return;
                    }
                }
            }
        }

        //règle 6
        /*
        if(currentPosition.cols[o].couleur==couleurAdverse(myColor) && somme_tours !=5) 
        {
            for (int k=0; k < voisins_origine.nb; k++) {
                if(currentPosition.cols[voisins_origine.cases[k]].couleur==couleurAdverse(myColor) && currentPosition.cols[voisins_origine.cases[k]].nb !=5) 
                {
                    ok=1;
                    voisins_du_voisin_origine=getVoisins(voisins_origine.cases[k]);

                    for (int l=0; l < voisins_du_voisin_origine.nb; l++) 
                    {
                        if ((currentPosition.cols[voisins_du_voisin_origine.cases[l]].couleur==couleurAdverse(myColor) && voisins_du_voisin_origine.cases[l] !=o) || voisins_du_voisin_origine.cases[l]==d) 
                        {
                            ok=0;
                            break;
                        }
                    }
                    if (ok) {
                        voisins_destination=getVoisins(d);
                        somme_tours=currentPosition.cols[d].nb+currentPosition.cols[o].nb;
                        for(int k=0; k < voisins_destination.nb; k++) {
                            if ((somme_tours + currentPosition.cols[voisins_destination.cases[k]].nb)==5) {
                                ok=0;
                            }
                        }
                        if (ok) {
                            ok = 0;
                            for(int k=0; k < voisins_destination.nb; k++) {
                                if ((somme_tours + currentPosition.cols[voisins_destination.cases[k]].nb)<5 && (currentPosition.cols[voisins_destination.cases[k]].couleur==myColor)) {
                                    ok=1;
                                }
                            }
                        }
                    }
                    if (ok) {
                        voisins_destination=getVoisins(d);
                        ok = 0;
                        for(int k=0; k < voisins_destination.nb; k++) {
                            if (currentPosition.cols[voisins_destination.cases[k]].couleur==myColor) {
                                ok=1;
                            }
                        }
                    }
                    if (ok) {
                        printf("On bouge un pion/tour de la couleur adverse pour isoler une tour adverse avec seulement des voisins de notre couleur \n");
                        ecrireIndexCoup(i);
                        return;
                    }
                }
            }
        }
        */
    }

    for(int i=0; i<listeCoups.nb; i++) {
        o=listeCoups.coups[i].origine;
        d=listeCoups.coups[i].destination;

        //Regle 7
        if((currentPosition.cols[o].nb==1)&&(currentPosition.cols[d].nb==1)&&(currentPosition.cols[o].couleur==myColor)&&(currentPosition.cols[d].couleur==couleurAdverse(myColor))) {
            printf("Empiler deux pions avec notre couleur au dessus\n");
            ecrireIndexCoup(i);
            return;
        }
    }

    //Regle 1//
    for(int i=0; i<listeCoups.nb; i++) {
        o=listeCoups.coups[i].origine;
        d=listeCoups.coups[i].destination;

        somme_tours=currentPosition.cols[d].nb+currentPosition.cols[o].nb;
        voisins_destination=getVoisins(d);
        ok=1;

        for(int k=0; k<voisins_destination.nb; k++) {
            if(somme_tours+currentPosition.cols[voisins_destination.cases[k]].nb==5) {
                ok=0;
                break;
            }
        }

        if (ok) {
            printf("On joue un coup qui ne permettra pas à l'adversaire de créer une tour de 5\n");
            ecrireIndexCoup(i);
            return;
        }
    }

    return;
}
