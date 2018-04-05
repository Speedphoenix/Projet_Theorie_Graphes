#include "toolbox.h"

ToolboxInterface::ToolboxInterface(int w, int h)
{
    int currenty = 0;

    //m_top_box fera partie de tool box de graphe
    m_top_box.set_dim(w, h);
    m_top_box.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
    m_top_box.set_padding(0);
    m_top_box.set_margin(0);
    m_top_box.set_border(0);

    //bouton d'ajout de sommet
    m_top_box.add_child(m_new_vertex_btn);
    m_new_vertex_btn.set_posy(currenty + BUTTONMARGIN); //x n'est pas nécéssaire car on utilise de la gravité
   m_new_vertex_btn.set_bg_color(VERTCLAIR); //trouver une bonne couleur svp!
    m_new_vertex_btn.set_dim(BUTTONWIDTH, BUTTONHEIGHT);
    m_new_vertex_btn.set_gravity_xy(grman::GravityX::Center, grman::GravityY::None);
    m_new_vertex_btn.set_margin(BUTTONMARGIN);

    //le message à mettre sur le bouton
    m_new_vertex_btn.add_child(m_new_vertex_msg);
    m_new_vertex_msg.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
    m_new_vertex_msg.set_color(VERTSOMBRE); //trouver une bonne couleur svp!
    m_new_vertex_msg.set_message("new vertex");

    currenty += BUTTONSPACE;

    //bouton d'ajout d'arete
    m_top_box.add_child(m_new_edge_btn);
    m_new_edge_btn.set_posy(currenty + BUTTONMARGIN);
   m_new_edge_btn.set_bg_color(VERTCLAIR); //trouver une bonne couleur svp!
    m_new_edge_btn.set_dim(BUTTONWIDTH, BUTTONHEIGHT);
    m_new_edge_btn.set_gravity_xy(grman::GravityX::Center, grman::GravityY::None);
    m_new_edge_btn.set_margin(BUTTONMARGIN);

    //le message à mettre sur le bouton
    m_new_edge_btn.add_child(m_new_edge_msg);
    m_new_edge_msg.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
    m_new_edge_msg.set_color(VERTSOMBRE); //trouver une bonne couleur svp!
    m_new_edge_msg.set_message("new edge");

    currenty += BUTTONSPACE;

    //bouton de suppression
    //ce bouton sera ajouté et enlevé lors des selections
    //m_top_box.add_child(m_delete_btn);

    m_delete_btn.set_posy(currenty + BUTTONMARGIN);
   m_delete_btn.set_bg_color(ROUGECLAIR); //trouver une bonne couleur svp!
    m_delete_btn.set_dim(BUTTONWIDTH, BUTTONHEIGHT);
    m_delete_btn.set_gravity_xy(grman::GravityX::Center, grman::GravityY::None);
    m_delete_btn.set_margin(BUTTONMARGIN);

    //le message à mettre sur le bouton
    m_delete_btn.add_child(m_delete_msg);
    m_delete_msg.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
    m_delete_msg.set_color(ROUGESOMBRE); //trouver une bonne couleur svp!
    m_delete_msg.set_message("delte");


//    currenty += BUTTONSPACE;
//
//    m_edit_btn.set_posy(currenty + BUTTONMARGIN);
/////   m_edit_btn.set_bg_color(); //trouver une bonne couleur svp! (du orange?)
//    m_edit_btn.set_dim(BUTTONWIDTH, BUTTONHEIGHT);
//    m_edit_btn.set_gravity_xy(grman::GravityX::Center, grman::GravityY::None);
//    m_edit_btn.set_margin(BUTTONMARGIN);
//
//    //le message à mettre sur le bouton
//    m_edit_btn.add_child(m_edit_msg);
//    m_edit_msg.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
/////    m_edit_msg.set_color(); //trouver une bonne couleur svp!

}

