#ifndef TOOLBOX_H_INCLUDED
#define TOOLBOX_H_INCLUDED


#include "edge.h"

/***************************************************
                       TOOLBOX
****************************************************/



class ToolboxInterface
{
    friend class Toolbox;
    friend class Graph;

private :

    /// La boite qui contient toute l'interface de la tool box
    grman::WidgetBox m_top_box;

    ///la liste des widgets qui sont dans la toolbox
    vector<Widget &> m_widgets;

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

public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Toolbox (ToolboxInterface *interface=nullptr) :
        m_interface(interface)  {  }

};

#endif // TOOLBOX_H_INCLUDED

