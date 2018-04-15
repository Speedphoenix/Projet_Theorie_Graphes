#ifndef TOOLBOX_H_INCLUDED
#define TOOLBOX_H_INCLUDED


#include "edge.h"

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/

/***************************************************
                       TOOLBOX
****************************************************/

const int BUTTONWIDTH = 72;
const int BUTTONHEIGHT = 35;
const int BUTTONMARGIN = 4;
const int BUTTONSPACE = BUTTONHEIGHT + 2*BUTTONMARGIN; //l'espace total utilisé par un bouton
const int BUTTONPART = 20;

/**
    \enum UserAction
    \brief enum des differentes actions prises par l'utilisateur
*/
enum class UserAction
{
    Nothing,
    Quit,
    NewGraph,
    SaveGraph,
    LoadGraph,
    AddVertex,
    AddEdge,
    Delete,
    HardConnex,
    KConnex,
    Turn
};

/**
    \class ToolboxInterface
    \brief Classe contenant l'interface (tous les widgets) de la classe Toolbox
*/
class ToolboxInterface
{
    friend class Toolbox;
    friend class Graph;

private :

    // La boite qui contient toute l'interface de la tool box
    grman::WidgetBox m_top_box;


    //les boutons
    //tous ces boutons pourraient être mis dans un tableau, mais ils ont tous des valeurs differentes...
    grman::WidgetButtonText m_quit_btn;

    grman::WidgetButtonText m_new_graph_btn;

    grman::WidgetButtonText m_save_graph_btn;

    grman::WidgetButtonText m_load_graph_btn;

    grman::WidgetButtonText m_new_vertex_btn;

    grman::WidgetButtonText m_new_edge_btn;

    //bouton pour detecter les composantes fortement connexes
    grman::WidgetButtonText m_fortement_conn_btn;

    //bouton pour detecter la k-connexité du graphe
    grman::WidgetButtonText m_k_connexe_btn;

    //bouton pour faire un tour
    grman::WidgetButtonText m_turn_btn;

    //check this to make a real-time turn
    grman::WidgetBox m_continuous_turn_box;
    grman::WidgetCheckBox m_continuous_turn_checkbox;
    grman::WidgetText m_continuous_turn_msg;

    //the following should be hidden until something is selected
    grman::WidgetButtonText m_delete_btn;



//    ///la liste des widgets qui sont dans la toolbox
//    std::vector<grman::Widget *> m_widgets;

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    ToolboxInterface(int w, int h);
};

/**
    \class Toolbox
    \brief Classe capturant toutes les interractions dans la partie à gauche d'un graphe
*/
class Toolbox
{
    friend class Graph;

private :

    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<ToolboxInterface> m_interface = nullptr;

    ///s'il faut montrer le bouton de suppression etc
    bool m_show_selection;

    ///l'action choisie de l'utilisateur, le plus souvent UserAction::Nothing
    UserAction m_user_choice;

    ///si il faut effectuer des tours en continu
    bool m_continuous_turn;

//    Coords m_bottom_space;

public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Toolbox (ToolboxInterface *interface=nullptr) :
        m_interface(interface), m_show_selection(false), m_continuous_turn(false)  {  }


    /**
        \fn void set_selection(bool how = true)
        \brief Donnel'état de séléction de l'utilisateur
                Si un sommet ou une arete est séléctionné il faut afficher le bouton delete
    */
    void set_selection(bool how = true);

    void pre_update();
    void post_update();

};

/**
    \fn void separate_loop(grman::Widget& parent, bool enter_to_send = false)

    \brief fais tourner une boucle de jeu et quitte avec un bouton.
            très pratique pour des saisies de l'utilisateur sans avoir
            à modifier le graphe d'origine

    \param parent Un widget contenant (en child widget) tous les widgets à afficher en boucle
    \param enter_to_send mettre à true pour que la touche ENTER fasse quitter la boucle
*/
void separate_loop(grman::Widget& parent, bool enter_to_send = false);

/**
    \fn void text_input(std::string& dest, std::string message_to_disp="")
    \brief demande une entrée de texte à l'utilisateur dans un separate loop

    \param[out] dest la string de destination
    \param[in] message_to_disp string a montrer (exemple "entrez le nom svp")
*/
void text_input(std::string& dest, std::string message_to_disp="");

/**
    \fn void new_vertex_values(std::string& name, std::string& pic_file_name)
    \brief demande une entrée des valeurs d'un sommet à l'utilisateur dans un separate loop

    \param[out] name le nom du sommet
    \param[out] pic_file_name string le nom du fichier econtenant l'image du sommet
*/
void new_vertex_values(std::string& name, std::string& pic_file_name);

//on a pas de solution graphique pour les chiffres à virgule pour l'instant
/**
    \fn void new_edge_tips(Graph& dest, int& from, int& to)
    \brief demande une entrée des valeurs d'une arete à l'utilisateur dans un separate loop

    \param[in] dest le graphe, pour blinder les sommets non existants
    \param[out] from id du sommet de départ de l'arete
    \param[out] to id du sommet d'arrivée de l'arete
*/
void new_edge_tips(Graph& dest, int& from, int& to);


#endif // TOOLBOX_H_INCLUDED




