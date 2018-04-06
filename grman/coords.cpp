#include "coords.h"

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert
    Merci de citer vos sources
*/



// Renvoie les coordonnées (dans le repère de la position de Frame) de l'intersection
// entre la demi-droite centre Frame -> vecteur directeur vec et le cadre de la Frame
Coords Frame::intersect(Coords vec)
{
    Coords mid = dim * 0.5;
    Coords rel = vec / mid;

    Coords res;
    if ( rel.x || rel.y )
    {
        if ( rel.x + rel.y > 0 )
        {
            if ( rel.x - rel.y > 0)
                res.x = 1, res.y = rel.y/rel.x;  // Secteur droit
            else
                res.x = rel.x/rel.y, res.y = 1;  // Secteur bas
        }
        else
        {
            if ( rel.x - rel.y > 0)
                res.x = -rel.x/rel.y, res.y = -1; // Secteur haut
            else
                res.x = -1, res.y = -rel.y/rel.x; // Secteur gauche
        }
    }

    return pos + mid + (res*mid);
}


std::ostream& operator<<(std::ostream& myStream, Coords& what)
{
    myStream << " " << what.x << " " << what.y << " ";
    return myStream;
}

std::istream& operator>>(std::istream& myStream, Coords& what)
{
    myStream >> what.x >> what.y;
    return myStream;
}

