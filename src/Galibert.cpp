#include "Galibert.h"
#include <time.h>
#include <stdlib.h>
#include <allegro.h>

Galibert::Galibert()
{
    //ctor
}

Galibert::~Galibert()
{
    //dtor
}

//void Galibert::GetSec(void)
//{
//    m_game_time++;
//}
//
//void Galibert::GetMiliSec(void)
//{
//    m_milisec_counter++;
//}

void GetTime()
{
    install_timer();
}

int GetMilisec(Galibert* first, Galibert* second)
{
    int sec, mili_sec;

    sec = second->Getm_sec_counter() - first->Getm_sec_counter();

    mili_sec = (int)(second->Getm_milisec_counter()/1000) - (int)(first->Getm_milisec_counter()/1000);

    return (1000*sec + (int)(mili_sec/1000));
}


