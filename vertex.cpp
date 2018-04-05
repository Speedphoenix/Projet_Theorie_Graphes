#include "vertex.h"

//pas de problème dansun .cpp
#include "edge.h"

/***************************************************
                    VERTEX
****************************************************/

using namespace std;

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );

    m_top_box.add_child( m_label_comp);
    m_label_comp.set_message( std::to_string(-1) );
    m_label_comp.set_color(ROSECLAIR);
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update(Graph* g)
{
    std::vector<double> k;
//    double n = m_value;
    Edge temp;

    if (m_interface)
    {
        /// Copier la valeur locale de la donnée m_value vers le slider associé
        m_interface->m_slider_value.set_value(m_value);

        /// Copier la valeur locale de la donnée m_value vers le label sous le slider
        m_interface->m_label_value.set_message( std::to_string( (int)m_value) );

        m_interface->m_label_comp.set_message( std::to_string(m_compNum));
    }

//    //Actualisation de m_value
//    for(int i = 0 ; i < m_in.size() ; i++)
//    {
//        temp = g.getEdge(m_in[i]);
//        // k = Coef_herbe* N_herbe
//        k =  g.getVertex(temp.m_from).m_value * N_herbe ;
//    }
//
//    m_value += m_r * n * (1 - n / k);

}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}


