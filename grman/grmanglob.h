#ifndef GRMANGLOB_H_INCLUDED
#define GRMANGLOB_H_INCLUDED


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include <allegro.h>

#include "grman_couleurs.h"
#include "coords.h"


/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/

/**
    Ce fichier contient toutes les globales du namespace grman
    Ce fichier a été séparé de grman.h pour pouvoir include widget.h en haut de grman.h (au lieu de tout en bas)

    Modifications:

    Ajout de la macro E pour debug, recyclée du projet Ecemon

    les autres modifications apportées sont énoncées dans grman.h
*/

#define MOUSEFILE "Mouse.bmp"

///pour eviter les problèmes de souris (en vain)
#ifndef _WINDOWS

    #include <xalleg.h>

    #ifdef None
        #undef None
    #endif

#endif // _WINDOWS

#ifdef E
    #undef E
#endif // E

///cette macro sert à débug: affichera x sous la forme d'une string, puis la valeur de x.
#define E(x) {std::cerr<<std::endl<<#x " : " << x << std::endl;}

// Temporisation de la boucle de jeu en ms ( rest dans mettre_a_jour )
#define TEMPO_MAJ 20


namespace grman
{

/// Constantes

//des reacourcis pour l'entrée de texte
const std::string shrct[10] = {
    ".txt",
    "graphe1.txt",
    "graphe2.txt",
    "botw",
    "arctique",
    "",             ///à utiliser pour des noms d'image, de grahes etc
    "",
    ".bmp",
    ".jpg",
    ".png"
    };

/// Variables globales : visibles partout où grman.h est inclus

// La couleur de fond effective utilisée
extern int page_color;

// Le buffer sur lequel tout est dessiné
extern BITMAP *page;

// La frame associée pour les widgets
extern Frame page_frame;

// Gestion des widget : avec quel widget la souris interagit ?
class Widget;
extern Widget *gui_over;
extern Widget *gui_last_over;
extern Widget *gui_focus; //focus = souris est dessus et clique
extern Widget *gui_leave;


/// Les globales suivantes nécessitent un appel à rafraichir_clavier_souris
/// en début de boucle d'interaction pour être mises à jour à chaque tour de boucle

// La key_last appuyée depuis le dernier tour de boucle
// '\0' si aucun bouton appuyé au dernier tour de boucle
extern char key_last;

extern Coords mouse_pos;

// Le clic souris depuis le dernier tour de boucle
// s'utilise comme mouse_b mais contrairement à mouse_b
// n'est valable que pendant un tour de boucle (pas de répétition)
// mouse_click&1 pour clic gauche, mouse_click&2 pour clic droit
extern int mouse_click;

// Le relâchement du clic souris depuis le dernier tour de boucle
// détecte le passage de l'état enfoncé à l'état relâché
// n'est valable que pendant un tour de boucle (pas de répétition)
// mouse_unclick&1 pour clic gauche, mouse_unclick&2 pour clic droit
extern int mouse_unclick;

// Même principe que key mais détecte les transitions (pressé<->non pressé)
// valable pendant un seul tour de boucle (pas de répétition)
// exemple : if (key_unpress[KEY_RIGHT]) printf("touche droite relachee !\n");
extern char key_press[KEY_MAX];
extern char key_unpress[KEY_MAX];

//position de la souris au moment du clic
extern int mouse_click_x;
extern int mouse_click_y;

// Déplacement relatif de la souris depuis le dernier tour de boucle
extern int mouse_move_x;
extern int mouse_move_y;
}



#endif // GRMANGLOB_H_INCLUDED

