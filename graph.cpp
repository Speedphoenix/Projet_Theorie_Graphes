#include "graph.h"

using namespace std;

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


void Graph::make_test1()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    //Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 1, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 1, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 1, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 1, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 1, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 1, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 1, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 1, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
}


/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (m_interface)
    {
        for (auto &elt : m_vertices)
            elt.second.pre_update(this);

        for (auto &elt : m_edges)
            elt.second.pre_update();

        m_interface->m_top_box.update();

        for (auto &elt : m_vertices)
            elt.second.post_update();

        for (auto &elt : m_edges)
            elt.second.post_update();
    }
}

///Ajout de sommet non interfacé
void Graph::add_vertex(int idx, double value, double r)
{
    if ( m_vertices.find(idx) != m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, r);
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
void Graph::add_interfaced_vertex(int idx, double value, double r, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx) != m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, r, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vertFrom, int id_vertTo, double weight)
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

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vertFrom], m_vertices[id_vertTo]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    //on ajoute les indices des sommets à l'arc
    m_edges.at(idx).m_from = id_vertFrom;
    m_edges.at(idx).m_to = id_vertTo;

    //on donne l'indice de l'arc aux deux sommets
    m_vertices.at(id_vertFrom).m_out.push_back(idx);
    m_vertices.at(id_vertTo).m_in.push_back(idx);
}


void Graph::reset_flags()
{
    for (auto &elem : m_vertices)
        elem.second.m_flag = false;
}

void Graph::reset_comps()
{
    for (auto &elem : m_vertices)
        elem.second.m_compNum = -1;
}

int Graph::getNewCompNum()
{
    int rep = 0;

    set<int> numList;
    for (auto & elem : m_vertices)
    {
        numList.insert(elem.second.m_compNum);
    }

    while (true)//pas si robuste mais bon...
    {
        if (numList.find(rep)==numList.end())
            break;
        else
            rep++;

    }

    return rep;
}



void Graph::fortementConnexes()
{
    vector<vector<int>> found;

    bool finished = false;

    reset_flags();
    reset_comps();

    while (!finished)
    {
        int currSommet = -1;

        for (auto &elem : m_vertices)
        {
            if (!elem.second.m_flag)
            {
                currSommet = elem.first;
                break;
            }
        }

        if (currSommet==-1)
        {
            finished = true;
        }
        else
        {
            //si le sommet n'a aucun prédécesseur, il est seul dans la composante connexe
            //pareil s'il n'a aucun suivant
            if (m_vertices.at(currSommet).m_in.empty() || m_vertices.at(currSommet).m_out.empty())
            {
                //pas besoin de passer par de longues boucles...
                m_vertices.at(currSommet).m_compNum = getNewCompNum();
                m_vertices.at(currSommet).m_flag = true;
            }
            else
            {
                vector<int> dump; //juste pour verifier / paranoïa. Normalement il devrait rester vide.
                set<int> receivedComps;

                sgadablouch(dump, currSommet, receivedComps);

                if (receivedComps.empty())
                    throw "receivedComps in fortementConnexes func is empty!!!!";
                if (!dump.empty())
                    throw "vector dump in fortementConnexes func is not empty!!!";

                flagRemaining(receivedComps);

            }
        }
    }

    reset_flags();
}


//le flag n'est levé QUE quand toute la composante est trouvée
//la fonction appelée dans fortementConnexes()
void Graph::sgadablouch(vector<int>& origin, int where, set<int>& passedBy)
{
    origin.push_back(where);
E("doifnf")

    //passedBy est un set contenant tous les sommets par lesquels on est passé.
    //la fonction insert empeche la duplication
    passedBy.insert(where);


    //elem est un auto d'aretes
    for (auto& elem : m_vertices.at(where).m_out)
    {E(10)
        //le sommet que l'on regarde maintenant
        int destVert = m_edges.at(elem).m_to;

        //s'il n'est pas déjà flagué
        if (!m_vertices.at(destVert).m_flag)
        {E(11)
            //numéro de la composante fortement connexe
            int compNum = m_vertices.at(destVert).m_compNum;

            //si il fait déjà partie de la même composante que le courrant, y'a pas besoin
            if (compNum!=-1 && compNum==m_vertices.at(where).m_compNum)
            {E(12)
                vector<int>::iterator it = find(origin.begin(), origin.end(), destVert);

                //si on a trouvé une boucle dans le graphe (pas forcément avec le départ)
                if (it!=origin.end())
                {E(13)
                    //numéro de la composante fortement connexe
                    int compNum = getNewCompNum(); //fonction à coder, donne un numéro pas encore utilisé

                    //on cherche si un des sommets précedent n'ont pas déjà un num
                    for (vector<int>::iterator passer = it;passer!=origin.end();it++)
                    {E(14)
                        //si il y avait déjà un num
                        if (m_vertices.at(*passer).m_compNum!=-1 && m_vertices.at(*passer).m_compNum!=compNum)
                        {E(15)
                            //préfere garder le déjà existant

                            unicompAllVert(compNum, m_vertices.at(*passer).m_compNum);

                            compNum = m_vertices.at(*passer).m_compNum;
                        }
                        else
                        {E(16)
                            m_vertices.at(*passer).m_compNum = compNum;
                        }
                    } //fin du for: on cherche si un des sommets précedent n'ont pas déjà un num
                }
                else //si on a pas (encore) fait de boucle
                {E(17)
                    //on appelle cette fonction recursivement pour le prochain
                    sgadablouch(origin, destVert, passedBy);
                } //fin du if/else: si on a trouvé une boucle dans le graphe (pas forcément avec le départ)
            } //fin du if: si il fait déjà partie de la même composante que le courrant, y'a pas besoin
        } //fin du if: s'il n'est pas déjà flagué
    } // fin de la boucle for générale des aretes suivantes du sommet en param

    //on enlève le sommet courant du vector
    origin.pop_back();
}


//colorie tous les sommets ayant comme numero de comp 'old' et leur donne 'new'
void Graph::unicompAllVert(int ancien, int nouveau)
{
    for (auto & elem : m_vertices)
    {
        if (elem.second.m_compNum==ancien)
        {
            elem.second.m_compNum = nouveau;
        }
    }
}

//flague tous les sommets par lesquels on est passé
//et donne à ceux tout seuls un numéro de composante
void Graph::flagRemaining(set<int>& receivedComps)
{
    int i = 0;
    for (auto & elem : receivedComps)
    {
        E(i++)
        //ça veut dire que ce sommet est seul dans sa composante connexe forte
        if (m_vertices.at(elem).m_compNum == -1)
        {
            E("oh non")
            m_vertices.at(elem).m_compNum = getNewCompNum();
        }

        m_vertices.at(elem).m_flag = true;
    }
}








