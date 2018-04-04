#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**************************************************************
    Ici sont proposées 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Arête ou Arc)
            Graph (=Graphe)

    Les arêtes et les sommets et le graphe qu'ils constituent
    "travaillent" étroitement ensemble : pour cette raison les
    Vertex et Edge se déclarent amis (friend) de Graph pour que
    ce dernier puisse librement accéder aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent êtres complétées. Il est également possible
    de les dériver mais il est malheureusement assez difficile
    de dériver le "triplet" des 3 classes en maintenant des relations
    cohérentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute préférable, si possible,
    de "customiser" ces classes de base directement, sans héritage.

    Le modèle proposé permet de représenter un graphe orienté éventuellement
    pondéré, les arcs portent une ou des informations supplémentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse être assez facilement
    lue et écrite en fichier, et bénéficie d'une bonne lisibilité en cas de bugs...

    Chaque arc possède 2 attributs principaux (en plus de son éventuelle pondération)
        -> m_from (indice du sommet de départ de l'arc )
        -> m_to (indice du sommet d'arrivée de l'arc )

    Chaque sommet possède 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : accès aux prédécesseurs)
        -> m_out (liste des indices des arcs partant du sommet : accès aux successeurs)

    Cependant le problème des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entité (un arc et/ou un sommet sont enlevés du graphe) alors :

    - Soit il faut reprendre toute la numérotation pour "boucher le trou"
      (par exemple on a supprimé le sommet n°4, le sommet n°5 devient le 4, 6 devient 5 etc...)
      ce qui pose des problèmes de stabilité et de cohérence, et une difficulté à ré-introduire
      le(s) même(s) élément supprimé (au même indice)

    - Soit on admet que la numérotation des sommets et arcs n'est pas contigue, càd qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La numérotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet à des indices arbitraires (pas forcément contigus)

    C'est cette 2ème approche qui est proposée ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra être attentif au fait que par rapport à un simple vecteur, le parcours des éléments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours à itérateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'intérêt se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associée à chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non représentés à l'écran
    Imaginons par exemple qu'on doive générer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas représenter graphiquement ces 1000 graphes, et les
    interfaces pèsent lourd en ressource, avec cette organisation on est libre de réserver ou
    pas une interface de présentation associée aux datas (découplage données/interface)

***********************************************************************************************/

#include "edge.h"


/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

    /// La boite qui contient toute l'interface d'un graphe
    grman::WidgetBox m_top_box;

    /// Dans cette boite seront ajoutés les (interfaces des) sommets et des arcs...
    grman::WidgetBox m_main_box;

    /// Dans cette boite seront ajoutés des boutons de contrôle etc...
    grman::WidgetBox m_tool_box;

        /// Dans cette boite sera ajout� l'interface de saisie
        //grman::WidgetTextSaisie;


    // A compléter éventuellement par des widgets de décoration ou
    // d'édition (boutons ajouter/enlever ...)

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    GraphInterface(int x, int y, int w, int h);
};


class Graph
{
private :

    /// La "liste" des arêtes
    std::map<int, Edge> m_edges;

    /// La liste des sommets
    std::map<int, Vertex> m_vertices;

    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<GraphInterface> m_interface = nullptr;


    /// 'colorie' (change nompNum) tous les sommets ayant comme numero de comp 'old' et leur donne 'new'
    void unicompAllVert(int ancien, int nouveau);

    ///la fonction récursive appelée dans fortementConnexes()
    ///permet de trouver les composantes connexes des sommets suivant 'where' de manière récursive
    ///previously called sgadablouch
    void reconnexite(std::vector<int>& origin, int where, std::set<int>& passedBy);

    ///renvoie un numero de composante pas encore utilisé
    int getNewCompNum();

    ///va flag les sommets de receivedComps et assigner une composante à ceux sans.
    void flagRemaining(std::set<int>& receivedComps);


public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Graph (GraphInterface *interface=nullptr) :
        m_interface(interface)  {  }

    //Getters and setters
    Edge& getEdge(int id) { return m_edges.at(id); }
    Vertex& getVertex (int id) { return m_vertices.at(id); }

    ///Ajout de fonctions pour graphs non interfacé
    void add_vertex(int idx, double value, double r);
    void add_edge(int idx, int vert1, int vert2, double weight=0);

    void add_interfaced_vertex(int idx, double value, double r, int x, int y, std::string pic_name="", int pic_idx=0 );
    void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

    /// Méthode spéciale qui construit un graphe arbitraire (démo)
    void make_test1();

    void reset_flags();
    void reset_comps();

    /// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
    void update();

    void fortementConnexes();
};


#endif // GRAPH_H_INCLUDED

