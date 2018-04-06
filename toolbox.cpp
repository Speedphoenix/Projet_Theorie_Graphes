#include "toolbox.h"

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

    //bouton dde création de graphe
    m_top_box.add_child(m_new_graph_btn);
    setup_button(m_new_graph_btn, currently, GRISCLAIR); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_new_graph_btn.get_message_widget(), ORANGE, "new graph");


    currently += BUTTONSPACE;


    //bouton dde sauvegarde de graphe
    m_top_box.add_child(m_save_graph_btn);
    setup_button(m_save_graph_btn, currently, GRISCLAIR); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_save_graph_btn.get_message_widget(), ORANGE, "save graph");


    currently += BUTTONSPACE;


    //bouton dde chargement de graphe
    m_top_box.add_child(m_load_graph_btn);
    setup_button(m_load_graph_btn, currently, GRISCLAIR); //trouver une bonne couleur svp!

    //le message à mettre sur le bouton
    setup_msg(m_load_graph_btn.get_message_widget(), ORANGE, "load graph");


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

    if (interface.m_new_graph_btn.clicked())
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









