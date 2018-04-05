#ifndef GALIBERT_H
#define GALIBERT_H
// inspiré du code http://iteme.free.fr/tutorials/Allegro.pdf

// classe Galibert pour la gestion du temps

class Galibert
{
    public:
        Galibert();
        virtual ~Galibert();

//         int Getm_game_time() { return m_game_time; }
//        void Setm_game_time( int val) { m_game_time = val; }

        virtual int Getm_sec_counter() { return m_sec_counter; }
        virtual void Setm_sec_counter( int val) { m_sec_counter = val; }

        virtual int Getm_milisec_counter() { return m_milisec_counter; }
        virtual void Setm_milisec_counter( int val) { m_milisec_counter = val; }

//        void GetSec(void);
//        void GetMiliSec(void);
//        int GetTime(void);

    private:

    //int m_game_time;
    int m_sec_counter;
    int m_milisec_counter;
};



#endif // GALIBERT_H
