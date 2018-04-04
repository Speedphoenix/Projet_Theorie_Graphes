#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED


#include "vertex.h"

/***************************************************
                    EDGE
****************************************************/

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

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    EdgeInterface(Vertex& from, Vertex& to);
};


class Edge
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class EdgeInterface;
    friend class Vertex;

private :
    /// indice du sommet de départ de l'arc
    int m_from;

    /// indice du sommet d'arrivée de l'arc
    int m_to;

    /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
    //m_weight correspond au coeficient de prédation : un lapin mange 1kg d'herbe (on le note coef)
    double m_weight;

    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<EdgeInterface> m_interface = nullptr;


public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Edge (double weight=0, EdgeInterface *interface=nullptr) :
        m_weight(weight), m_interface(interface)  {  }

    /// Edge étant géré par Graph ce sera la méthode update de graph qui appellera
    /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
    /// Voir l'implémentation Graph::update dans le .cpp
    void pre_update();
    void post_update();
};




#endif // EDGE_H_INCLUDED
