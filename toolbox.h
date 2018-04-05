#ifndef TOOLBOX_H_INCLUDED
#define TOOLBOX_H_INCLUDED


#include "edge.h"

/***************************************************
                       TOOLBOX
****************************************************/

const int BUTTONWIDTH = 72;
const int BUTTONHEIGHT = 35;
const int BUTTONMARGIN = 4;
const int BUTTONSPACE = BUTTONHEIGHT + 2*BUTTONMARGIN; //l'espace total utilisé par un bouton

class ToolboxInterface
{
    friend class Toolbox;
    friend class Graph;

private :

    /// La boite qui contient toute l'interface de la tool box
    grman::WidgetBox m_top_box;

    grman::WidgetButton m_new_vertex_btn;
    grman::WidgetText m_new_vertex_msg;

    grman::WidgetButton m_new_edge_btn;
    grman::WidgetText m_new_edge_msg;

    ///FAIRE UNE FONCTION QUI AFFICHE/ENLEVE
    //the following should be hidden until something is selected
    grman::WidgetButton m_delete_btn;
    grman::WidgetText m_delete_msg;

//    //only when exactly one edge/vertex is selected? not necessary, is it?
//    grman::WidgetButton m_edit_btn;
//    grman::WidgetText m_edit_msg;


//    ///la liste des widgets qui sont dans la toolbox
//    std::vector<grman::Widget *> m_widgets;

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    ToolboxInterface(int w, int h);
};


class Toolbox
{
    friend class Graph;

private :

    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<ToolboxInterface> m_interface = nullptr;

//    Coords m_bottom_space;

public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Toolbox (ToolboxInterface *interface=nullptr) :
        m_interface(interface)  {  }



    void position_widgets();

    void add_widget(grman::Widget* addit);


};

#endif // TOOLBOX_H_INCLUDED

