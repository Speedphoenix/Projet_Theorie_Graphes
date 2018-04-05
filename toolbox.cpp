#include "toolbox.h"

ToolboxInterface::ToolboxInterface(int w, int h)
{
    //m_top_box fera partie de tool box de graphe
    m_top_box.set_dim(w, h);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
}


