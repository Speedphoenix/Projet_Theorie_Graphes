#include "toolbox.h"

#include "graph.h" //ok dans un .cpp

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/

using namespace std;

void setup_button(grman::Widget& x, int currently, int col)
{
    x.set_posy(currently + BUTTONMARGIN); //x n'est pas nécéssaire car on utilise de la gravité
    x.set_bg_color(col); //trouver une bonne couleur svp!
    x.set_dim(BUTTONWIDTH, BUTTONHEIGHT);
    x.set_gravity_xy(grman::GravityX::Center, grman::GravityY::None);
    x.set_margin(BUTTONMARGIN);
}

void setup_msg(grman::WidgetText& x, int col, string msg)
{
    x.set_color(col); //trouver une bonne couleur svp!
    x.set_message(msg);
}

ToolboxInterface::ToolboxInterface(int w, int h)
{
    int currently = 0;

    //m_top_box fera partie de tool box de graphe
    m_top_box.set_dim(w, h);
    m_top_box.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
    m_top_box.set_padding(0);
    m_top_box.set_margin(0);
    m_top_box.set_border(0);

    //bouton dde sauvegarde de graphe
    m_top_box.add_child(m_quit_btn);
    setup_button(m_quit_btn, currently, BLANC); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_quit_btn.get_message_widget(), ROUGE, "exit");


    currently += BUTTONSPACE;


    //bouton dde création de graphe
    m_top_box.add_child(m_new_graph_btn);
    setup_button(m_new_graph_btn, currently, GRISCLAIR); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_new_graph_btn.get_message_widget(), ORANGESOMBRE, "new graph");


    currently += BUTTONSPACE;


    //bouton dde sauvegarde de graphe
    m_top_box.add_child(m_save_graph_btn);
    setup_button(m_save_graph_btn, currently, GRISCLAIR); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_save_graph_btn.get_message_widget(), ORANGESOMBRE, "save graph");


    currently += BUTTONSPACE;


    //bouton dde chargement de graphe
    m_top_box.add_child(m_load_graph_btn);
    setup_button(m_load_graph_btn, currently, GRISCLAIR); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_load_graph_btn.get_message_widget(), ORANGESOMBRE, "load graph");


    currently += BUTTONSPACE;


    //bouton d'ajout de sommet
    m_top_box.add_child(m_new_vertex_btn);
    setup_button(m_new_vertex_btn, currently, VERTCLAIR); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_new_vertex_btn.get_message_widget(), VERTSOMBRE, "new vertex");


    currently += BUTTONSPACE;


    //bouton d'ajout d'arete
    m_top_box.add_child(m_new_edge_btn);
    setup_button(m_new_edge_btn, currently, VERTCLAIR); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_new_edge_btn.get_message_widget(), VERTSOMBRE, "new edge");


    currently += BUTTONSPACE;


    //bouton pour detecter les composantes fortement connexes
    m_top_box.add_child(m_fortement_conn_btn);
    setup_button(m_fortement_conn_btn, currently, BLEUCLAIR);

    setup_msg(m_fortement_conn_btn.get_message_widget(), NOIR, "fort-connexes");


    currently += BUTTONSPACE;


    //bouton pour faire un tour
    m_top_box.add_child(m_k_connexe_btn);
    setup_button(m_k_connexe_btn, currently, BLEUCLAIR);

    setup_msg(m_k_connexe_btn.get_message_widget(), NOIR, "k-connexe");


    currently += BUTTONSPACE;


    //bouton pour faire un tour
    m_top_box.add_child(m_turn_btn);
    setup_button(m_turn_btn, currently, BLEUCLAIR);

    setup_msg(m_turn_btn.get_message_widget(), NOIR, "1 tour");


    currently += BUTTONSPACE;


    //check this to make a real-time turn
    m_top_box.add_child(m_continuous_turn_box);
    setup_button(m_continuous_turn_box, currently, BLEUCLAIR); //marche aussi

    m_continuous_turn_box.add_child(m_continuous_turn_msg);
    setup_msg(m_continuous_turn_msg, NOIR, "tours continus");
    m_continuous_turn_msg.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Up);

    m_continuous_turn_box.add_child(m_continuous_turn_checkbox);
    m_continuous_turn_checkbox.set_dim(2*BUTTONPART, BUTTONPART);
    m_continuous_turn_checkbox.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Down);


    currently += BUTTONSPACE;


    //bouton de suppression
    //ce bouton sera ajouté et enlevé lors des selections
    setup_button(m_delete_btn, currently, ROUGECLAIR); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_delete_btn.get_message_widget(), NOIR, "delete");


    currently += BUTTONSPACE;


/*  //pas nécéssaire
    m_edit_btn.set_posy(currenty + BUTTONMARGIN);
///   m_edit_btn.set_bg_color(); //trouver une bonne couleur svp! (du orange?)
    m_edit_btn.set_dim(BUTTONWIDTH, BUTTONHEIGHT);
    m_edit_btn.set_gravity_xy(grman::GravityX::Center, grman::GravityY::None);
    m_edit_btn.set_margin(BUTTONMARGIN);

    //le message à mettre sur le bouton
    m_edit_btn.add_child(m_edit_msg);
    m_edit_msg.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
///    m_edit_msg.set_color(); //trouver une bonne couleur svp!
*/

}



void Toolbox::set_selection(bool val)
{
    if (m_show_selection!=val)
    {
        ToolboxInterface& interface = *m_interface;

        if (val)
        {
            interface.m_top_box.add_child(interface.m_delete_btn);
        }
        else
        {
            interface.m_top_box.remove_child(interface.m_delete_btn);
        }

        m_show_selection = val;
    }
}

void Toolbox::pre_update()
{

}


void Toolbox::post_update()
{
    ToolboxInterface& interface = *m_interface;

    if (interface.m_quit_btn.clicked())
    {
        m_user_choice = UserAction::Quit;
    }
    else if (interface.m_new_graph_btn.clicked())
    {
        m_user_choice = UserAction::NewGraph;
    }
    else if (interface.m_load_graph_btn.clicked())
    {
        m_user_choice = UserAction::LoadGraph;
    }
    else if (interface.m_save_graph_btn.clicked())
    {
        m_user_choice = UserAction::SaveGraph;
    }
    else if (interface.m_new_vertex_btn.clicked())
    {
        m_user_choice = UserAction::AddVertex;
    }
    else if (interface.m_new_edge_btn.clicked())
    {
        m_user_choice = UserAction::AddEdge;
    }
    else if (interface.m_fortement_conn_btn.clicked())
    {
        m_user_choice = UserAction::HardConnex;
    }
    else if (interface.m_k_connexe_btn.clicked())
    {
        m_user_choice = UserAction::KConnex;
    }
    else if (interface.m_turn_btn.clicked())
    {
        m_user_choice = UserAction::Turn;
    }
    else if (interface.m_delete_btn.clicked())
    {
        m_user_choice = UserAction::Delete;
    }
    else
    {
        m_user_choice = UserAction::Nothing;
    }


    m_continuous_turn = interface.m_continuous_turn_checkbox.get_value();

    //add for edit btn if necessary

}


///LES FONCTIONS CI-DESSOUS NE FONT PAS PARTIE DE LA CLASSE TOOLBOX
///mais elles servent tout de même à intérragir avec l'utilisateur

void separate_loop(grman::Widget& parent, bool enter_to_send)
{
    bool fini = false;

    grman::WidgetBox top_box;
    grman::WidgetButtonText exit_btn;

    top_box.set_dim(500, 500);
    //top_box.set_bg_color(BLANC);


    //on ajoute le truc en param
    top_box.add_child(parent);


    ///on ajoute le bouton de sortie du mode. absolument mettre ça APRES parent pour qu'il soit au dessus
    ///ref widget.cpp ligne 35
    top_box.add_child(exit_btn);
    exit_btn.set_bg_color(BLANC);
    exit_btn.set_dim(2*BUTTONWIDTH, 2*BUTTONHEIGHT);
    exit_btn.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    exit_btn.set_margin(BUTTONMARGIN);

    exit_btn.get_message_widget().set_message("Confirm");



    while (!fini)
    {
        top_box.update();

        grman::mettre_a_jour();

        if (grman::key_press[KEY_ESC] || exit_btn.clicked()     ||
            (enter_to_send && (grman::key_press[KEY_ENTER] || grman::key_press[KEY_ENTER_PAD])) )
            fini = true;
    }
}


void text_input(string& dest, string message_to_disp)
{
    grman::WidgetBox top_box;

    grman::WidgetText message;
    grman::WidgetTextSaisie val;

    top_box.set_dim(500, 500);

    top_box.add_child(val);
    val.set_message("");
    val.set_padding(2);
    val.set_border(2);
    val.set_posy(220);


    if (message_to_disp!="")
    {
        top_box.add_child(message);
        message.set_message(message_to_disp);
        message.set_posy(200);
    }


    separate_loop(top_box, true);

    dest = val.get_message();
}


void new_vertex_values(string& name, string& pic_file_name)
{
    grman::WidgetBox top_box;

    grman::WidgetText ask1;
    grman::WidgetText ask2;
    grman::WidgetTextSaisie nom;
    grman::WidgetTextSaisie pic_file;

    top_box.set_dim(500, 500);

    top_box.add_child(nom);
    nom.set_message("");
    nom.set_padding(2);
    nom.set_border(2);
    nom.set_posy(120);


    top_box.add_child(ask1);
    ask1.set_message("enter the name of the new vertex");
    ask1.set_posy(100);


    top_box.add_child(pic_file);
    pic_file.set_message("");
    pic_file.set_padding(2);
    pic_file.set_border(2);
    pic_file.set_posy(220);

    top_box.add_child(ask2);
    ask2.set_message("enter filename");
    ask2.set_posy(200);


    separate_loop(top_box, true);

    name = nom.get_message();
    pic_file_name = pic_file.get_message();
}


void new_edge_tips(Graph& dest, int& from, int& to)
{
    grman::WidgetBox top_box;

    grman::WidgetText ask1;
    grman::WidgetText ask2;
    grman::WidgetNumSaisie fromVertex;
    grman::WidgetNumSaisie toVertex;

    top_box.set_dim(500, 500);

    top_box.add_child(fromVertex);
    fromVertex.set_padding(2);
    fromVertex.set_border(2);
    fromVertex.set_posy(120);
    fromVertex.set_value(-1);


    top_box.add_child(ask1);
    ask1.set_message("enter the number of the from vertex");
    ask1.set_posy(100);


    top_box.add_child(toVertex);
    toVertex.set_padding(2);
    toVertex.set_border(2);
    toVertex.set_posy(220);
    toVertex.set_value(-1);

    top_box.add_child(ask2);
    ask2.set_message("enter the number of the to vertex");
    ask2.set_posy(200);

    bool works = false;

    while (!works)
    {

        separate_loop(top_box, true);

        from = fromVertex.get_value();
        to = toVertex.get_value();

        if ( (from != to && dest.vertex_exists(from) && dest.edge_exists(to)) || grman::key_press[KEY_ESC])
        {
            works = true;
        }

    }
}















