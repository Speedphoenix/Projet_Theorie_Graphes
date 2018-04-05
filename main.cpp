#include "grman/grman.h"
#include <iostream>

#include "graph.h"

using namespace std;

int main()
{
    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");
    //mis avant init pour utiliser la souris

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Un exemple de graphe
    Graph g;
//    g.make_test1();

/// POUR TESTER LE chargement/sauvegarde
//    ifstream myFile("graphze1.txt", ios::in);
//
//    myFile >> g;
//    myFile.close();
//
//
//    ofstream myFile2("graphze2.txt", ios::out | ios::trunc);
//
//    myFile2 << g;
//    myFile2.close();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
        if (grman::key_press[KEY_SPACE])
            g.fortementConnexes();

        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();



