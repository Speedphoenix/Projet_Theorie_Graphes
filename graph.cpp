#include "graph.h"

using namespace std;

/***************************************************
                    GRAPH
****************************************************/

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/

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

void Graph::initialize_toolbox()
{
    //si la toolbox n'est pas encore initialisée
    if (!m_toolbox.m_interface)
    {
        m_toolbox.m_interface = make_shared<ToolboxInterface>(m_interface->m_tool_box.get_dimx()-2, m_interface->m_tool_box.get_dimy()-2);

        m_interface->m_tool_box.add_child(m_toolbox.m_interface->m_top_box);
    }
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

        //si le nom est vide, le getline prendra bien string vide
        myStream << elem.second.m_name << endl;

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

        int edgeType = 0;
        switch (elem.second.m_type) //enum Edge_type
        {
            case Edge_type::Trophic:
            edgeType = 1;
        break;

            case Edge_type::Non_Trophic:
            edgeType = 2;
        break;

            default:
            edgeType = 0;
        break;
        }

        myStream << edgeType << " ;" << endl;
    }
}

void Graph::get_stream(istream& myStream)
{
    //val et val2 sont des entiers pour quand y'en a besoin
    int val, val2, container_size = 0;
    string dump;
    Coords someCoords;


    reset_graph();

    myStream >> val;

    if (val)
    {
        myStream >> someCoords;

        m_interface = make_shared<GraphInterface>(0, 0, someCoords.x, someCoords.y);

        initialize_toolbox();
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
        string vertex_name = "";
        int pic_idx = 0;

        myStream >> idx;
        myStream >> value >> r >> val;
        getline(myStream, dump);

        getline(myStream, vertex_name);

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

            add_interfaced_vertex(idx, value, r, someCoords.x, someCoords.y, vertex_name, pic_name, pic_idx);
        }
        else
        {
            add_vertex(vertex_name, idx, value, r);
        }
    } //fin de la boucle for (for chaque sommet)


    myStream >> container_size;
    getline(myStream, dump);

    //boucle de chaque arete
    for (int i=0;i<container_size;i++)
    {
        int idx, from, to;
        double weight = 0;

        myStream >> idx;

        myStream >> from >> to >> weight;

        myStream >> val;

        getline(myStream, dump);

        myStream >> val2;

        Edge_type type;
        switch (val2) //enum Edge_type
        {
            case 1:
            type = Edge_type::Trophic;
        break;

            case 2:
            type = Edge_type::Non_Trophic;
        break;

            case 0:
            default:
            throw "tried taking an edge of unknown type. (in Graph::get_stream() func)";
        break;
        }

        getline(myStream, dump);

        if (val)
        {
            add_interfaced_edge(idx, from, to, weight, type);
        }
        else
        {
            add_edge(idx, from, to, weight, type);
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

    initialize_toolbox();

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
        {
            elt.second.pre_update();
        }

        for (auto &elt : m_edges)
            elt.second.pre_update();

        m_toolbox.pre_update();


        //update of all Widgets
        m_interface->m_top_box.update();


        m_selected_vertices.clear();
        for (auto &elt : m_vertices)
        {
            elt.second.post_update();

            if (elt.second.m_selected)
                m_selected_vertices.push_back(elt.first);
        }

        m_selected_edges.clear();
        for (auto &elt : m_edges)
        {
            elt.second.post_update();

            if (elt.second.m_selected)
                m_selected_edges.push_back(elt.first);
        }

        if (!m_selected_edges.empty() || !m_selected_vertices.empty())
        {
            m_toolbox.set_selection(true);
        }
        else
        {
            m_toolbox.set_selection(false);
        }

        m_toolbox.post_update();

        processInput(m_toolbox.m_user_choice);

        if (m_toolbox.m_continuous_turn)
        {
            ///FAIRE DES TOURS CONTINUS
        }
    }
}



//enlève une arete (et l'enlève des sommets)
void Graph::remove_edge(int id)
{
    map<int, Edge>::iterator it_remove = m_edges.find(id);

    if (it_remove==m_edges.end())
        return ;

    Edge& toRemove = m_edges.at(id);

    int sommet = toRemove.m_from;

    vector<int>::iterator it = find(m_vertices.at(sommet).m_out.begin(), m_vertices.at(sommet).m_out.end(), id);

    if (it == m_vertices.at(sommet).m_out.end())
    {
        throw "L'arete à suprimer n'est pas dans le sommet sommet de depart...";
    }

    m_vertices.at(sommet).m_out.erase(it);

    //maintenant l'arete n'est plus dans m_from

    sommet = toRemove.m_to;

    it = find(m_vertices.at(sommet).m_in.begin(), m_vertices.at(sommet).m_in.end(), id);

    if (it == m_vertices.at(sommet).m_in.end())
    {
        throw "L'arete à suprimer n'est pas dans le sommet d'arrivée...";
    }

    m_vertices.at(sommet).m_in.erase(it);

    //maintenant l'arete n'est plus dans m_to

    if (m_interface)
        m_interface->m_main_box.remove_child(toRemove.m_interface->m_top_edge);

    //pas besoin de delete m_interface de l'arete car c'est contenu dans un shared ptr

    //maintenant l'arete id n'existe plus
    m_edges.erase(it_remove);
}


void Graph::remove_vertex(int id)
{
    map<int, Vertex>::iterator it_remove = m_vertices.find(id);

    if (it_remove==m_vertices.end())
        return ;

    Vertex& toRemove = m_vertices.at(id);

    vector<int> edgesToRemove;

    //on fait la liste des aretes à supprimer
    for (unsigned i=0;i<toRemove.m_in.size();i++)
    {
        edgesToRemove.push_back(toRemove.m_in.at(i));
    }

    for (unsigned i=0;i<toRemove.m_out.size();i++)
    {
        edgesToRemove.push_back(toRemove.m_out.at(i));
    }

    //on les supprime une par une
    for (unsigned i=0;i<edgesToRemove.size();i++)
    {
        remove_edge(edgesToRemove.at(i));
    }

    //maintenant toutes les aretes connéctées à notre sommet n'éxistent plus
    if (m_interface && toRemove.m_interface)
        m_interface->m_main_box.remove_child(toRemove.m_interface->m_top_box);

    //pas besoin de delete m_interface du sommet car c'est contenu dans un shared ptr

    m_vertices.erase(it_remove);
}


//une fonction qui agit en fonction du choix de l'utilisateur dans la toolbox
void Graph::processInput(UserAction what)
{
    //all these can't be declared inside the switch
    int k_value;
    bool worked;

    string name, filename;
    ofstream outfile;
    ifstream infile;
    //vector<int> composante;   //pour kConnexe normal
    vector<vector<int>> composante_heavy; // pour kConnexe heavy

    //these can't be declared inside the switch
    int integer1, integer2;

    switch (what)
    {
        default:
        case UserAction::Nothing:
        //nothing
    break;

        case UserAction::Quit:
        m_want_to_quit = true;
    break;

        case UserAction::NewGraph:
        reset_graph();
    break;

        case UserAction::LoadGraph:
        text_input(filename, "entrez le nom du fichier");

        infile.open(filename, ios::in);

        if (!infile)
        {
            cerr << "veuillez entrer un nom de fichier existant";
        }
        else
        {
            get_stream(infile); ///RESET LE GRAPHE DABORD

            infile.close();
        }
    break;

        case UserAction::SaveGraph:

        text_input(filename, "entrez le nom du fichier");

        outfile.open(filename, ios::out | ios::trunc);

        if (!outfile)
        {
            cerr << "veuillez entrer un nom de fichier valable....";
        }
        else
        {
            send_stream(outfile);

            outfile.close();
        }
    break;

        case UserAction::AddVertex:
        new_vertex_values(name, filename);

        add_interfaced_vertex(getUnusedVertexId(), default_value, default_r, default_x, default_y, name, filename);

    break;

        case UserAction::AddEdge:
        new_edge_tips(*this, integer1, integer2);

        add_interfaced_edge(getUnusedEdgeId(), integer1, integer2);

    break;

        case UserAction::Delete:
        //que s'il y a des trucs à supprimer
        if (m_selected_edges.empty() && m_selected_vertices.empty())
            break;

        //delete the edges first so don't have to check if they were deleted with a vertex...
        for (auto& elem : m_selected_edges)
            remove_edge(elem);

        for (auto& elem : m_selected_vertices)
            remove_vertex(elem);
    break;

        case UserAction::HardConnex:
        fortementConnexes();
    break;

        case UserAction::KConnex:

//        composante.clear(); //should already be empty
        composante_heavy.clear(); //should already be empty


//        k_value = kConnexe(composante, max_k_connexe, worked);
        k_value = kConnexe_heavy(composante_heavy, max_k_connexe, worked);

        if (worked)
        {
            cout << endl << "le graphe a une k-connexité de " << k_value << endl;
            for (auto& elem_external : composante_heavy)
            {
                cout << "une combinaison de sommets à enlever est : ";
                for (auto& elem : elem_external) //composante)
                    cout << elem << " ";
                cout << endl;
            }
            cout << endl;
        }
        else
        {
            cout << endl << "le graphe est entièrement connexe ou a une k-connexité supèrieure à ";
            cout << max_k_connexe << endl;
        }

    break;

        case UserAction::Turn:

    break;
    }
}


void Graph::turn()
{
    for (auto &elt : m_vertices)
        elt.second.turn(*this);

//    for (auto &elt : m_edges)
//        elt.second.turn();
}


void Graph::add_vertex(Vertex& source, int id)
{
    add_vertex(source.m_name, id, source.m_value, source.m_r);
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


void Graph::add_edge(Edge& source, int id)
{
    add_edge(id, source.m_from, source.m_to, source.m_weight, source.m_type);
}

///Ajout d'arete non interfacé
void Graph::add_edge(int idx, int id_vertFrom, int id_vertTo, double weight, Edge_type type)
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

    m_edges[idx] = Edge(weight, type);

    //on ajoute les indices des sommets à l'arc
    m_edges.at(idx).m_from = id_vertFrom;
    m_edges.at(idx).m_to = id_vertTo;

    //on donne l'indice de l'arc aux deux sommets
    m_vertices.at(id_vertFrom).m_out.push_back(idx);
    m_vertices.at(id_vertTo).m_in.push_back(idx);
}


/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(Vertex& source, int id)
{
    VertexInterface* interface = source.m_interface.get();

    if (interface)
        add_interfaced_vertex(id, source.m_value, source.m_r,
            interface->m_top_box.get_posx(), interface->m_top_box.get_posy(), source.m_name,
            interface->m_img.get_pic_name(), interface->m_img.get_pic_idx());
    else
        add_vertex(source.m_name, id, source.m_value, source.m_r);
}


/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex( int idx, double value, double r, int x, int y, std::string name, std::string pic_name, int pic_idx )
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


void Graph::add_interfaced_edge(Edge& source, int id)
{
    EdgeInterface* interface = source.m_interface.get();

    if (interface)
        add_interfaced_edge(id, source.m_from, source.m_to, source.m_weight, source.m_type);
    else
        add_edge(id, source.m_from, source.m_to, source.m_weight, source.m_type);
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
    m_edges[idx] = Edge(weight, type, ei);

    //on ajoute les indices des sommets à l'arc
    m_edges.at(idx).m_from = id_vertFrom;
    m_edges.at(idx).m_to = id_vertTo;

    //on donne l'indice de l'arc aux deux sommets
    m_vertices.at(id_vertFrom).m_out.push_back(idx);
    m_vertices.at(id_vertTo).m_in.push_back(idx);
}


//pas très robuste mais bon
int Graph::getUnusedVertexId()
{
    int i = 0;

    while (m_vertices.find(i)!=m_vertices.end())
        i++;

    return i;
}

//pas très robuste mais bon
int Graph::getUnusedEdgeId()
{
    int i = 0;

    while (m_edges.find(i)!=m_edges.end())
        i++;

    return i;
}

//copie un autre graph
void Graph::operator=(Graph& source)
{
    reset_graph();

    if (source.m_interface)
    {
        int w = source.m_interface->m_main_box.get_dimx();
        int h = source.m_interface->m_main_box.get_dimy();

        m_interface = make_shared<GraphInterface>(0, 0, w, h);

        initialize_toolbox();

        for (auto& elem : source.m_vertices)
        {
            //l'éxistence de l'interface est testée à l'interieur
            add_interfaced_vertex(elem.second, elem.first);
        }

        for (auto& elem : source.m_edges)
        {
            //l'éxistence de l'interface est testée à l'interieur
            add_interfaced_edge(elem.second, elem.first);
        }
    }
    else
    {
        interfaceless(source);
    }
}

//copie un autre graph sans l'interface
void Graph::interfaceless(Graph& source)
{
    reset_graph();

    m_interface = nullptr;

    for (auto& elem : source.m_vertices)
    {
        add_vertex(elem.second, elem.first);
    }

    for (auto& elem : source.m_edges)
    {
        add_edge(elem.second, elem.first);
    }
}

//reset le graphe
void Graph::reset_graph()
{
    if (m_interface)
    {
        for (auto& elem : m_edges)
        {
            if (elem.second.m_interface)
            {
                m_interface->m_main_box.remove_child(elem.second.m_interface->m_top_edge);

                elem.second.m_interface = nullptr; //on sait jamais
            }
        }

        for (auto& elem : m_vertices)
        {
            if (elem.second.m_interface)
            {
                m_interface->m_main_box.remove_child(elem.second.m_interface->m_top_box);

                elem.second.m_interface = nullptr; //on sait jamais
            }
        }
    }

    m_edges.clear();
    m_vertices.clear();

    m_selected_edges.clear();
    m_selected_vertices.clear();
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


bool Graph::simplementConnexe()
{
    unsigned flag_count = 0;

    int some_vertex = m_vertices.begin()->first;

    reset_flags();

    dfs_recurs(some_vertex, flag_count);

    if (flag_count!=m_vertices.size())
        return false;
    else
        return true;
}

void Graph::dfs_recurs(int where, unsigned& flag_count)
{
    //pour economiser des lignes de code dans les boucles
    if (m_vertices.at(where).m_flag)
        return;
    else
    {
        m_vertices.at(where).m_flag = true;
        flag_count++;
    }

    if (flag_count==m_vertices.size())
        return;

    for (auto& elem : m_vertices.at(where).m_out)
    {
        dfs_recurs(m_edges.at(elem).m_to, flag_count);
    }

    for (auto& elem : m_vertices.at(where).m_in)
    {
        dfs_recurs(m_edges.at(elem).m_from, flag_count);
    }
}


//pas très optimisée...
//pourrait être optimisée par exemple:
//  en regardant d'abord des sommets avec une seule arete
//
//cette fonction ne regarde pas toutes les possibilités,
//elle ne prend qu'une des combinaisons ayant le plus petit k
//pour avoir toutes les combinaisons, utiliser kConnexe_heavy (encore moins optimisée)
int Graph::kConnexe(vector<int>& rep, int max_k, bool& worked)
{
    vector<int> temporary;

    int current_max = max_k;
    Graph inter;

    inter.interfaceless(*this);

    worked = false;

    //si le graphe est entrièrement connexe
    if (m_vertices.size()<=1)
    {
        //on s'arrete là
        return max_k;
    }

    for (auto& elem : m_vertices)
    {
        Vertex& curr_vertex = elem.second;
        int curr_id = elem.first;

        inter.remove_vertex(curr_id);

        //si ce sommet ne permet pas (à lui seul) la connexité
        if (inter.simplementConnexe())
        {
            //pour enlever le non nécéssaire
            if (current_max>2 || (!worked && current_max>1))
            {
                int new_k_value;
                bool inter_worked;

                temporary.clear();

                //si on a pas encore trouvé, on veut trouver à la limite de current_max.
                //si on a déjà trouvé, c'est que on a déjà qqc à current_max, donc on essaye current_max-2
                new_k_value = inter.kConnexe(temporary, current_max - (worked?2:1), inter_worked);

                if (inter_worked)
                {
                    current_max = new_k_value + 1;
                    worked = true;
                    rep.clear();
                    rep.push_back(curr_id);
                    rep.insert(rep.end(), temporary.begin(), temporary.end());
                }
            }
        }
        else
        {
            current_max = 1;
            rep.clear();
            rep.push_back(curr_id);

            worked = true;

            //plus besoin de continuer la boucle
            break;
        }

        //on remet le sommet dans le graphe intermédiaire pour le prochain tour de boucle
        inter.add_vertex(curr_vertex, curr_id);

        for (auto& edgeElem : curr_vertex.m_in)
            inter.add_edge(m_edges.at(edgeElem), edgeElem);

        for (auto& edgeElem : curr_vertex.m_out)
            inter.add_edge(m_edges.at(edgeElem), edgeElem);
    }

    return current_max;
}



//l'ajout de recevoir toutes les combinaisons de k (au lieu de la première)
//a beaucoup allourdi cette fonction...
int Graph::kConnexe_heavy(vector<vector<int>>& rep, int max_k, bool& worked)
{
    vector<vector<int>> temporary;
    vector<int> inter_vect;

    int current_max = max_k;
    Graph inter;

    inter.interfaceless(*this);

    worked = false;

    //si le graphe est entrièrement connexe
    if (m_vertices.size()<=1)
    {
        //on s'arrete là
        return max_k;
    }

    for (auto& elem : m_vertices)
    {
        Vertex& curr_vertex = elem.second;
        int curr_id = elem.first;

        inter.remove_vertex(curr_id);

        //si ce sommet ne permet pas (à lui seul) la connexité
        if (inter.simplementConnexe())
        {
            //pour enlever le non nécéssaire
            if (current_max>1)
            {
                int new_k_value;
                bool inter_worked;

                temporary.clear();

                //si on a pas encore trouvé, on veut trouver à la limite de current_max.
                //si on a déjà trouvé, c'est que on a déjà qqc à current_max, donc on essaye current_max-2
                new_k_value = inter.kConnexe_heavy(temporary, current_max - 1, inter_worked);

                if (inter_worked)
                {
                    //si c'est pas la première k-connexité avec ce k trouvée
                    if (current_max==(new_k_value+1) && worked)
                    {
                        //on veut éviter les doublons qui ont un ordre différent
                        //l'operator== de vector compare chaque elément au même indice
                        // les vectors dans rep sont normalement déjà ordonnés

                        for (unsigned i=0;i<temporary.size();i++)
                        {
                            inter_vect.clear();
                            inter_vect.push_back(curr_id);
                            inter_vect.insert(inter_vect.end(), temporary.at(i).begin(), temporary.at(i).end());
                            sort(inter_vect.begin(), inter_vect.end());

                            //s'il n'y a pas déjà cette combinaison dans rep
                            if (find(rep.begin(), rep.end(), inter_vect)==rep.end())
                            {
                                ///pas sûr que ça marche...
                                rep.push_back(inter_vect);
                            }
                        }
                    }
                    else
                    {
                        current_max = new_k_value + 1;
                        worked = true;
                        rep.clear();

                        for (unsigned i=0;i<temporary.size();i++)
                        {
                            temporary.at(i).push_back(curr_id);
                            //si la fonction sort est mal codée, ça pourrait pomper enormément de processeur.
                            sort (temporary.at(i).begin(), temporary.at(i).end());

                            ///pas sûr que ça marche...
                            rep.push_back(temporary.at(i));
                        }
                    }
                }
            }
        }
        else
        {
            if (current_max>1)
                rep.clear();

            //pas besoin de tester si le sommet a déjà été ajouté (differents tours de boucle...)
            inter_vect.clear();
            inter_vect.push_back(curr_id);
            rep.push_back(inter_vect);

            current_max = 1;

            worked = true;
        }

        //on remet le sommet dans le graphe intermédiaire pour le prochain tour de boucle
        inter.add_vertex(curr_vertex, curr_id);

        for (auto& edgeElem : curr_vertex.m_in)
            inter.add_edge(m_edges.at(edgeElem), edgeElem);

        for (auto& edgeElem : curr_vertex.m_out)
            inter.add_edge(m_edges.at(edgeElem), edgeElem);
    }

    return current_max;
}


