#include "graph.h"

using namespace std;

/***************************************************
                    GRAPH
****************************************************/

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    AdaptÃ© par le groupe nÂ°2 du TD09, ING2 PROMO 2021 Ã  l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
/// Les paramÃ¨tres en entrÃ©e ne servent Ã  rien
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908, 670);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    m_top_box.add_child(m_console_box);
    m_console_box.set_dim(908, 50);
    m_console_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_console_box.set_bg_color(BLANCROSE);
    m_console_box.add_child(m_console_text_l1);
    m_console_text_l1.set_posy(15);
    m_console_box.add_child(m_console_text_l2);
    m_console_text_l2.set_posy(30);
}


Graph::Graph (std::string filename)
{
    ifstream myFile(filename, ios::in);

    if (myFile)
    {
        get_stream(myFile);
        myFile.close();
    }
    else
    {
        myFile.open(GRAPHFOLDER + filename, ios::in);

        if (myFile)
        {
            get_stream(myFile);
            myFile.close();
        }
        else
            cerr << endl << "couldn't find file" << endl;
    }


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

void Graph::make_test1()
{
    reset_graph();
    m_interface = std::make_shared<GraphInterface>(50, 0, 908, 720);
    initialize_toolbox();

    add_interfaced_vertex( 0, 20.0,   3, 250, 500, "herbe", Vertex_type::Exp);
    add_interfaced_vertex( 1, 20.0, 1.1, 100, 300, "lapin");
    add_interfaced_vertex( 2, 30.0, 1.2, 400, 300, "souris");
    add_interfaced_vertex( 3, 10.0, 0.3, 250, 100, "renard");
    //add_interfaced_vertex( 4, 10.0,   0, 350, 400, "pollution");

    add_interfaced_edge(0, 0, 1, 1.0);
    //add_interfaced_edge(1, 1, 0, 1, Edge_type::Non_Trophic);
    add_interfaced_edge(1, 1, 3, 0.5);
    //add_interfaced_edge(3, 3, 0, 1, Edge_type::Non_Trophic);
    //add_interfaced_edge(3, 4, 1, 0.5,Edge_type::Non_Trophic);
    add_interfaced_edge(2, 0, 2, 1.2);
    add_interfaced_edge(3, 2, 3, 0.7);

}


void Graph::send_stream(ostream& myStream)
{
    Coords someCoords;

    if (!myStream)
        throw "File couldn't open properly (send_stream())";

    if (m_interface==nullptr)
        myStream << 0 << " ";
    else
    {
        someCoords = m_interface->m_main_box.get_dim();
        myStream << 1 << " " << someCoords;
    }

    myStream << " ;" << endl; //pour bien sÃ©parer les vals

    myStream << m_vertices.size() << " ;" << endl;

    for (auto& elem : m_vertices)
    {
        //le contenu de cette boucle pourrait Ãªtre mis dans un operator<<
        bool has_interface = elem.second.m_interface!=nullptr;
        myStream << elem.first << " "; // l'indice du sommet

        myStream << elem.second.m_value << " " << elem.second.m_r << " ;" << endl;

        myStream << (int)has_interface << " " << vertex_to_int(elem.second.m_type) << " ;" << endl;

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
                myStream << interface.m_img.get_pic_name() << endl; // faire un getline pour le prendre aprÃ¨s
                myStream << interface.m_img.get_pic_idx() << " ;" << endl;
            }
            else
            {
                myStream << 0 << " ;" << endl;
            }
        }
    }

    myStream << endl << endl; //fin des sommets, dÃ©but des aretes

    myStream << m_edges.size() << " ;" << endl;

    for (auto& elem : m_edges)
    {
        //le contenu de cette boucle pourrait Ãªtre mis dans un operator<<
        bool has_interface = elem.second.m_interface!=nullptr;
        myStream << elem.first << " " << endl; //l'indice de l'arete

        myStream << elem.second.m_from << " " << elem.second.m_to << " " << elem.second.m_weight << endl;
        myStream << (int) has_interface << " ;" << endl;

        myStream << edge_to_int(elem.second.m_type) << " ;" << endl;
    }
}

void Graph::get_stream(istream& myStream)
{
    //val et val2 sont des entiers pour quand y'en a besoin
    int val, val2, typeNum, container_size = 0;
    string dump;
    Coords someCoords;

    if (!myStream)
        throw "File couldn't open properly (get_stream())";

    reset_graph();

    myStream >> val;

    if (val)
    {
        myStream >> someCoords;

        if (!m_interface)
        {
            m_interface = make_shared<GraphInterface>(0, 0, someCoords.x, someCoords.y);
        }

        initialize_toolbox();

    }
    else
    {
        m_interface = nullptr;
    }

    //la ligne n'etait pas terminÃ©e
    getline(myStream, dump); //pour bien sÃ©parer les vals


    //la nombre de sommets
    myStream >> container_size;


    getline(myStream, dump);


    for (int i=0;i<container_size;i++)
    {
        //les variables à passer en param des func de crÃ©ation de vertex
        int idx;
        double value, r;
        string pic_name = "";
        string vertex_name = "";
        int pic_idx = 0;

        myStream >> idx;
        myStream >> value >> r;// >> val; ///enlever val ici
        getline(myStream, dump);

        ///décomenter ici
        myStream >> val >> typeNum;
        getline(myStream, dump);
        Vertex_type type = int_to_vertex(typeNum);

        getline(myStream, vertex_name);

        //ici val est un bool si le vertex a une interface ou non

        if (val)
        {
            //les coordonnees
            myStream >> someCoords >> val2;
            //val2 est ici un booleen de si il y a une image ou non
            getline(myStream, dump);

            if (val2)
            {
                getline(myStream, pic_name);
                myStream >> pic_idx;
                getline(myStream, dump);
            }

            add_interfaced_vertex(idx, value, r, someCoords.x, someCoords.y, vertex_name, type, pic_name, pic_idx);
        }
        else
        {
            add_vertex(vertex_name, idx, value, r, type);
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

        myStream >> typeNum;

        Edge_type type = int_to_edge(typeNum);

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


/// La mÃ©thode update Ã  appeler dans la boucle de jeu pour les graphes avec interface
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
            turn((double)TEMPO_MAJ/1000); //tempo_maj est en gros le temps passé dans un tour de boucle (en vrai un peu plus...)
        }
    }
}



//enlÃ¨ve une arete (et l'enlÃ¨ve des sommets)
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
        throw "L'arete Ã  suprimer n'est pas dans le sommet sommet de depart...";
    }

    m_vertices.at(sommet).m_out.erase(it);

    //maintenant l'arete n'est plus dans m_from

    sommet = toRemove.m_to;

    it = find(m_vertices.at(sommet).m_in.begin(), m_vertices.at(sommet).m_in.end(), id);

    if (it == m_vertices.at(sommet).m_in.end())
    {
        throw "L'arete Ã  suprimer n'est pas dans le sommet d'arrivÃ©e...";
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

    //on fait la liste des aretes Ã  supprimer
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

    //maintenant toutes les aretes connÃ©ctÃ©es Ã  notre sommet n'Ã©xistent plus
    if (m_interface && toRemove.m_interface)
        m_interface->m_main_box.remove_child(toRemove.m_interface->m_top_box);

    //pas besoin de delete m_interface du sommet car c'est contenu dans un shared ptr

    m_vertices.erase(it_remove);
}


//une fonction qui agit en fonction du choix de l'utilisateur dans la toolbox
void Graph::processInput(UserAction what)
{
    GraphInterface& interface = *m_interface;

    //all these can't be declared inside the switch
    int k_value;
    bool worked;

    string name, filename;
    ofstream outfile;
    ifstream infile;
    //vector<int> composante;   //pour kConnexe normal
    vector<vector<int>> composante_heavy; // pour kConnexe heavy

    stringstream message_to_show;

    int integer1, integer2;

    interface.m_console_text_l1.set_message("");
    interface.m_console_text_l2.set_message("");


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

        infile.open(GRAPHFOLDER + filename, ios::in);

        if (!infile)
        {
            //ptet' que le fichier est à la racine au lieu de GRAPHFOLDER
            infile.open(filename, ios::in);

            if (!infile)
            {
                //cerr << "veuillez entrer un nom de fichier existant";
                interface.m_console_text_l1.set_message("veuillez entrer un nom de fichier existant");
            }
            else
            {
                get_stream(infile);

                infile.close();
            }
        }
        else
        {
            get_stream(infile);

            infile.close();
        }
    break;

        case UserAction::SaveGraph:

        text_input(filename, "entrez le nom du fichier");

        outfile.open(GRAPHFOLDER + filename, ios::out | ios::trunc);

        if (!outfile)
        {
            //cerr << "veuillez entrer un nom de fichier valable....";
            interface.m_console_text_l1.set_message("veuillez entrer un nom de fichier valable....");
        }
        else
        {
            send_stream(outfile);

            outfile.close();
        }
    break;

        case UserAction::AddVertex:
        new_vertex_values(name, filename);

        add_interfaced_vertex(getUnusedVertexId(), default_value, default_r, default_x, default_y, name, Vertex_type::Logistic, filename);

    break;

        case UserAction::AddEdge:
        new_edge_tips(*this, integer1, integer2);

        add_interfaced_edge(getUnusedEdgeId(), integer1, integer2, default_weight);

    break;

        case UserAction::Delete:
        //que s'il y a des trucs Ã  supprimer
        if (m_selected_edges.empty() && m_selected_vertices.empty())
            break;

        //delete the edges first so don't have to check if they were deleted with a vertex...
        for (auto& elem : m_selected_edges)
            remove_edge(elem);

        for (auto& elem : m_selected_vertices)
            remove_vertex(elem);
    break;

        case UserAction::HardConnex:
        integer1 = fortementConnexes();
        interface.m_console_text_l1.set_message("Nombre de composantes fortement connexes:");
        interface.m_console_text_l2.set_message(grman::to_string_prec(integer1));
    break;

        case UserAction::KConnex:

//        composante.clear(); //should already be empty
        composante_heavy.clear(); //should already be empty


//        k_value = kConnexe(composante, max_k_connexe, worked);
        k_value = kConnexe_heavy(composante_heavy, max_k_connexe, worked);

        if (worked)
        {
            //laissÃ© l'affichage console pour quand y'en a trop
            cout << endl << "le graphe a une k-connexitÃ© de " << k_value << endl;

            message_to_show.str(""); //pour le vider
            message_to_show << "le graphe a une k-connexite de ";
            message_to_show << k_value;
            message_to_show << ". Les combinaisons de sommets a enlever sont : ";

            interface.m_console_text_l1.set_message(message_to_show.str());

            message_to_show.str("");
            for (auto& elem_external : composante_heavy)
            {
                cout << "une combinaison de sommets Ã  enlever est : ";
                for (auto& elem : elem_external) //composante)
                {    message_to_show << elem << " ";
                    cout << elem << " ";
                }
                message_to_show << " ; ";
                cout << endl;
            }
            cout << endl;
            interface.m_console_text_l2.set_message(message_to_show.str());
        }
        else
        {
            message_to_show.str("");
            message_to_show << "Le graphe est entierement connexe ou a une k-connexite supÃ¨rieure a ";
            message_to_show << max_k_connexe;
            interface.m_console_text_l1.set_message(message_to_show.str());
//            cout << endl << "Le graphe est entiÃ¨rement connexe ou a une k-connexitÃ© supÃ¨rieure Ã  ";
//            cout << max_k_connexe << endl;
        }

    break;

        case UserAction::Turn:
        turn();
    break;
    }
}


void Graph::turn(double t)
{
    for (auto &elt : m_vertices)
        elt.second.turn2(*this, t);

//    for (auto &elt : m_edges)
//        elt.second.turn();
}


void Graph::add_vertex(Vertex& source, int id)
{
    add_vertex(source.m_name, id, source.m_value, source.m_r, source.m_type);
}

///Ajout de sommet sans interface
void Graph::add_vertex(std::string name, int idx, double value, double r, Vertex_type type)
{
    if ( m_vertices.find(idx) != m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, r, name, type);
}


void Graph::add_edge(Edge& source, int id)
{
    add_edge(id, source.m_from, source.m_to, source.m_weight, source.m_type);
}

///Ajout d'arete non interfacÃ©
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

    //on ajoute les indices des sommets Ã  l'arc
    m_edges.at(idx).m_from = id_vertFrom;
    m_edges.at(idx).m_to = id_vertTo;

    //on donne l'indice de l'arc aux deux sommets
    m_vertices.at(id_vertFrom).m_out.push_back(idx);
    m_vertices.at(id_vertTo).m_in.push_back(idx);
}


/// Aide Ã  l'ajout de sommets interfacÃ©s
void Graph::add_interfaced_vertex(Vertex& source, int id)
{
    VertexInterface* interface = source.m_interface.get();

    if (interface)
        add_interfaced_vertex(id, source.m_value, source.m_r,
            interface->m_top_box.get_posx(), interface->m_top_box.get_posy(),
            source.m_name, source.m_type,
            interface->m_img.get_pic_name(), interface->m_img.get_pic_idx());
    else
        add_vertex(source.m_name, id, source.m_value, source.m_r, source.m_type);
}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex( int idx, double value, double r, int x, int y, std::string name, Vertex_type type, std::string pic_name, int pic_idx)
{
    if ( m_vertices.find(idx) != m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // CrÃ©ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, name, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, r, vi, name, type);
}


void Graph::add_interfaced_edge(Edge& source, int id)
{
    EdgeInterface* interface = source.m_interface.get();

    if (interface)
        add_interfaced_edge(id, source.m_from, source.m_to, source.m_weight, source.m_type);
    else
        add_edge(id, source.m_from, source.m_to, source.m_weight, source.m_type);
}

/// Aide Ã  l'ajout d'arcs interfacÃ©s
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

    //on ajoute les indices des sommets Ã  l'arc
    m_edges.at(idx).m_from = id_vertFrom;
    m_edges.at(idx).m_to = id_vertTo;

    //on donne l'indice de l'arc aux deux sommets
    m_vertices.at(id_vertFrom).m_out.push_back(idx);
    m_vertices.at(id_vertTo).m_in.push_back(idx);
}


//pas trÃ¨s robuste mais bon
int Graph::getUnusedVertexId()
{
    int i = 0;

    while (m_vertices.find(i)!=m_vertices.end())
        i++;

    return i;
}

//pas trÃ¨s robuste mais bon
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
            //l'Ã©xistence de l'interface est testÃ©e Ã  l'interieur
            add_interfaced_vertex(elem.second, elem.first);
        }

        for (auto& elem : source.m_edges)
        {
            //l'Ã©xistence de l'interface est testÃ©e Ã  l'interieur
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


int Graph::fortementConnexes()
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
            //si le sommet n'a aucun prÃ©dÃ©cesseur, il est seul dans la composante connexe
            //pareil s'il n'a aucun suivant
            if (m_vertices.at(currSommet).m_in.empty() || m_vertices.at(currSommet).m_out.empty())
            {
                //pas besoin de passer par de longues boucles...
                m_vertices.at(currSommet).m_compNum = getNewCompNum();
                m_vertices.at(currSommet).m_flag = true;
            }
            else
            {
                vector<int> dump; //juste pour verifier / paranoÃ¯a. Normalement il devrait rester vide.
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

    vector<int> used_comps;
    for (auto & elem : m_vertices)
    {
        if (find(used_comps.begin(), used_comps.end(), elem.second.m_compNum)==used_comps.end())
            used_comps.push_back(elem.second.m_compNum);
    }

    reset_flags();

    return used_comps.size();
}


//le flag n'est levÃ© QUE quand toute la composante est trouvÃ©e
//la fonction appelÃ©e dans fortementConnexes()
void Graph::reconnexite(vector<int>& origin, int where, set<int>& passedBy)
{
    origin.push_back(where);

    //passedBy est un set contenant tous les sommets par lesquels on est passÃ©.
    //la fonction insert empeche la duplication
    passedBy.insert(where);


    //elem est un auto d'aretes
    for (auto& elem : m_vertices.at(where).m_out)
    {
        //le sommet que l'on regarde maintenant
        int destVert = m_edges.at(elem).m_to;

        //s'il n'est pas dÃ©jÃ  flaguÃ©
        if (!m_vertices.at(destVert).m_flag)
        {
            //numÃ©ro de la composante fortement connexe
            int compNum = m_vertices.at(destVert).m_compNum;

            //si il fait dÃ©jÃ  partie de la mÃªme composante que le courrant, y'a pas besoin
            if (compNum==-1 || compNum!=m_vertices.at(where).m_compNum)
            {
                vector<int>::iterator it = find(origin.begin(), origin.end(), destVert);

                //si on a trouvÃ© une boucle dans le graphe (pas forcÃ©ment avec le dÃ©part)
                if (it!=origin.end())
                {
                    //numÃ©ro de la composante fortement connexe
                    compNum = getNewCompNum(); //fonction Ã  coder, donne un numÃ©ro pas encore utilisÃ©

                    //on cherche si un des sommets prÃ©cedent n'ont pas dÃ©jÃ  un num
                    for (vector<int>::iterator passer = it;passer!=origin.end();passer++)
                    {
                        //si il y avait dÃ©jÃ  un num
                        if (m_vertices.at(*passer).m_compNum!=-1 && m_vertices.at(*passer).m_compNum!=compNum)
                        {
                            //prÃ©fere garder le dÃ©jÃ  existant

                            unicompAllVert(compNum, m_vertices.at(*passer).m_compNum);

                            compNum = m_vertices.at(*passer).m_compNum;
                        }
                        else
                        {
                            m_vertices.at(*passer).m_compNum = compNum;
                        }
                    } //fin du for: on cherche si un des sommets prÃ©cedent n'ont pas dÃ©jÃ  un num
                }
                else //si on a pas (encore) fait de boucle
                {
                    //on appelle cette fonction recursivement pour le prochain
                    reconnexite(origin, destVert, passedBy);
                } //fin du if/else: si on a trouvÃ© une boucle dans le graphe (pas forcÃ©ment avec le dÃ©part)
            } //fin du if: si il fait dÃ©jÃ  partie de la mÃªme composante que le courrant, y'a pas besoin
        } //fin du if: s'il n'est pas dÃ©jÃ  flaguÃ©
    } // fin de la boucle for gÃ©nÃ©rale des aretes suivantes du sommet en param

    //on enlÃ¨ve le sommet courant du vector
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

//flague tous les sommets par lesquels on est passÃ©
//et donne Ã  ceux tout seuls un numÃ©ro de composante
void Graph::flagRemaining(set<int>& receivedComps)
{
    for (auto & elem : receivedComps)
    {
        //Ã§a veut dire que ce sommet est seul dans sa composante connexe forte
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


//pas trÃ¨s optimisÃ©e...
//pourrait Ãªtre optimisÃ©e par exemple:
//  en regardant d'abord des sommets avec une seule arete
//
//cette fonction ne regarde pas toutes les possibilitÃ©s,
//elle ne prend qu'une des combinaisons ayant le plus petit k
//pour avoir toutes les combinaisons, utiliser kConnexe_heavy (encore moins optimisÃ©e)
int Graph::kConnexe(vector<int>& rep, int max_k, bool& worked)
{
    vector<int> temporary;

    int current_max = max_k;
    Graph inter;

    inter.interfaceless(*this);

    worked = false;

    //si le graphe est entriÃ¨rement connexe
    if (m_vertices.size()<=1)
    {
        //on s'arrete la
        return max_k;
    }

    for (auto& elem : m_vertices)
    {
        Vertex& curr_vertex = elem.second;
        int curr_id = elem.first;

        inter.remove_vertex(curr_id);

        //si ce sommet ne permet pas (Ã  lui seul) la connexitÃ©
        if (inter.simplementConnexe())
        {
            //pour enlever le non nÃ©cÃ©ssaire
            if (current_max>2 || (!worked && current_max>1))
            {
                int new_k_value;
                bool inter_worked;

                temporary.clear();

                //si on a pas encore trouvÃ©, on veut trouver Ã  la limite de current_max.
                //si on a dÃ©jÃ  trouvÃ©, c'est que on a dÃ©jÃ  qqc Ã  current_max, donc on essaye current_max-2
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

        //on remet le sommet dans le graphe intermediaire pour le prochain tour de boucle
        inter.add_vertex(curr_vertex, curr_id);

        for (auto& edgeElem : curr_vertex.m_in)
            inter.add_edge(m_edges.at(edgeElem), edgeElem);

        for (auto& edgeElem : curr_vertex.m_out)
            inter.add_edge(m_edges.at(edgeElem), edgeElem);
    }

    return current_max;
}



//l'ajout de recevoir toutes les combinaisons de k (au lieu de la premiÃ¨re)
//a beaucoup allourdi cette fonction...
int Graph::kConnexe_heavy(vector<vector<int>>& rep, int max_k, bool& worked)
{
    vector<vector<int>> temporary;
    vector<int> inter_vect;

    int current_max = max_k;
    Graph inter;

    inter.interfaceless(*this);

    worked = false;

    //si le graphe est entriÃ¨rement connexe
    if (m_vertices.size()<=1)
    {
        //on s'arrete lÃ 
        return max_k;
    }

    for (auto& elem : m_vertices)
    {
        Vertex& curr_vertex = elem.second;
        int curr_id = elem.first;

        inter.remove_vertex(curr_id);

        //si ce sommet ne permet pas (Ã  lui seul) la connexitÃ©
        if (inter.simplementConnexe())
        {
            //pour enlever le non nÃ©cÃ©ssaire
            if (current_max>1)
            {
                int new_k_value;
                bool inter_worked;

                temporary.clear();

                //si on a pas encore trouvÃ©, on veut trouver Ã  la limite de current_max.
                //si on a dÃ©jÃ  trouvÃ©, c'est que on a dÃ©jÃ  qqc Ã  current_max, donc on essaye current_max-2
                new_k_value = inter.kConnexe_heavy(temporary, current_max - 1, inter_worked);

                if (inter_worked)
                {
                    //si c'est pas la premiÃ¨re k-connexitÃ© avec ce k trouvÃ©e
                    if (current_max==(new_k_value+1) && worked)
                    {
                        //on veut Ã©viter les doublons qui ont un ordre diffÃ©rent
                        //l'operator== de vector compare chaque elÃ©ment au mÃªme indice
                        // les vectors dans rep sont normalement dÃ©jÃ  ordonnÃ©s

                        for (unsigned i=0;i<temporary.size();i++)
                        {
                            inter_vect.clear();
                            inter_vect.push_back(curr_id);
                            inter_vect.insert(inter_vect.end(), temporary.at(i).begin(), temporary.at(i).end());
                            sort(inter_vect.begin(), inter_vect.end());

                            //s'il n'y a pas dÃ©jÃ  cette combinaison dans rep
                            if (find(rep.begin(), rep.end(), inter_vect)==rep.end())
                            {
                                ///pas sÃ»r que Ã§a marche...
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
                            //si la fonction sort est mal codÃ©e, Ã§a pourrait pomper enormÃ©ment de processeur.
                            sort (temporary.at(i).begin(), temporary.at(i).end());

                            ///pas sÃ»r que Ã§a marche...
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

            //pas besoin de tester si le sommet a dÃ©jÃ  Ã©tÃ© ajoutÃ© (differents tours de boucle...)
            inter_vect.clear();
            inter_vect.push_back(curr_id);
            rep.push_back(inter_vect);

            current_max = 1;

            worked = true;
        }

        //on remet le sommet dans le graphe intermediaire pour le prochain tour de boucle
        inter.add_vertex(curr_vertex, curr_id);

        for (auto& edgeElem : curr_vertex.m_in)
            inter.add_edge(m_edges.at(edgeElem), edgeElem);

        for (auto& edgeElem : curr_vertex.m_out)
            inter.add_edge(m_edges.at(edgeElem), edgeElem);
    }

    return current_max;
}


