#include "graph.h"


/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    //m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
//    add_interfaced_vertex(0, 30.0, 0.5, 200, 100, "clown1.jpg");
//    add_interfaced_vertex(1, 60.0, 0.5, 400, 100, "clown2.jpg");
//    add_interfaced_vertex(2, 50.0, 0.5, 200, 300, "clown3.jpg");
//    add_interfaced_vertex(3, 0.0, 0.5, 400, 300, "clown4.jpg");
//    add_interfaced_vertex(4, 100.0, 0.5, 600, 300, "clown5.jpg");
//    add_interfaced_vertex(5, 0.0, 0.5, 100, 500, "bad_clowns_xx3xx.jpg", 0);
//    add_interfaced_vertex(6, 0.0, 0.5, 300, 500, "bad_clowns_xx3xx.jpg", 1);
//    add_interfaced_vertex(7, 0.0, 0.5, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 5.0);
    add_interfaced_edge(1, 0, 1, 5.0);
    add_interfaced_edge(2, 1, 3, 7.0);
    add_interfaced_edge(3, 4, 1, 2.0);
    add_interfaced_edge(4, 6, 3, 2.0);
    add_interfaced_edge(5, 7, 3, 2.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 10.0);
    add_interfaced_edge(8, 5, 2, 2.0);
    add_interfaced_edge(9, 3, 7, 8.0);
}

void Graph::make_test1()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    add_interfaced_vertex( 0, 30.0, 0.5, 100, 600, "herbe", "clown1.jpg");
    add_interfaced_vertex( 1, 60.0, 0.5, 100, 400, "lapin");
    add_interfaced_vertex( 2, 10.0, 0.5, 100, 200, "renard");

    add_interfaced_edge(0, 0, 1, 1);
    add_interfaced_edge(1, 1, 0, 1, Edge_type::Non_Trophic);
    add_interfaced_edge(2, 1, 2, 1);
    add_interfaced_edge(3, 2, 0, 1, Edge_type::Non_Trophic);
}


/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (m_interface)
    {
        for (auto &elt : m_vertices)
            elt.second.pre_update();

        for (auto &elt : m_edges)
            elt.second.pre_update();

        m_interface->m_top_box.update();

        for (auto &elt : m_vertices)
            elt.second.post_update();

        for (auto &elt : m_edges)
            elt.second.post_update();
    }
}

void Graph::turn()
{
    for (auto &elt : m_vertices)
        elt.second.turn(*this);

//    for (auto &elt : m_edges)
//        elt.second.turn();
}

///Ajout de sommet non interfacé
void Graph::add_vertex(std::string name, int idx, double value, double r)
{
    if ( m_vertices.find(idx) != m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, r, name);
}

///Ajout d'arete non interfacé
void Graph::add_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    m_edges[idx] = Edge(weight);
}


/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex( int idx, double value, double r, int x, int y,std::string name, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx) != m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, name, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, r, vi, name);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vertFrom, int id_vertTo, double weight, Edge_type type)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vertFrom)==m_vertices.end() || m_vertices.find(id_vertTo)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vertFrom << " and " << id_vertTo << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vertFrom], m_vertices[id_vertTo], type);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei, type);

    //on ajoute les indices des sommets à l'arc
    m_edges.at(idx).m_from = id_vertFrom;
    m_edges.at(idx).m_to = id_vertTo;

    //on donne l'indice de l'arc aux deux sommets
    m_vertices.at(id_vertFrom).m_out.push_back(idx);
    m_vertices.at(id_vertTo).m_in.push_back(idx);
}


//
//void Graph::fortementConnexes()
//{
//
//}



