#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED


#include "vertex.h"

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/

/***************************************************
                    EDGE
****************************************************/

enum class Edge_type
{
    Trophic,
    Non_Trophic
};

Edge_type int_to_edge(int what);

int edge_to_int(Edge_type what);


/**
    \class EdgeInterface
    \brief Classe contenant l'interface (les widgets) d'une arete
*/
class EdgeInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Edge;
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

    // Le WidgetEdge qui "contient" toute l'interface d'un arc
    grman::WidgetEdge m_top_edge;

    // Une boite pour englober les widgets de réglage associés
    grman::WidgetBox m_box_edge;

    // Un slider de visualisation/modification du poids valeur de l'arc
    grman::WidgetVSlider m_slider_weight;

    // Un label de visualisation du poids de l'arc
    grman::WidgetText m_label_weight;

    // Une checkbox pour etre selectionné
    grman::WidgetCheckBox m_selection;

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    EdgeInterface(Vertex& from, Vertex& to, Edge_type type);
};


/**
    \class Edge
    \brief une arete
*/
class Edge
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class EdgeInterface;
    friend class Vertex;

private :
    /// indice du sommet de départ de l'arc
    int m_from = -1;

    /// indice du sommet d'arrivée de l'arc
    int m_to = -1;

    /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
    //m_weight correspond au coeficient de prédation : un lapin mange 1kg d'herbe (on le note coef)
    double m_weight;

    Edge_type m_type;

    bool m_selected = false;


    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<EdgeInterface> m_interface = nullptr;


public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Edge (double weight=0, Edge_type type=Edge_type::Trophic, EdgeInterface *interface=nullptr) :
        m_weight(weight), m_type(type), m_interface(interface) { }

    /// Edge étant géré par Graph ce sera la méthode update de graph qui appellera
    /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
    /// Voir l'implémentation Graph::update dans le .cpp
    void pre_update();
    void post_update();
};




#endif // EDGE_H_INCLUDED
