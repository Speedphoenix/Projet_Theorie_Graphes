#include "vertex.h"

//pas de problème dansun .cpp
#include "edge.h"
#include "graph.h"

/***************************************************
                    VERTEX
****************************************************/

using namespace std;

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string name, std::string pic_name, int pic_idx)
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
    m_box_name.set_dim(10 * m_name.get_message().size(), 9);
    m_box_name.set_bg_color(BLANC);

    m_top_box.add_child( m_name );
    m_name.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Up);


    m_top_box.add_child( m_label_comp);
    m_label_comp.set_message( std::to_string(-1) );
    m_label_comp.set_color(ROSECLAIR);
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

        m_interface->m_label_comp.set_message( std::to_string(m_compNum));
    }
}




/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();

    //si la valeur est en train d'être entrée en texte, on prend celle là
    if (m_interface->m_label_value.is_typing())
        m_value = m_interface->m_label_value.get_value();
}


void Vertex::turn(Graph& g)
{
    double k = 0;
    double n = m_value;
    Edge temp;
    double coef_proie = 0;
    double coef_pred = 0;
    double pred_tot = 0;
    double n_proie = 0;
    double n_pred = 0;

    ///E(endl) E(m_value)

    //Actualisation de m_value
    ///Model 1 : on considere que si un sommet n'a aucune arete entrante, alors son N est constant
    if(! m_in.empty())
    {
        //Ce que notre sommet mange
        for(unsigned i = 0 ; i < m_in.size() ; i++)
        {
            //temps correspond à l'arete entrante
            temp = g.getEdge(m_in[i]);

            // k = Coef_herbe->lapin * N_herbe + Coef_carrottes->lapin *N_carrottes
            coef_proie = temp.m_weight;
            n_proie = g.getVertex(temp.m_from).m_value;

            k += coef_proie * n_proie;
        }
        //Notre vertex est mangé par ses prédateurs
        for(unsigned i = 0 ; i < m_out.size() ; i++ )
        {
            temp = g.getEdge(m_out[i]);

            //pred_tot = coef_lapin->renard * n_renard - coef_lapin->loup * n_loup...
            coef_pred = temp.m_weight;
            n_pred = g.getVertex(temp.m_to).m_value;

            pred_tot += coef_pred * n_pred;

        }

        ///DÉCOMMENTE POUR DEBUG
//E(n) E(m_r) E(k) E(n/k) E((m_r * n *(1 - n / k))) E(pred_tot) E(coef_pred) E(n_pred)

        m_value = n + m_r * n *(1 - n / k) - pred_tot ;
    }

    if(m_value < 0 || isnan(m_value))
        m_value = 0;

}

