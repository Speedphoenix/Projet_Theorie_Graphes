#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <iostream>
#include <fstream>

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <set>
#include <sstream>

#include "grman/grman.h"

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/

class Graph;


const int default_x = 20;
const int default_y = 20;
const int default_value = 0;
const int default_r = 0.1;

const double max_value = 200.0;


/***************************************************
                    VERTEX
****************************************************/

/**
    \enum Vertex_type
    \brief enum des differents types de sommets
*/
enum class Vertex_type
{
    Exp,        //vertex non prédateur, ne consommant pas de ressource (herbe)
    Logistic,   //vertex predateur et predaté
    Fossil      //
};

/** \fn Vertex_type int_to_vertex(int what)
    \brief renvoie un element de l'enum Vertex_Type associé à l'entier en entree
*/
Vertex_type int_to_vertex(int what);

/** \fn int vertex_to_int(Vertex_type what)
    \brief renvoie un entier associé à l'element de l'enum Vertex_Type en entree
*/
int vertex_to_int(Vertex_type what);


/**
    \class VertexInterface
    \brief classe contenant l'interface (les Widgets) d'un sommet
*/
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
    grman::WidgetVSliderLog m_slider_value;

    // Un label de visualisation de la valeur du sommet
    grman::WidgetNumSaisie m_label_value;

    // Une image de "remplissage"
    grman::WidgetImage m_img;

    // Un label indiquant l'index du sommet
    grman::WidgetText m_label_idx;

    // Une boite pour le label précédent
    grman::WidgetText m_box_label_idx;

    // Un label indiquant le numero de sa composante (fortement) connexe
    grman::WidgetText m_label_comp;

    //une boite pour le label précédent
    grman::WidgetText m_box_label_comp;

    // Une checkbox pour etre selectionné
    grman::WidgetCheckBox m_selection;

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    VertexInterface(int idx, int x, int y, std::string name="", std::string pic_name="", int pic_idx=0);
};

/**
    \class Vertex
    \brief un sommet
*/
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
    Vertex_type m_type;

    ///un marquages pour les algos de passage
    bool m_flag;
    int m_compNum;

    ///si ce sommet est séléctionné
    bool m_selected = false;


    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<VertexInterface> m_interface = nullptr;

    // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
    // La ligne précédente est en gros équivalent à la ligne suivante :
    // VertexInterface * m_interface = nullptr;


public:

    Vertex (double value=0, double r=1, VertexInterface *interface=nullptr, std::string name="", Vertex_type type=Vertex_type::Logistic) :
        m_value(value), m_r(r), m_name(name), m_type(type), m_flag(false), m_compNum(-1), m_interface(interface) { }

    Vertex (double value, double r, std::string name="", Vertex_type type=Vertex_type::Logistic) :
        m_value(value), m_r(r), m_name(name), m_type(type), m_flag(false), m_compNum(-1), m_interface(nullptr) { }


    /// Vertex étant géré par Graph ce sera la méthode update de graph qui appellera
    /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
    /// Voir l'implémentation Graph::update dans le .cpp
    //On fait l'étude démographique dans pre_update

    void pre_update();

    void post_update();

    void turn2(Graph& g, double t = 1);

//    void turn(Graph& g);
//
//    void turn_dif(Graph& g);
//    void turn_exp(Graph& g);
//    void turn_logistic(Graph& g);
};

#endif

