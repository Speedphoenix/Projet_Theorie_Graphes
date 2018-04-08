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

#include "toolbox.h"
#include <algorithm> //pour la fonction sort()

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/


/***************************************************
                    GRAPH
****************************************************/

const int max_k_connexe = 5;


/**
    \class GraphInterface
    \brief classe contenant l'interface (les widgets) d'un graphe
*/
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

    /// Dans cette boite seront affichés les resultats d'algos....
    grman::WidgetBox m_console_box;
    grman::WidgetText m_console_text_l1;
    grman::WidgetText m_console_text_l2;

    // A compléter éventuellement par des widgets de décoration ou
    // d'édition (boutons ajouter/enlever ...)

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    GraphInterface(int x, int y, int w, int h);
};


/**
    \class Graph
    \brief Un graphe. contient toutes les aretes, tous les sommets, la toolbox
            s'occupe de s'actualiser, contient toutes les methodes importantes
*/
class Graph
{
private :

    bool m_want_to_quit = false;

    /// La "liste" des arêtes
    std::map<int, Edge> m_edges;

    /// La liste des sommets
    std::map<int, Vertex> m_vertices;

    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<GraphInterface> m_interface = nullptr;

    /// La tool_box qui contien les boutons
    Toolbox m_toolbox;

    ///les sommets selectionnés
    std::vector<int> m_selected_vertices;

    ///les aretes selectionnées
    std::vector<int> m_selected_edges;

    /// \fn void processInput(UserAction what)
    /// \brief Regarde la demande de l'utilisateur et agit en conséquence
    /// \param[in] what l'action à effectuer
    void processInput(UserAction what);

    /// 'colorie' (change nompNum) tous les sommets ayant comme numero de comp 'old' et leur donne 'new'
    void unicompAllVert(int ancien, int nouveau);

    /// \fn void reconnexite(std::vector<int>& origin, int where, std::set<int>& passedBy)
    /// \brief la fonction récursive appelée dans fortementConnexes()
    ///     permet de trouver les composantes connexes des sommets suivant 'where' de manière récursive
    ///     previously called sgadablouch
    void reconnexite(std::vector<int>& origin, int where, std::set<int>& passedBy);

    ///renvoie un numero de composante fortement connexe pas encore utilisé
    int getNewCompNum();

    /// \fn int getUnusedVertexId()
    /// \return un indice de sommet pas encore utilisé
    int getUnusedVertexId();

    /// \fn int getUnusedEdgeId()
    /// \return un indice d'arete pas encore utilisé
    int getUnusedEdgeId();

    /// \fn void flagRemaining(std::set<int>& receivedComps)
    /// \brief va flag les sommets de receivedComps et assigner une composante à ceux sans.
    void flagRemaining(std::set<int>& receivedComps);

    /// \fn void dfs_recurs(int where, unsigned& flag_count)
    /// \brief fonction style dsf pour voir si le graphe est connexe (simplement)
    void dfs_recurs(int where, unsigned& flag_count);

public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Graph (GraphInterface *interface=nullptr) :
        m_interface(interface)  { if (interface) initialize_toolbox(); }

    ///prend le graphe depuis un fichier
    Graph (std::string filename);
    Graph (std::istream& file);

    /**
        \fn void initialize_toolbox()
        \brief crée et initialise la toolbox...
    */
    void initialize_toolbox();


    //Getters and setters
    Edge& getEdge(int id) { return m_edges.at(id); }
    Vertex& getVertex (int id) { return m_vertices.at(id); }

    ///Ajout de fonctions pour graphs non interfacé
    void add_vertex(Vertex& source, int id);
    void add_vertex(std::string name, int idx, double value, double r, Vertex_type type=Vertex_type::Logistic);
    void add_edge(Edge& source, int id);
    void add_edge(int idx, int id_vertFrom, int id_vertTo, double weight=0, Edge_type type=Edge_type::Trophic);

    void add_interfaced_vertex(Vertex& source, int id);
    void add_interfaced_vertex(int idx, double val, double r, int x, int y, std::string name="", Vertex_type type=Vertex_type::Logistic, std::string pic_name="", int pic_idx=0 );
    void add_interfaced_edge(Edge& source, int id);
    void add_interfaced_edge(int idx, int id_vertFrom, int id_vertTo, double weight=0, Edge_type type=Edge_type::Trophic);

    /** \fn void remove_vertex(int id)
        \brief enleve le sommet n°id et toutes les aretes connectées
    */
    void remove_vertex(int id);

    /** \fn void remove_edge(int id)
        \brief enleve l'arete n°id
    */
    void remove_edge(int id);

    /// Méthode spéciale qui construit un graphe arbitraire (démo)
    void make_test1();

    /** \fn void reset_flags()
        \brief remet à zero tous les marquages des sommets
    */
    void reset_flags();
    void reset_comps();

    /** \fn void reset_graph();
        \brief enlève tous les sommets e les aretes
    */
    void reset_graph();

    /** \fn void operator=(Graph& source)
        \brief copie le graphe source (l'interface aussi)
    */
    void operator=(Graph& source);

    /** \fn void interfaceless(Graph& source)
        \brief copie le graphe source (sans l'interface)
    */
    void interfaceless(Graph& source);

    /** \fn bool vertex_exists(int id)
        \return si le sommet n°id existe ou non
    */
    bool vertex_exists(int id) { return m_vertices.find(id)!=m_vertices.end(); }

    /** \fn bool edge_exists(int id)
        \return si l'arete n°id existe ou non
    */
    bool edge_exists(int id) { return m_edges.find(id)!=m_edges.end(); }


    /// \fn void update()
    /// \brief La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
    void update();

    /** \fn void fortementConnexes()
        \brief trouve les composantes fortement connexes (accessibles dans m_compNum de chaque sommet)
        \return le nombre de composantes fortement connexes
    */
    int fortementConnexes();

    /** \fn bool simplementConnexe()
        \return si le graphe est connexe (simplement) ou non
    */
    bool simplementConnexe();

    /** \fn int kConnexe(std::vector<int>& rep, int max_k, bool& worked)
        \brief calcule k, le nombre minimum de sommets à enlever pour que le graphe ne soit plus (simplement)connexe
                cette fonction utilise beaucoup de ressources

        \param[out] rep le vecteur qui va contenir un des ensembles de k vecteurs à enlever
        \param[in] max_k la limite de k_connexité qu'on veut (on ne veut pas surcharger)
        \param[out] worked si la recherche de k-connexité a abouti ou non
        \return k, le nombre de sommets minimum à enlever pour que le graphe de soit plus connexe
    */
    //renvoie k, le nombre minimum de sommets à enlever pour que le graphe ne soit plus (simplement)connexe
    int kConnexe(std::vector<int>& rep, int max_k, bool& worked);

    /** \fn int kConnexe_heavy(std::vector<std::vector<int>>& rep, int max_k, bool& worked)
        \brief calcule k, le nombre minimum de sommets à enlever pour que le graphe ne soit plus (simplement)connexe
                Difference: elle permet d'avoir toutes les possibilités de k au lieu de juste une
                cette fonction utilise encore plus de ressources. Elle n'est pas très optimisée...


        \param[out] rep le vecteur qui va contenir les ensembles de k vecteurs à enlever
        \param[in] max_k la limite de k_connexité qu'on veut (on ne veut pas surcharger)
        \param[out] worked si la recherche de k-connexité a abouti ou non
        \return k, le nombre de sommets minimum à enlever pour que le graphe de soit plus connexe
    */
    int kConnexe_heavy(std::vector<std::vector<int>>& rep, int max_k, bool& worked);

    /** \fn void send_stream(std::ostream& myStream)
        \brief Envoie toutes les données du graphe dans un Stream ou fichier
    */
    void send_stream(std::ostream& myStream);

    /** \fn void get_stream(std::istream& myStream)
        \brief Recoit toutes les données du graphe depuis un Stream ou fichier
    */
    void get_stream(std::istream& myStream);

    /** \fn void turn(double t = 1)
        \brief effectue un 'tour' (une actualisation du graphe)
        \param[in] t le facteur de changement (pour faire des fractions de changements)
    */
    void turn(double t = 1);

    /** \fn bool get_quit()
        \return s'il faut fermer le programme ou non
    */
    bool get_quit() { return m_want_to_quit; }
};


std::ostream& operator<<(std::ostream& myStream, Graph& what);

std::istream& operator>>(std::istream& myStream, Graph& what);

#endif // GRAPH_H_INCLUDED

