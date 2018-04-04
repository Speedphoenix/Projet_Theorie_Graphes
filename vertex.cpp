#include "vertex.h"

//pas de problème dansun .cpp
#include "edge.h"
#include "graph.h"

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(std::string name, int idx, int x, int y, std::string pic_name, int pic_idx)
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

    //Le nom
    m_name.set_message( name );

    m_top_box.add_child( m_box_name );
    m_box_name.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Up);
    m_box_name.set_dim(10 * m_name.get_message().size(), 12);
    m_box_name.set_bg_color(BLANC);

    m_top_box.add_child( m_name );
    m_name.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Up);



}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (m_interface)
    {
        /// Copier la valeur locale de la donnée m_value vers le slider associé
        m_interface->m_slider_value.set_value(m_value);

        /// Copier la valeur locale de la donnée m_value vers le label sous le slider
        m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
    }
}




/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}


void Vertex::turn(Graph& g)
{
    double k = 0;
    double n = m_value;
    Edge temp;
    double coef = 0;
    double n_proie = 0;


    //Actualisation de m_value
    ///Model 1 : on considere que si un sommet n'a aucune arete sortante, alors son N est constant
    if(! m_in.empty())
    {
        for(unsigned i = 0 ; i < m_in.size() ; i++)
        {
            temp = g.getEdge(m_in[i]);

            // k = Coef_herbe* N_herbe + Coef_carrottes *N_carrottes
            coef = temp.m_weight;
            n_proie = g.getVertex(temp.m_from).m_value;

            k += coef * n_proie;
        }

        m_value = n + m_r * n *(1 - n / k);
    }

    if(m_value < 0)
        m_value = 0;
}


