#ifndef GRMAN_H_INCLUDED
#define GRMAN_H_INCLUDED

/// Graphic Manager is a collection of functions and globals
/// that acts as a glue between your C++ app and Allegro
/// Call grman::init(); once at startup

#include <allegro.h>

#include <string>


/**
 *  MODIFICATIONS APPORTÉES:
 *
 *  déplacé le commentaire de mouse_click_x/y (mouse_move_x/y)
 *  enlevé la deuxième déclaration de mouse_click
 *  changé le type de key_press/unpress à char pour économie de mémoire (key de allegro est un tableau de volatile char)
 *                key_prev et key_now dans rafraichir_clavier_souris() aussi
 *
 *
 *
 *
 *
*/


#include "grman_couleurs.h"
#include "coords.h"

namespace grman
{



/// Constantes

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
extern Widget *gui_focus;
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

//deuxième déclaration...
//extern int mouse_click;

/// Gestion des ressources image (fichiers images et BITMAP chargées)
unsigned get_picture_nb(std::string name);
BITMAP *get_picture(std::string pic_name);
void show_picture(BITMAP *dest, std::string file_name, int x, int y, unsigned idx=0);
void set_pictures_path(std::string path_name);

/// A appeler une fois en fin de boucle de jeu
void mettre_a_jour();

/// Lancement et fermeture services Allegro

// A appeler une fois et une seule en début de main (après les déclarations)
void init();

// Pour libérer le buffer et fermer tous les services
// pas indispensable : la fin du programme ferme et libère tout automatiquement
void fermer_allegro();


/// Buffer et sorties graphiques

// Efface le buffer (utilise page_color)
// A appeler une fois dans la boucle d'interaction
// avant de redessiner le dessin (et éventuellement
// autre chose après)
void buf_effacer_page();

// Affiche la page effectivement à l'écran
// A appeler une fois dans la boucle d'interaction
// A LA FIN JUSTE AVANT rest(10);
void buf_afficher_page();


/// Entrées clavier/souris

// un appel à rafraichir_clavier_souris doit être fait
// en début de boucle d'interaction pour mettre à jour les globales
void rafraichir_clavier_souris();


/// Auxiliaires : compléments aux fonctions graphiques allegro

void thick_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int thickness, int color);

}

/************************************************
            A CORRIGER
*************************************************/

/// Cette inclusion en fin de header parce que widget dépend de grman
/// Ceci est évitable (et à éviter) en re-factorisant le projet et les dépendance...
#include "widget.h"


#endif // GRMAN_H_INCLUDED
