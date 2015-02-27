/*
    Collisions - a real-time simulation program of colliding particles.
    Copyright (C) 2011 - 2015  G. Endignoux

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

#include "solveur.hpp"

#include <cmath>
#include <ctime>

// Générateur de nombres aléatoires.
std::mt19937& Solveur::generateur = Solveur::makeGenerateur();

std::mt19937& Solveur::makeGenerateur()
{
    static std::mt19937 generateur;
    generateur.seed(std::time(0));
    return generateur;
}


// Degré 2.
void Solveur::quadratique(const std::complex<double>& a, const std::complex<double>& b, const std::complex<double>& c, std::complex<double>& z0, std::complex<double>& z1)
{
    std::complex<double> delta = b * b - 4.0 * a * c;

    std::complex<double> rac = std::sqrt(delta);
    z0 = (-b - rac) / (2.0 * a);
    z1 = (-b + rac) / (2.0 * a);
}

double Solveur::fstQuadratique(double a, double b, double c)
{
    a *= 2.0;
    double delta = b * b - 2.0 * a * c;

    if (delta >= 0.0)
        return a > 0.0 ? -(b + std::sqrt(delta)) / a : (-b + std::sqrt(delta)) / a;
    return -1;
}

double Solveur::sndQuadratique(double a, double b, double c)
{
    a *= 2.0;
    double delta = b * b - 2.0 * a * c;

    if (delta >= 0.0)
        return a > 0.0 ? (-b + std::sqrt(delta)) / a : -(b + std::sqrt(delta)) / a;
    return -1;
}

// Degré 3.
double Solveur::racinecubique(double x)
{
    return x < 0.0 ? -std::exp(std::log(-x) / 3.0) : std::exp(std::log(x) / 3.0);
}

double Solveur::racinecubiquecomplex(std::complex<double> z)
{
    double module = std::sqrt(std::norm(z));
    z /= module;

    return std::exp(std::log(module) / 3.0) * std::cos(std::arg(z) / 3.0);
}

void Solveur::cubique(double a, double b, double c, double d, std::complex<double>& z0, std::complex<double>& z1, std::complex<double>& z2)
{
    d /= a;
    c /= a;
    b /= a;

    double s = -b / 3.0;

    double p = - b * b / 3.0 + c;
    double q = b * (2.0 * b * b - 9.0 * c) / 27.0 + d;
    double delta = q * q + 4.0 * (p * p * p) / 27.0;

    if (delta > 0.0)
    {
        double rac = std::sqrt(delta);
        z0 = s + racinecubique((-q + rac) / 2.0) + racinecubique((-q - rac) / 2.0);
        Solveur::quadratique(1.0, b + z0.real(), c + z0.real() * (b + z0.real()), z1, z2);
    }
    else if (delta < 0.0)
    {
        double tmp = racinecubiquecomplex(std::complex<double>(-q, std::sqrt(-delta)) / 2.0);
        z0 = s + 2.0 * tmp;
        Solveur::quadratique(1.0, b + z0.real(), c + z0.real() * (b + z0.real()), z1, z2);
    }
    else
    {
        z0 = s + 3.0 * q / p;
        z2 = z1 = s - 3.0 * q / (2.0 * p);
    }
}

double Solveur::realCubique(double a, double b, double c, double d)
{
    d /= a;
    c /= a;
    b /= a;

    double s = -b / 3.0;

    double p = - b * b / 3.0 + c;
    double q = b * (2.0 * b * b - 9.0 * c) / 27.0 + d;
    double delta = q * q + 4.0 * (p * p * p) / 27.0;

    if (delta > 0.0)
    {
        double rac = std::sqrt(delta);
        s += racinecubique((-q + rac) / 2.0) + racinecubique((-q - rac) / 2.0);
    }
    else if (delta < 0.0)
    {
        double tmp = racinecubiquecomplex(std::complex<double>(-q, std::sqrt(-delta)) / 2.0);
        s += 2.0 * tmp;
    }
    else
        s += 3.0 * q / p;

    return s;
}

// Degré 4.
void Solveur::quartique(double a, double b, double c, double d, double e, std::complex<double>& z0, std::complex<double>& z1, std::complex<double>& z2, std::complex<double>& z3)
{
    e /= a;
    d /= a;
    c /= a;
    b /= a;

    double s = -b / 4.0;
    double p = -3.0 * b * b / 8.0 + c;
    double q = b * b * b / 8.0 - b * c / 2.0 + d;
    double r = -3.0 * b * b * b * b / 256.0 + c * b * b / 16.0 - b * d / 4.0 + e;

    double y1 = Solveur::realCubique(8.0, -4.0 * p, -8.0 * r, 4.0 * r * p - (q * q));

    std::complex<double> rac1 = std::sqrt(std::complex<double>(2.0 * y1 - p));
    std::complex<double> rac2p = std::sqrt(-2.0 * y1 - p + 2.0 * q / rac1);
    std::complex<double> rac2n = std::sqrt(-2.0 * y1 - p - 2.0 * q / rac1);

    z0 = s + (rac1 + rac2n) / 2.0;
    z1 = s + (rac1 - rac2n) / 2.0;
    z2 = s + (rac2p - rac1) / 2.0;
    z3 = s - (rac2p + rac1) / 2.0;
}

void Solveur::min(double& a, const std::complex<double>& b, double _a, double _b, double _c, double _d)
{
    if (b.imag() == 0.0 && b.real() >= 0.0 && (b.real() < a || a < 0.0)
            && (_a * std::pow(b.real(), 3) + _b * std::pow(b.real(), 2) + _c * b.real() + _d) <= 0.0)
        a = b.real();
}

double Solveur::fstQuartique(double a, double b, double c, double d, double e)
{
    std::complex<double> z0, z1, z2, z3;
    Solveur::quartique(a, b, c, d, e, z0, z1, z2, z3);

    double result = -1;
    Solveur::min(result, z0, 4 * a, 3 * b, 2 * c, d);
    Solveur::min(result, z1, 4 * a, 3 * b, 2 * c, d);
    Solveur::min(result, z2, 4 * a, 3 * b, 2 * c, d);
    Solveur::min(result, z3, 4 * a, 3 * b, 2 * c, d);

    return result;
}
