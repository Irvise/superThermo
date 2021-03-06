/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "PengRobinsonGasPConst.H"
#include "mathematicalConstants.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Specie>
inline Foam::PengRobinsonGasPConst<Specie>::PengRobinsonGasPConst
(
    const Specie& sp,
    const scalar& Tc,
    const scalar& Vc,
    const scalar& Zc,
    const scalar& Pc,
    const scalar& omega,
    const scalar& PConst
)
:
    Specie(sp),
    Tc_(Tc),
    Vc_(Vc),
    Zc_(Zc),
    Pc_(Pc),
    omega_(omega),
    PConst_(PConst)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
inline Foam::PengRobinsonGasPConst<Specie>::PengRobinsonGasPConst
(
    const word& name,
    const PengRobinsonGasPConst& pg
)
:
    Specie(name, pg),
    Tc_(pg.Tc_),
    Pc_(pg.Pc_),
    Vc_(pg.Vc_),
    Zc_(pg.Zc_),
    omega_(pg.omega_),
    PConst_(pg.PConst_)
{}


template<class Specie>
inline Foam::autoPtr<Foam::PengRobinsonGasPConst <Specie>>
Foam::PengRobinsonGasPConst<Specie>::clone() const
{
    return autoPtr<PengRobinsonGasPConst<Specie>>
    (
        new PengRobinsonGasPConst<Specie>(*this)
    );
}


template<class Specie>
inline Foam::autoPtr<Foam::PengRobinsonGasPConst<Specie>>
Foam::PengRobinsonGasPConst<Specie>::New
(
    Istream& is
)
{
    return autoPtr<PengRobinsonGasPConst<Specie>>(new PengRobinsonGasPConst<Specie>(is));
}


template<class Specie>
inline Foam::autoPtr<Foam::PengRobinsonGasPConst<Specie>>
Foam::PengRobinsonGasPConst<Specie>::New
(
    const dictionary& dict
)
{
    return autoPtr<PengRobinsonGasPConst<Specie>>
    (
        new PengRobinsonGasPConst<Specie>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
inline Foam::scalar Foam::PengRobinsonGasPConst<Specie>::rho
(
    scalar p,
    scalar T
) const
{
    scalar Z = this->Z(PConst_, T);
    return PConst_/(Z*this->R()*T);
}


template<class Specie>
inline Foam::scalar Foam::PengRobinsonGasPConst<Specie>::h(scalar p, scalar T) const
{
    scalar Pr = PConst_/Pc_;
    scalar Tr = T/Tc_;
    scalar B = 0.07780*Pr/Tr;
    scalar kappa = 0.37464 + 1.54226*omega_ - 0.26992*sqr(omega_);
    scalar alpha = sqr(1 + kappa*(1 - sqrt(Tr)));

    scalar Z = this->Z(PConst_, T);

    return
        RR*Tc_
       *(
           Tr*(Z - 1)
         - 2.078*(1 + kappa)*sqrt(alpha)
          *log((Z + 2.414*B)/(Z - 0.414*B))
        );
}


template<class Specie>
inline Foam::scalar Foam::PengRobinsonGasPConst<Specie>::cp(scalar p, scalar T) const
{
    scalar Tr = T/Tc_;
    scalar a = 0.45724*sqr(RR*Tc_)/Pc_;
    scalar b = 0.07780*RR*Tc_/Pc_;
    scalar kappa = 0.37464 + 1.54226*omega_ - 0.26992*sqr(omega_);
    scalar alpha = sqr(1 + kappa*(1 - sqrt(Tr)));

    scalar A = a*alpha*PConst_/sqr(RR*T);
    scalar B = b*PConst_/(RR*T);

    scalar Z = this->Z(PConst_, T);

    scalar ap = kappa*a*(kappa/Tc_ - (1 + kappa)/sqrt(T*Tc_));
    scalar app = kappa*a*(1 + kappa)/(2*sqrt(pow3(T)*Tc_));

    scalar M = (sqr(Z) + 2*B*Z - sqr(B))/(Z - B);
    scalar N = ap*B/(b*RR);

    const scalar root2 = sqrt(2.0);

    return
        app*(T/(2*root2*b))*log((Z + (root2 + 1)*B)/(Z - (root2 - 1)*B))
      + RR*sqr(M - N)/(sqr(M) - 2*A*(Z + B))
      - RR;
}


template<class Specie>
inline Foam::scalar Foam::PengRobinsonGasPConst<Specie>::s
(
    scalar p,
    scalar T
) const
{
    scalar Pr = PConst_/Pc_;
    scalar Tr = T/Tc_;
    scalar B = 0.07780*Pr/Tr;
    scalar kappa = 0.37464 + 1.54226*omega_ - 0.26992*sqr(omega_);

    scalar Z = this->Z(PConst_, T);

    return
      - RR*log(PConst_/Pstd)
      + RR
       *(
           log(Z - B)
         - 2.078*kappa*((1 + kappa)/sqrt(Tr) - kappa)
          *log((Z + 2.414*B)/(Z - 0.414*B))
        );
}


template<class Specie>
inline Foam::scalar Foam::PengRobinsonGasPConst<Specie>::psi
(
    scalar p,
    scalar T
) const
{
    //scalar Z = this->Z(p, T);

    //return 1.0/(Z*this->R()*T);
    return 0;
}


template<class Specie>
inline Foam::scalar Foam::PengRobinsonGasPConst<Specie>::Z
(
    scalar p,
    scalar T
) const
{
    scalar Tr = T/Tc_;
    scalar a = 0.45724*sqr(RR*Tc_)/Pc_;
    scalar b = 0.07780*RR*Tc_/Pc_;
    scalar kappa = 0.37464 + 1.54226*omega_ - 0.26992*sqr(omega_);
    scalar alpha = sqr(1 + kappa*(1 - sqrt(Tr)));

    scalar A = a*alpha*PConst_/sqr(RR*T);
    scalar B = b*PConst_/(RR*T);

    scalar a2 = B - 1;
    scalar a1 = A - 2*B - 3*sqr(B);
    scalar a0 = -A*B + sqr(B) + pow3(B);

    scalar Q = (3*a1 - a2*a2)/9.0;
    scalar Rl = (9*a2*a1 - 27*a0 - 2*a2*a2*a2)/54.0;

    scalar Q3 = Q*Q*Q;
    scalar D = Q3 + Rl*Rl;

    scalar root = -1;

    if (D <= 0)
    {
        scalar th = ::acos(Rl/sqrt(-Q3));
        scalar qm = 2*sqrt(-Q);
        scalar r1 = qm*cos(th/3.0) - a2/3.0;
        scalar r2 = qm*cos((th + 2*constant::mathematical::pi)/3.0) - a2/3.0;
        scalar r3 = qm*cos((th + 4*constant::mathematical::pi)/3.0) - a2/3.0;

        root = max(r1, max(r2, r3));
    }
    else
    {
        // One root is real
        scalar D05 = sqrt(D);
        scalar S = pow(Rl + D05, 1.0/3.0);
        scalar Tl = 0;
        if (D05 > Rl)
        {
            Tl = -pow(mag(Rl - D05), 1.0/3.0);
        }
        else
        {
            Tl = pow(Rl - D05, 1.0/3.0);
        }

        root = S + Tl - a2/3.0;
    }

    return root;
}


template<class Specie>
inline Foam::scalar Foam::PengRobinsonGasPConst<Specie>::cpMcv
(
    scalar p,
    scalar T
) const
{
    scalar Tr = T/Tc_;
    scalar a = 0.45724*sqr(RR*Tc_)/Pc_;
    scalar b = 0.07780*RR*Tc_/Pc_;
    scalar kappa = 0.37464 + 1.54226*omega_ - 0.26992*sqr(omega_);
    scalar alpha = sqr(1 + kappa*(1 - sqrt(Tr)));

    scalar A = alpha*a*PConst_/sqr(RR*T);
    scalar B = b*PConst_/(RR*T);

    scalar Z = this->Z(PConst_, T);

    scalar ap = kappa*a*(kappa/Tc_ - (1 + kappa)/sqrt(T*Tc_));
    scalar M = (sqr(Z) + 2*B*Z - sqr(B))/(Z - B);
    scalar N = ap*B/(b*RR);

    return RR*sqr(M - N)/(sqr(M) - 2*A*(Z + B));
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline void Foam::PengRobinsonGasPConst<Specie>::operator+=
(
    const PengRobinsonGasPConst<Specie>& pg
)
{
    scalar molr1 = this->nMoles();
    Specie::operator+=(pg);

    molr1 /= this->nMoles();
    scalar molr2 = pg.nMoles()/this->nMoles();

    Tc_ = molr1*Tc_ + molr2*pg.Tc_;
    Vc_ = molr1*Vc_ + molr2*pg.Vc_;
    Zc_ = molr1*Zc_ + molr2*pg.Zc_;
    Pc_ = RR*Zc_*Tc_/Vc_;
    omega_ = molr1*omega_ + molr2*pg.omega_;
    PConst_ = PConst_;
}


template<class Specie>
inline void Foam::PengRobinsonGasPConst<Specie>::operator-=
(
    const PengRobinsonGasPConst<Specie>& pg
)
{
    scalar molr1 = this->nMoles();

    Specie::operator-=(pg);

    molr1 /= this->nMoles();
    scalar molr2 = pg.nMoles()/this->nMoles();

    Tc_ = molr1*Tc_ - molr2*pg.Tc_;
    Vc_ = molr1*Vc_ - molr2*pg.Vc_;
    Zc_ = molr1*Zc_ - molr2*pg.Zc_;
    Pc_ = RR*Zc_*Tc_/Vc_;
    omega_ = molr1*omega_ - molr2*pg.omega_;
    PConst_ = PConst_;
}


template<class Specie>
inline void Foam::PengRobinsonGasPConst<Specie>::operator*=(const scalar s)
{
     Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


template<class Specie>
Foam::PengRobinsonGasPConst<Specie> Foam::operator+
(
    const PengRobinsonGasPConst<Specie>& pg1,
    const PengRobinsonGasPConst<Specie>& pg2
)
{
    scalar nMoles = pg1.nMoles() + pg2.nMoles();
    scalar molr1 = pg1.nMoles()/nMoles;
    scalar molr2 = pg2.nMoles()/nMoles;

    const scalar Tc = molr1*pg1.Tc_ + molr2*pg2.Tc_;
    const scalar Vc = molr1*pg1.Vc_ + molr2*pg2.Vc_;
    const scalar Zc = molr1*pg1.Zc_ + molr2*pg2.Zc_;

    return PengRobinsonGasPConst<Specie>
    (
        static_cast<const Specie&>(pg1)
      + static_cast<const Specie&>(pg2),
        Tc,
        Vc,
        Zc,
        RR*Zc*Tc/Vc,
        molr1*pg1.omega_ + molr2*pg2.omega_,
        pg1.PConst_
    );
}


template<class Specie>
Foam::PengRobinsonGasPConst<Specie> Foam::operator-
(
    const PengRobinsonGasPConst<Specie>& pg1,
    const PengRobinsonGasPConst<Specie>& pg2
)
{
    scalar nMoles = pg1.nMoles() + pg2.nMoles();
    scalar molr1 = pg1.nMoles()/nMoles;
    scalar molr2 = pg2.nMoles()/nMoles;

    const scalar Tc = molr1*pg1.Tc_ + molr2*pg2.Tc_;
    const scalar Vc = molr1*pg1.Vc_ + molr2*pg2.Vc_;
    const scalar Zc = molr1*pg1.Zc_ + molr2*pg2.Zc_;

    return PengRobinsonGasPConst<Specie>
    (
        static_cast<const Specie&>(pg1)
      - static_cast<const Specie&>(pg2),
        Tc,
        Vc,
        Zc,
        RR*Zc*Tc/Vc,
        molr1*pg1.omega_ - molr2*pg2.omega_,
        pg1.PConst_
    );
}


template<class Specie>
Foam::PengRobinsonGasPConst<Specie> Foam::operator*
(
    const scalar s,
    const PengRobinsonGasPConst<Specie>& pg
)
{
    return PengRobinsonGasPConst<Specie>
    (
        s*static_cast<const Specie&>(pg),
        pg.Tc_,
        pg.Vc_,
        pg.Zc_,
        pg.Pc_,
        pg.omega_,
        pg.PConst_
    );
}


template<class Specie>
Foam::PengRobinsonGasPConst<Specie> Foam::operator==
(
    const PengRobinsonGasPConst<Specie>& pg1,
    const PengRobinsonGasPConst<Specie>& pg2
)
{
    return pg2 - pg1;
}


// ************************************************************************* //
