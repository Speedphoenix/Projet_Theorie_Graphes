#ifndef COORDS_H
#define COORDS_H

#include <cmath>
#include <iostream>

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert
    Merci de citer vos sources
*/

/**
 *  Modifications apportées:
 *
 *  Ajout de l'operator/ pour un double
 *  Ajout des operator<< et >>
 *
 *  À faire:
 *  une meilleure fonction de rotation
*/

struct Coords
{
        double x;
        double y;

        Coords()
            : x(0), y(0) {}

        Coords(double x_, double y_)
            : x(x_), y(y_) {}

        //utiliser ccette fonction au lieu de norm() pour toutes les comparaisons
        double norm2() { return x*x + y*y; }

        //ne pas utiliser en temps normal, mais utile pour donner des distances...
        double norm() { return sqrt(norm2()); }


        Coords operator+ (const Coords &rhs)
        {
            return Coords( x+rhs.x, y+rhs.y  );
        }

        Coords operator- (const Coords &rhs)
        {
            return Coords( x-rhs.x, y-rhs.y  );
        }

        Coords operator- ()
        {
            return Coords( -x, -y  );
        }

        Coords operator* (const double &rhs)
        {
            return Coords( x*rhs, y*rhs  );
        }

        Coords operator* (const Coords &rhs)
        {
            return Coords( x*rhs.x, y*rhs.y  );
        }

        Coords operator/ (const double &rhs)
        {
            return Coords( x/rhs, y/rhs );
        }

        Coords operator/ (const Coords &rhs)
        {
            return Coords( x/rhs.x, y/rhs.y  );
        }

        Coords normalize()
        {
            double nrm = norm();
            if ( !nrm )
                return Coords(0,0);

            //pas mieux de faire return Coords(1, 1); ?
            return *this * (1./nrm);
        }

        //pas une rotation...
        Coords rotate_90() { return Coords(y, -x);  }

};


std::ostream& operator<<(std::ostream& myStream, Coords& what);

std::istream& operator>>(std::istream& myStream, Coords& what);


struct Frame
{
        Coords pos;
        Coords dim;

        Frame()
            : pos(), dim() {}

        Frame(Coords pos_, Coords dim_)
            : pos(pos_), dim(dim_) {}

        Frame(double x_, double y_, double w_, double h_)
            : pos(x_, y_), dim(w_, h_) {}

        /// Renvoie les coordonnées (dans le repère de la position de Frame) de l'intersection
        /// entre la demi-droite centre Frame -> vecteur directeur vec et le cadre de la Frame
        Coords intersect(Coords vec);

};


#endif // COORDS_H
