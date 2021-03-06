#include "widget.h"

//ne pose pas de problème dans un .cpp
#include "grman.h"


/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/

/**
 *  Modifications:
 *  Les classes WidgetTextSaisie, WidgetNumSaisie, WidgetButtonText
*/

namespace grman
{

/*
    fonction prise de la réponse de hmjd
    https://stackoverflow.com/a/16606128/7207370
*/
//to_string mais avec max 2 chiffres après la virgule
//std::string to_string_prec(const double a_value)
//{
//    std::ostringstream out;
//    out << std::setprecision(2) << a_value;
//    return out.str();
//}




void rect_around(BITMAP *bmp, int color, int thickness=1, int receding=0)
{
    for (int i=0+receding; i<thickness+receding; ++i)
        rect(bmp, i, i, bmp->w-1-i, bmp->h-1-i, color);
}


/***************************************************
                WIDGET BASE CLASS
****************************************************/


void Widget::update()
{
    update_interact();
    update_pre_draw();
    update_draw();
}

/// Gestion des événements
void Widget::update_interact()
{
    create_frame_context();

    /// Propagation de l'update aux elements enfants
    /// On interagit en 1er avec les éléments ajoutés en dernier
    for (auto it=m_children.rbegin(); it!=m_children.rend(); ++it)
        (*it)->update_interact();

    if (!gui_focus && !gui_over && is_mouse_over() && captures_focus())
        gui_over = this;

    if ( is_gui_over() && mouse_click && captures_focus())
        gui_focus = this;

    if ( is_gui_over() ) //si la souris est dessus (peu importe si clic)
        interact_over();

    if ( is_gui_focus() ) //si la souris clique dessus
        interact_focus();

    if ( is_gui_leave() ) //si la souris relache le clic dessus
        interact_leave();

    if ( key_last!='\0' )
        interact_keybd();

        //si la souris a cliqué qqp mais pas ici
    if ( !is_gui_over() && gui_focus)
        interact_elsewhere();

    destroy_frame_context();
}

/// Gestion des affichages, 1ère passe pour fixer les cadres
/// (Nécessaire pour les liens : affichés au fond donc en 1er mais s'appuyant sur les positions des autres qui sont affichés après)
void Widget::update_pre_draw()
{
    create_frame_context();

    /// Propagation de l'update aux elements enfants
    for (auto &e : m_children)
        e->update_pre_draw();

    destroy_frame_context();
}


/// Gestion des affichages
void Widget::update_draw()
{
    create_frame_context();

    if (m_bg_color!=-1)
        clear_to_color(m_view_wb, m_bg_color);

    draw();

    /// Propagation de l'update aux elements enfants
    /// On affiche en dernier (devant les autres) les éléments ajoutés en dernier
    for (auto &e : m_children)
        e->update_draw();

    if (m_border)
        draw_border();

    destroy_frame_context();
}


void Widget::create_frame_context()
{
    /// Calculer absolute frame à partir de relative et absolute parent ( si parent, sinon page_frame )
    const Frame &abspar = m_parent ? m_parent->m_abs_frame : page_frame;

    m_abs_frame.pos.x = abspar.pos.x + m_frame.pos.x;
    m_abs_frame.pos.y = abspar.pos.y + m_frame.pos.y;

    m_abs_frame.dim = m_frame.dim;

    // Petits soucis de plantages avec les sub_bitmaps hors écran...
    int x = std::max(m_abs_frame.pos.x, 0.);
    int y = std::max(m_abs_frame.pos.y, 0.);
    int w = m_abs_frame.dim.x+std::min(m_abs_frame.pos.x, 0.);
    int h = m_abs_frame.dim.y+std::min(m_abs_frame.pos.y, 0.);

    m_view_wb = create_sub_bitmap(page, x, y, w, h);
    int inside = m_border + m_padding;
    m_view = create_sub_bitmap(m_view_wb, inside, inside, w-2*inside, h-2*inside);

    // Malheureusement ceci ne marche pas ... (à creuser)
    //set_clip_rect(m_view, std::max(-x, 0), std::max(-y, 0), w-1, h-1);
}

void Widget::destroy_frame_context()
{
    if (m_view)
        destroy_bitmap(m_view);
    m_view = nullptr;

    if (m_view_wb)
        destroy_bitmap(m_view_wb);
    m_view_wb = nullptr;

}

bool Widget::is_mouse_over()
{
    return     mouse_x>=m_abs_frame.pos.x && mouse_x<=m_abs_frame.pos.x+m_abs_frame.dim.x
               &&  mouse_y>=m_abs_frame.pos.y && mouse_y<=m_abs_frame.pos.y+m_abs_frame.dim.y;
}

void Widget::reframe()
{
    int pardimx = m_parent ? m_parent->get_dimx() : page_frame.dim.x;
    int pardimy = m_parent ? m_parent->get_dimy() : page_frame.dim.y;

    if (m_gravity_x==GravityX::Left)
        reset_posx( m_margin );
    else if (m_gravity_x==GravityX::Right)
        reset_posx( pardimx - m_margin - m_frame.dim.x );
    else if (m_gravity_x==GravityX::Center)
        reset_posx( (pardimx - m_frame.dim.x)/2. );

    if (m_gravity_y==GravityY::Up)
        reset_posy( m_margin );
    else if (m_gravity_y==GravityY::Down)
        reset_posy( pardimy - m_margin - m_frame.dim.y );
    else if (m_gravity_y==GravityY::Center)
        reset_posy( (pardimy - m_frame.dim.y)/2. );
}

void Widget::draw_border()
{
    rect_around(m_view_wb, get_border_color(), m_border);
}

/***************************************************
                        TEXT
****************************************************/

/// Extrêmement rudimentaire : à compléter !
void WidgetText::draw()
{
    if (!m_vertical)
        textprintf_ex(m_view, font, 0, 0, m_color, -1, "%s", m_message.c_str()); //pour éviter le warning
    else
        for (int i=0, y=0; i<(int)m_message.length(); ++i, y+=text_height(m_font))
            textprintf_ex(m_view, font, 0, y, m_color, -1, "%c", m_message[i]);
}


void WidgetText::set_message(std::string message)
{
    m_message = message;
    reframe_text_box();
}


void WidgetText::reframe_text_box()
{
    if (!m_vertical)
        set_dim( text_length(m_font, m_message.c_str()), text_height(m_font) );
    else
        set_dim( text_length(m_font, "M"), text_height(m_font)*m_message.length() );
    reframe();
}

void WidgetTextSaisie::reframe_text_box()
{
    if (m_message=="" && !m_vertical)
    {
        set_dim( text_length(m_font, m_message.c_str()) + 15, text_height(m_font) + 9);
    }
    else
        WidgetText::reframe_text_box();
}


void WidgetTextSaisie::interact_leave()
{
    m_isTyping = true;

    m_message = "";
}

void WidgetTextSaisie::interact_elsewhere()
{
    m_isTyping = false;
}

void WidgetTextSaisie::interact_keybd()
{
    if (key_last == '\0')
        return ;

    if (!m_isTyping)
        return ;


    if (key_press[KEY_0_PAD] || key_press[KEY_1_PAD] ||
        key_press[KEY_2_PAD] || key_press[KEY_3_PAD] ||
        key_press[KEY_4_PAD] || key_press[KEY_5_PAD] ||
        key_press[KEY_6_PAD] || key_press[KEY_7_PAD] ||
        key_press[KEY_8_PAD] || key_press[KEY_9_PAD] )
    {
        m_message += shrct[key_last - '0'];
    }
    else if ((key_last>='a' && key_last<='z') || //les caractères acceptés
        (key_last>='A' && key_last<='Z')    ||
        (key_last>='0' && key_last<='9')    ||
        key_last=='_' || key_last=='-'      ||
        key_last==' ' || key_last=='.'      )
    {
        m_message += key_last;
    }
    else if (key_press[KEY_BACKSPACE] && m_message.length()>0)
    {
        m_message.pop_back();
    }

    reframe_text_box();
}

void WidgetNumSaisie::interact_leave()
{
    m_value = 0;

    WidgetTextSaisie::interact_leave();
}

void WidgetNumSaisie::interact_keybd()
{
    if (key_last == '\0')
        return ;

    if (!m_isTyping)
        return ;

    //Si on entre une valeur entre 0 et 9
    if (key_last >= '0' && key_last <= '9')
    {
        int val = key_last - '0';

        if(!m_virgule)      //si la valeur n'a pas de virgule
        {
            m_value = (m_value)*10 + val;
        }
        else if(m_virgule)      //si c'est un nombre à virgule
        {
            int puissDix = 1;

            //la fonction pow utilise dess aproximations exponentielles...
            for (int i=0;i<m_exposant;i++)
            {
                puissDix *= 10;
            }

            m_value += val/puissDix;
            if((m_exposant>= 0) && (m_exposant<= m_max_exposant))
                m_exposant++;
        }
        m_message = to_string_prec(m_value);
    }
//    else if(key_last == '.' || key_last == ',')
//    {
//        m_virgule = true;
//        m_exposant = 1;
//        m_message += '.';
//    }

    if (key_press[KEY_BACKSPACE])           ///SI on choisit de supprimer la dernière valeur saisie
    {
        // si il n'y a pas de virgule
        if ( ((int) m_value) == m_value )
        {
            m_value = (int) (m_value/10);
        }
        // si il y a une virgule
        else
        {
            int puissDix = 1;

            //la fonction pow utilise dess aproximations exponentielles...
            for (int i=0 ; i < m_exposant-1 ; i++)
            {
                puissDix *= 10;
            }

            m_value = ( (int) (m_value*puissDix) ) * puissDix;

            m_exposant--;

            if (m_exposant==0)
            {
                m_virgule = false;
            }
        }

        m_message = to_string_prec(m_value);
    }
    reframe_text_box();
}



/***************************************************
                    CHECKBOX
****************************************************/

void WidgetCheckBox::draw()
{
    if (m_value)
    {
        thick_line(m_view, m_view->w/4, m_view->h/4,   m_view->w/2, m_view->h-3, 2, VERT );
        thick_line(m_view, m_view->w-3, 0,            m_view->w/2, m_view->h-3, 2, VERT );
    }
    else
    {
        thick_line(m_view, 0, 0,           m_view->w, m_view->h, 2, ROUGE );
        thick_line(m_view, 0, m_view->h,    m_view->w, 0,        2, ROUGE );
    }

}

void WidgetCheckBox::interact_focus()
{
    if ( mouse_click )
        m_value = !m_value;

}



/***************************************************
                    BUTTON
****************************************************/

void WidgetButton::interact_focus()
{
    if ( mouse_click )
        m_value = true;
}


WidgetButtonText::WidgetButtonText()
{
    add_child(m_message);
    m_message.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
}


/***************************************************
                    VSLIDER
****************************************************/

void WidgetVSlider::draw()
{
    int hhandle = get_hhandle();

    thick_line(m_view, m_view->w/2, hhandle,   m_view->w/2, m_view->h-hhandle-1, m_rail_ratio*m_view->w, m_rail_color );

    double range_pix = m_view->h - 2*hhandle + 1;
    double propor = (typed(m_value)-m_min)/(m_max-m_min); // pour supporter sur VSliderLog
    int ys = m_view->h - hhandle - propor*range_pix ;


    thick_line(m_view, 0, ys, m_view->w-1, ys, hhandle, m_handle_color );
}

void WidgetVSlider::interact_focus()
{
    if ( mouse_b )
    {
        int hhandle = get_hhandle();
        int yrel = mouse_y - (m_abs_frame.pos.y + get_bp() + hhandle);
        m_value = m_max-(m_max-m_min)*yrel/(m_view->h-2*hhandle);
        limit_to_range();
    }
}

void WidgetVSlider::interact_over()
{
    if ( key[KEY_UP] )
    {
        m_value += .01*(m_max-m_min);
        limit_to_range();
    }

    if ( key[KEY_DOWN] )
    {
        m_value -= .01*(m_max-m_min);
        limit_to_range();
    }
}



/***************************************************
                    IMAGE
****************************************************/

void WidgetImage::reframe()
{
    if (m_pic_name=="")
        return;

    BITMAP *pic = get_picture(m_pic_name);
    if (!pic)
        return;

    set_dim( pic->w/get_picture_nb(m_pic_name), pic->h );
}

void WidgetImage::draw()
{
    if (m_animate)
    {

        if ( ++m_animate_cpt_tempo >= m_animate_tempo )
        {
            m_animate_cpt_tempo = 0;
            m_pic_idx = (m_pic_idx+1) % get_picture_nb(m_pic_name);
        }
    }
    show_picture(m_view, m_pic_name, 0, 0, m_pic_idx);
}


/***************************************************
                    BOX
****************************************************/

void WidgetBox::interact_focus()
{
    if (mouse_click)
    {
        m_pos_start_move = m_frame.pos;
    }

    if (m_moveable)
    {
        m_frame.pos.x = m_pos_start_move.x + mouse_x-mouse_click_x;
        m_frame.pos.y = m_pos_start_move.y + mouse_y-mouse_click_y;

        if (m_contained)
        {
            if ( m_frame.dim.x+2*m_margin+2*get_parent_bp() <= get_parent_frame().dim.x )
            {
                if ( get_posx() < m_margin )
                    set_posx(m_margin);
                if ( m_frame.pos.x + m_frame.dim.x > get_parent_frame().dim.x - get_parent_bp() - m_margin)
                    m_frame.pos.x = get_parent_frame().dim.x - m_frame.dim.x - get_parent_bp() - m_margin;
            }

            if ( m_frame.dim.y+2*m_margin+2*get_parent_bp() <= get_parent_frame().dim.y )
            {
                if ( get_posy() < m_margin )
                    set_posy(m_margin);
                if ( m_frame.pos.y + m_frame.dim.y > get_parent_frame().dim.y - get_parent_bp() - m_margin)
                    m_frame.pos.y = get_parent_frame().dim.y - m_frame.dim.y - get_parent_bp() - m_margin;
            }
        }
    }

}


/***************************************************
                    EDGE
****************************************************/

void WidgetEdge::draw()
{
    if ( !(m_attach[0] && m_attach[1] ) )
        return;

    Coords p[2] {
        m_attach[0]->get_center_abs_pos(),
        m_attach[1]->get_center_abs_pos()
    };

    // Vecteur de centre à centre
    Coords vec_dir = p[1] - p[0];

    // Détermination des intersections avec les frames des 2 widgets liés
    p[0] = m_attach[0]->get_abs_frame().intersect(vec_dir);
    p[1] = m_attach[1]->get_abs_frame().intersect(-vec_dir);

    // Dessin du lien cadre à cadre
    thick_line(page, p[0].x, p[0].y, p[1].x, p[1].y, m_thickness, m_color);

    // Calcul du nouveau vecteur cadre à cadre
    vec_dir = p[1] - p[0];

    // Point d'ancrage pour les éventuels enfants ( Widgets attachés à l'arête )
    Coords anchor = p[0] + vec_dir * m_children_position - vec_dir.normalize().rotate_90()*m_children_lateral;
    set_pos( anchor - get_parent_abs_frame().pos );


    /// La suite concerne les éléments de décorations, flèches/extrémités
    /// Pour chaque élément de décoration du lien...
    for (const auto& itm : m_items)
    {
        Coords head = p[0] + vec_dir * itm.m_position;

        /// Cas Bullet
        if (itm.m_type == ArrowItemType::Bullet)
            circlefill(page, head.x, head.y, itm.m_size/3, m_color);

        /// Cas pointe de flèche ou triangle
        else
        {
            Coords U = vec_dir.normalize()*itm.m_size;
            Coords V = U.rotate_90() * itm.m_proportion;

            Coords arrow_p = head - U;
            Coords ap1 = arrow_p - V;
            Coords ap2 = arrow_p + V;

            /// Pointe de flèche
            if (itm.m_type == ArrowItemType::Arrow)
            {
                thick_line(page, head.x, head.y, ap1.x, ap1.y, m_thickness, m_color);
                thick_line(page, head.x, head.y, ap2.x, ap2.y, m_thickness, m_color);
            }

            /// Triangle
            if (itm.m_type == ArrowItemType::Triangle)
                triangle(page, head.x, head.y, ap1.x, ap1.y, ap2.x, ap2.y, m_color);
        }
    }

}



}

