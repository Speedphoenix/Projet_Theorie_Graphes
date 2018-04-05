#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <iostream>
#include <fstream>

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <set>

#include "grman/grman.h"

///cette macro sert à débug: affichera x sous la forme d'une string, puis la valeur de x.
#define E(x) {std::cerr<<std::endl<<#x " : " << x << std::endl;}

class Graph;


/***************************************************
                    VERTEX
****************************************************/

class VertexInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

    // La boite qui contient toute l'interface d'un sommet
    grman::WidgetBox m_top_box;

    //Un text qui contient le nom du sommet
    grman::WidgetText m_name;

    //Une boite pour le nom
    grman::WidgetText m_box_name;

    // Un slider de visualisation/modification de la valeur du sommet
    grman::WidgetVSlider m_slider_value;

    // Un label de visualisation de la valeur du sommet
    grman::WidgetTextSaisie m_label_value;

    // Une image de "remplissage"
    grman::WidgetImage m_img;

    // Un label indiquant l'index du sommet
    grman::WidgetText m_label_idx;

    // Une boite pour le label précédent
    grman::WidgetText m_box_label_idx;

    // Un label indiquant le numero de sa composante (fortement) connexe
    ///A DÉPLACER!! (en termes de position/methode d'affichage dans le widget principal)
    grman::WidgetText m_label_comp;

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    VertexInterface(int idx, int x, int y, std::string name="", std::string pic_name="", int pic_idx=0);
};


class Vertex
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

private :
    /// liste des indices des arcs arrivant au sommet : accès aux prédécesseurs
    std::vector<int> m_in;

    /// liste des indices des arcs partant du sommet : accès aux successeurs
    std::vector<int> m_out;

    /// un exemple de donnée associée au sommet, on peut en ajouter d'autres...
    double m_value; //nombre d'individu (pour des animaux) ou masse totale (pour des ressources)
    double m_r; //rythme de croissance
    std::string m_name;

    ///un marquages pour les algos de passage
    bool m_flag;
    int m_compNum;


    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<VertexInterface> m_interface = nullptr;

    // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
    // La ligne précédente est en gros équivalent à la ligne suivante :
    // VertexInterface * m_interface = nullptr;


public:

    /// Les constructeurs sont à compléter selon vos besoin...

    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    //Problème d'ordre des paramètres (initialisation d'interface sans le nom...)
    Vertex (double value=0, double r=1, VertexInterface *interface=nullptr, std::string name="") :
        m_value(value), m_r(r), m_name(name), m_flag(false), m_compNum(-1), m_interface(interface) { }

    Vertex (double value, double r, std::string name="") :
        m_value(value), m_r(r), m_name(name), m_flag(false), m_compNum(-1), m_interface(nullptr) { }

    /// Vertex étant géré par Graph ce sera la méthode update de graph qui appellera
    /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
    /// Voir l'implémentation Graph::update dans le .cpp
    //On fait l'étude démographique dans pre_update

    void pre_update();
    void post_update();

    void turn(Graph& g);
};

#endif
