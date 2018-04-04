#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED


#include <vector>
#include <map>
#include <string>
#include <memory>
#include <set>

#include "grman/grman.h"


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

    // Un slider de visualisation/modification de la valeur du sommet
    grman::WidgetVSlider m_slider_value;

    // Un label de visualisation de la valeur du sommet
    grman::WidgetText m_label_value;

    // Une image de "remplissage"
    grman::WidgetImage m_img;

    // Un label indiquant l'index du sommet
    grman::WidgetText m_label_idx;

    // Une boite pour le label précédent
    grman::WidgetText m_box_label_idx;

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
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

    ///un marquages pour les algos de passage
    bool m_flag;
    int m_compNum;

    /// un exemple de donnée associée au sommet, on peut en ajouter d'autres...
    double m_value; //nombre d'individu (pour des animaux) ou masse totale (pour des ressources)
    double m_r; //rythme de croissance

    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<VertexInterface> m_interface = nullptr;

    // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
    // La ligne précédente est en gros équivalent à la ligne suivante :
    // VertexInterface * m_interface = nullptr;


public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Vertex (double value=0, double r=1, VertexInterface *interface=nullptr) :
        m_value(value), m_r(r), m_interface(interface)  {  }

    /// Vertex étant géré par Graph ce sera la méthode update de graph qui appellera
    /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
    /// Voir l'implémentation Graph::update dans le .cpp
    //On fait l'étude démographique dans pre_update
    void pre_update(Graph* g);
    void post_update();
};


#endif
