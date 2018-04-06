#include "grman/grman.h"
#include <iostream>

#include "graph.h"

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/


#define GRAPHFILE1 "graphe1.txt"
#define GRAPHFILE2 "graphe2.txt"

using namespace std;

int main()
{
    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");
    //mis avant init pour utiliser la souris

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Un exemple de graphe. pris depuis le fichier GRAPHEFILE1
    Graph g(GRAPHFILE1);
    //g.make_test1();


//    //sauvegarde dans le nouveau fichier
//    ofstream myFile1(GRAPHFILE1, ios::out | ios::trunc);
//
//    myFile1 << g;
//    myFile1.close();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] && !g.get_quit())
    {

        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();


        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

        if( grman::key_press[KEY_SPACE] )
            g.turn();
    }

    ///NE PAS ÉCRIRE DANS LE GRAPHEFILE1!! (on veut garder une instance de ce fichier propre)
    //sauvegarde dans le nouveau fichier
    ofstream myFile2(GRAPHFILE2, ios::out | ios::trunc);

    myFile2 << g;
    myFile2.close();

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();



