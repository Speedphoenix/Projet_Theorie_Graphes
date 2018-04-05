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
    m_main_box.set_dim(w, h);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


Graph::Graph (std::string filename)
{
    ifstream myFile(filename, ios::in);

    get_stream(myFile);

    myFile.close();
}

Graph::Graph (std::istream& file)
{
    get_stream(file);
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


void Graph::send_stream(ostream& myStream)
{
    Coords someCoords;

    if (m_interface==nullptr)
        myStream << 0 << " ";
    else
    {
        someCoords = m_interface->m_main_box.get_dim();
        myStream << 1 << " " << someCoords;
    }

    myStream << " ;" << endl; //pour bien séparer les vals

    myStream << m_vertices.size() << " ;" << endl;

    for (auto& elem : m_vertices)
    {
        //le contenu de cette boucle pourrait être mis dans un operator<<
        bool has_interface = elem.second.m_interface!=nullptr;
        myStream << elem.first << " "; // l'indice du sommet

        myStream << elem.second.m_value << " " << elem.second.m_r << " " << (int)has_interface << " ;" << endl;

        if (has_interface)
        {
            VertexInterface& interface = *(elem.second.m_interface);
            someCoords = interface.m_top_box.get_pos();
            myStream << someCoords << " ";

            if (interface.m_img.get_pic_name()!="")
            {
                myStream << 1 << " " << interface.m_img.get_pic_idx() << " ;" << endl;
                myStream << interface.m_img.get_pic_name() << endl; // faire un getline pour le prendre après
                myStream << interface.m_img.get_pic_idx() << " ;" << endl;
            }
            else
            {
                myStream << 0 << " ;" << endl;
            }
        }
    }

    myStream << endl << endl; //fin des sommets, début des aretes

    myStream << m_edges.size() << " ;" << endl;

    for (auto& elem : m_edges)
    {
        //le contenu de cette boucle pourrait être mis dans un operator<<
        bool has_interface = elem.second.m_interface!=nullptr;
        myStream << elem.first << " " << endl; //l'indice de l'arete

        myStream << elem.second.m_from << " " << elem.second.m_to << " " << elem.second.m_weight << endl;
        myStream << (int) has_interface << " ;" << endl;
    }
}

void Graph::get_stream(istream& myStream)
{
    //val et val2 sont des entiers pour quand y'en a besoin
    int val, val2, container_size = 0;
    string dump;
    Coords someCoords;

    myStream >> val;

    if (val)
    {
        myStream >> someCoords;

        m_interface = make_shared<GraphInterface>(0, 0, someCoords.x, someCoords.y);
    }
    else
    {
        m_interface = nullptr;
    }

    //la ligne n'etait pas terminée
    getline(myStream, dump); //pour bien séparer les vals


    //la nombre de sommets
    myStream >> container_size;


    getline(myStream, dump);


    for (int i=0;i<container_size;i++)
    {
        //les variables à passer en param des func de création de vertex
        int idx;
        double value, r;
        string pic_name = "";
        int pic_idx = 0;

        myStream >> idx;
        myStream >> value >> r >> val;
        getline(myStream, dump);

        //ici val est un bool si le vertex a une interface ou non

        if (val)
        {
            //les coordonnées
            myStream >> someCoords >> val2;
            //val2 est ici un booleen de si il y a une image ou non
            getline(myStream, dump);

            if (val2)
            {
                getline(myStream, pic_name);
                myStream >> pic_idx;
                getline(myStream, dump);
            }

            add_interfaced_vertex(idx, value, r, someCoords.x, someCoords.y, pic_name, pic_idx);
        }
        else
        {
            add_vertex(idx, value, r);
        }
    } //fin de la boucle for (for chaque sommet)

    myStream >> container_size;
    getline(myStream, dump);

    for (int i=0;i<container_size;i++)
    {
        int idx, from, to;
        double weight = 0;

        myStream >> idx;

        myStream >> from >> to >> weight;

        myStream >> val;

        getline(myStream, dump);

        if (val)
        {
            add_interfaced_edge(idx, from, to, weight);
        }
        else
        {
            add_edge(idx, from, to, weight);
        }
    } //fin de la boucle des aretes
}


ostream& operator<<(ostream& myStream, Graph& what)
{
    what.send_stream(myStream);
    return myStream;
}

istream& operator>>(istream& myStream, Graph& what)
{
    what.get_stream(myStream);
    return myStream;
}


void Graph::make_test1()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 908, 720);

    add_interfaced_vertex( 0, 30.0, 0.5, 100, 600, "herbe", "clown1.jpg");
    add_interfaced_vertex( 1, 60.0, 1, 100, 400, "lapin");
    add_interfaced_vertex( 2, 10.0, 0.6, 100, 200, "renard");

    add_interfaced_edge(0, 0, 1, 3);
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

                reconnexite(dump, currSommet, receivedComps);

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
void Graph::reconnexite(vector<int>& origin, int where, set<int>& passedBy)
{
    origin.push_back(where);

    //passedBy est un set contenant tous les sommets par lesquels on est passé.
    //la fonction insert empeche la duplication
    passedBy.insert(where);


    //elem est un auto d'aretes
    for (auto& elem : m_vertices.at(where).m_out)
    {
        //le sommet que l'on regarde maintenant
        int destVert = m_edges.at(elem).m_to;

        //s'il n'est pas déjà flagué
        if (!m_vertices.at(destVert).m_flag)
        {
            //numéro de la composante fortement connexe
            int compNum = m_vertices.at(destVert).m_compNum;

            //si il fait déjà partie de la même composante que le courrant, y'a pas besoin
            if (compNum==-1 || compNum!=m_vertices.at(where).m_compNum)
            {
                vector<int>::iterator it = find(origin.begin(), origin.end(), destVert);

                //si on a trouvé une boucle dans le graphe (pas forcément avec le départ)
                if (it!=origin.end())
                {
                    //numéro de la composante fortement connexe
                    compNum = getNewCompNum(); //fonction à coder, donne un numéro pas encore utilisé

                    //on cherche si un des sommets précedent n'ont pas déjà un num
                    for (vector<int>::iterator passer = it;passer!=origin.end();passer++)
                    {
                        //si il y avait déjà un num
                        if (m_vertices.at(*passer).m_compNum!=-1 && m_vertices.at(*passer).m_compNum!=compNum)
                        {
                            //préfere garder le déjà existant

                            unicompAllVert(compNum, m_vertices.at(*passer).m_compNum);

                            compNum = m_vertices.at(*passer).m_compNum;
                        }
                        else
                        {
                            m_vertices.at(*passer).m_compNum = compNum;
                        }
                    } //fin du for: on cherche si un des sommets précedent n'ont pas déjà un num
                }
                else //si on a pas (encore) fait de boucle
                {
                    //on appelle cette fonction recursivement pour le prochain
                    reconnexite(origin, destVert, passedBy);
                } //fin du if/else: si on a trouvé une boucle dans le graphe (pas forcément avec le départ)
            } //fin du if: si il fait déjà partie de la même composante que le courrant, y'a pas besoin
        } //fin du if: s'il n'est pas déjà flagué
    } // fin de la boucle for générale des aretes suivantes du sommet en param

    //on enlève le sommet courant du vector
    origin.pop_back();
}


// 'colorie' (change nompNum) tous les sommets ayant comme numero de comp 'old' et leur donne 'new'
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
    for (auto & elem : receivedComps)
    {
        //ça veut dire que ce sommet est seul dans sa composante connexe forte
        if (m_vertices.at(elem).m_compNum == -1)
        {
            m_vertices.at(elem).m_compNum = getNewCompNum();
        }

        m_vertices.at(elem).m_flag = true;
    }
}








