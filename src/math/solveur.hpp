/*
    Collisions - a real-time simulation program of colliding particles.
    Copyright (C) 2011 - 2014  G. Endignoux

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.txt
*/

#ifndef SOLVEUR_HPP
#define SOLVEUR_HPP

#include <complex>
#include <random>

// Classe pour résoudre des équations polynomiales de degré inférieur ou égal à 4.
class Solveur
{
public:
    // Degré 2.
    static void quadratique(const std::complex<double>& a, const std::complex<double>& b, const std::complex<double>& c, std::complex<double>& z0, std::complex<double>& z1);
    static double fstQuadratique(double a, double b, double c);
    static double sndQuadratique(double a, double b, double c);
    // Degré 3.
    static void cubique(double a, double b, double c, double d, std::complex<double>& z0, std::complex<double>& z1, std::complex<double>& z2);
    static double realCubique(double a, double b, double c, double d);
    // Degré 4.
    static void quartique(double a, double b, double c, double d, double e, std::complex<double>& z0, std::complex<double>& z1, std::complex<double>& z2, std::complex<double>& z3);
    static double fstQuartique(double a, double b, double c, double d, double e);

    // Générateur de nombres aléatoires.
    static std::mt19937& generateur;

private:
    // Fonctions utiles.
    static double racinecubique(double x);
    static double racinecubiquecomplex(std::complex<double> z);
    static void min(double& a, const std::complex<double>& b, double _a, double _b, double _c, double _d);

    // Générateur de nombres aléatoires.
    static std::mt19937& makeGenerateur();
};

#endif // SOLVEUR_HPP
