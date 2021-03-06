/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2012 OpenFOAM Foundation
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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class equationOfState>
inline Foam::superH<equationOfState>::superH
(
    const equationOfState& st,
    const scalar cpL_in,
    const scalar cpG_in,
    const scalar TSpike_in,
    const scalar SpikeWidth1_in,
    const scalar cpSpike1_in,
    const scalar SpikeWidth2_in,
    const scalar cpSpike2_in,
    const scalar SpikeWidth3_in,
    const scalar cpSpike3_in,
    const scalar SpikeWidth4_in,
    const scalar cpSpike4_in
)
:
    equationOfState(st),
    cpL(cpL_in),
    cpG(cpG_in),    
    TSpike(TSpike_in),    
    SpikeWidth1(SpikeWidth1_in),
    cpSpike1(cpSpike1_in),
    SpikeWidth2(SpikeWidth2_in),
    cpSpike2(cpSpike2_in),
    SpikeWidth3(SpikeWidth3_in),
    cpSpike3(cpSpike3_in),
    SpikeWidth4(SpikeWidth4_in),
    cpSpike4(cpSpike4_in)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class equationOfState>
inline Foam::superH<equationOfState>::superH
(
    const word& name,
    const superH& ct
)
:
    equationOfState(name, ct),
    cpL(ct.cpL),
    cpG(ct.cpG),
    TSpike(ct.TSpike),
    SpikeWidth1(ct.SpikeWidth1),
    cpSpike1(ct.cpSpike1),
    SpikeWidth2(ct.SpikeWidth2),
    cpSpike2(ct.cpSpike2),
    SpikeWidth3(ct.SpikeWidth3),
    cpSpike3(ct.cpSpike3),
    SpikeWidth4(ct.SpikeWidth4),
    cpSpike4(ct.cpSpike4)
{}


template<class equationOfState>
inline Foam::autoPtr<Foam::superH<equationOfState> >
Foam::superH<equationOfState>::clone() const
{
    return autoPtr<superH<equationOfState> >
    (
        new superH<equationOfState>(*this)
    );
}


template<class equationOfState>
inline Foam::autoPtr<Foam::superH<equationOfState> >
Foam::superH<equationOfState>::New(Istream& is)
{
    return autoPtr<superH<equationOfState> >
    (
        new superH<equationOfState>(is)
    );
}


template<class equationOfState>
inline Foam::autoPtr<Foam::superH<equationOfState> >
Foam::superH<equationOfState>::New(const dictionary& dict)
{
    return autoPtr<superH<equationOfState> >
    (
        new superH<equationOfState>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
inline Foam::scalar Foam::superH<EquationOfState>::limit
(
    const scalar T
) const
{
    	return T;
	//return max(min(Tband_,T),-Tband_);
}


template<class equationOfState>
inline Foam::scalar Foam::superH<equationOfState>::cp
(
    const scalar p,
    const scalar T
) const
{
	const scalar cp = pos(T-TSpike)*cpG + neg(T-TSpike)*cpL
	                  + cpSpike1*exp(-1.0 * pow( (T-TSpike)/SpikeWidth1, 2) )
	                  + cpSpike2*exp(-1.0 * pow( (T-TSpike)/SpikeWidth2, 2) )
	                  + cpSpike3*exp(-1.0 * pow( (T-TSpike)/SpikeWidth3, 2) )
	                  + cpSpike4*exp(-1.0 * pow( (T-TSpike)/SpikeWidth4, 2) );
    return cp;
    
    
    //return cpG;
}


template<class equationOfState>
inline Foam::scalar Foam::superH<equationOfState>::ha
(
    const scalar p, const scalar T
) const
{
    return
    (
          pos(T - TSpike)*cpG*(T - TSpike)
        + neg(T - TSpike)*cpL*(T - TSpike)
        - (sqrt(3.141592)*cpSpike1*SpikeWidth1/2.0)*erf((TSpike-T)/SpikeWidth1)
        - (sqrt(3.141592)*cpSpike2*SpikeWidth2/2.0)*erf((TSpike-T)/SpikeWidth2)
        - (sqrt(3.141592)*cpSpike3*SpikeWidth3/2.0)*erf((TSpike-T)/SpikeWidth3)
        - (sqrt(3.141592)*cpSpike4*SpikeWidth4/2.0)*erf((TSpike-T)/SpikeWidth4)
    );
}


template<class equationOfState>
inline Foam::scalar Foam::superH<equationOfState>::hs
(
    const scalar p, const scalar T
) const
{
    return ha(p, T);
}


template<class equationOfState>
inline Foam::scalar Foam::superH<equationOfState>::hc() const
{
    return 0.0;
}


template<class equationOfState>
inline Foam::scalar Foam::superH<equationOfState>::s
(
    const scalar p, const scalar T
) const
{
    notImplemented
    (
        "scalar superH<equationOfState>::"
        "s(const scalar p, const scalar T) const"
    );
    return T;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

//Never tested...
template<class equationOfState>
inline void Foam::superH<equationOfState>::operator+=
(
    const superH<equationOfState>& ct
)
{
    scalar molr1 = this->nMoles();

    equationOfState::operator+=(ct);

    molr1 /= this->nMoles();
    scalar molr2 = ct.nMoles()/this->nMoles();
    Info<< "Mixture case... Not applicable for this thermo model 1" << endl;
//    cpL_    = molr1*cpL_ + molr2*ct.cpL_;
//    cpG_    = molr1*cpG_ + molr2*ct.cpG_;
//    cpVF_   = molr1*cpVF_ + molr2*ct.cpVF_;
//    TSpike_ = molr1*TSpike_ + molr2*ct.TSpike_;
//    SpikeWidth_ = molr1*SpikeWidth_ + molr2*ct.SpikeWidth_;
//    Tband_ = molr1*Tband_ + molr2*ct.Tband_;
}


template<class equationOfState>
inline void Foam::superH<equationOfState>::operator-=
(
    const superH<equationOfState>& ct
)
{
    scalar molr1 = this->nMoles();

    equationOfState::operator-=(ct);

    molr1 /= this->nMoles();
    scalar molr2 = ct.nMoles()/this->nMoles();
    Info<< "Mixture case... Not applicable for this thermo model 2" << endl;
//    cpL_    = molr1*cpL_ - molr2*ct.cpL_;
//    cpG_    = molr1*cpG_ - molr2*ct.cpG_;
//    cpVF_   = molr1*cpVF_ - molr2*ct.cpVF_;
//    TSpike_ = molr1*TSpike_ - molr2*ct.TSpike_;
//    SpikeWidth_ = molr1*SpikeWidth_ - molr2*ct.SpikeWidth_;
//    Tband_ = molr1*Tband_ - molr2*ct.Tband_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class equationOfState>
inline Foam::superH<equationOfState> Foam::operator+
(
    const superH<equationOfState>& ct1,
    const superH<equationOfState>& ct2
)
{
    equationOfState eofs
    (
        static_cast<const equationOfState&>(ct1)
      + static_cast<const equationOfState&>(ct2)
    );
    
    Info<< "Mixture case... Not applicable for this thermo model 3" << endl;

    return superH<equationOfState>
    (
        eofs,
        ct1.nMoles()/eofs.nMoles()*ct1.CpL,
        ct1.nMoles()/eofs.nMoles()*ct1.CpG,
        ct1.TSpike,
        ct1.SpikeWidth1,
        ct1.nMoles()/eofs.nMoles()*ct1.cpSpike1,
        ct1.SpikeWidth2,
        ct1.nMoles()/eofs.nMoles()*ct1.cpSpike2,
        ct1.SpikeWidth3,
        ct1.nMoles()/eofs.nMoles()*ct1.cpSpike3,
        ct1.SpikeWidth4,
        ct1.nMoles()/eofs.nMoles()*ct1.cpSpike4
        
//        ct1.nMoles()/eofs.nMoles()*ct1.CpL_
//      + ct2.nMoles()/eofs.nMoles()*ct2.CpL_,
//        ct1.nMoles()/eofs.nMoles()*ct1.CpG_
//      + ct2.nMoles()/eofs.nMoles()*ct2.CpG_,
//        ct1.nMoles()/eofs.nMoles()*ct1.CpVF_
//      + ct2.nMoles()/eofs.nMoles()*ct2.CpVF_,
//        ct1.nMoles()/eofs.nMoles()*ct1.TSpike_
//      + ct2.nMoles()/eofs.nMoles()*ct2.TSpike_,
//        ct1.nMoles()/eofs.nMoles()*ct1.SpikeWidth_
//      + ct2.nMoles()/eofs.nMoles()*ct2.SpikeWidth_,
//        ct1.nMoles()/eofs.nMoles()*ct1.Tband_
//      + ct2.nMoles()/eofs.nMoles()*ct2.Tband_
    );
}


template<class equationOfState>
inline Foam::superH<equationOfState> Foam::operator-
(
    const superH<equationOfState>& ct1,
    const superH<equationOfState>& ct2
)
{
    equationOfState eofs
    (
        static_cast<const equationOfState&>(ct1)
      - static_cast<const equationOfState&>(ct2)
    );
    
    Info<< "Mixture case... Not applicable for this thermo model 4" << endl;
    
    return superH<equationOfState>
    (
        eofs,
        ct1.nMoles()/eofs.nMoles()*ct1.CpL,
        ct1.nMoles()/eofs.nMoles()*ct1.CpG,
        ct1.TSpike,
        ct1.SpikeWidth1,
        ct1.nMoles()/eofs.nMoles()*ct1.cpSpike1,
        ct1.SpikeWidth2,
        ct1.nMoles()/eofs.nMoles()*ct1.cpSpike2,
        ct1.SpikeWidth3,
        ct1.nMoles()/eofs.nMoles()*ct1.cpSpike3,
        ct1.SpikeWidth4,
        ct1.nMoles()/eofs.nMoles()*ct1.cpSpike4
//        ct1.nMoles()/eofs.nMoles()*ct1.CpL_
//      - ct2.nMoles()/eofs.nMoles()*ct2.CpL_,
//        ct1.nMoles()/eofs.nMoles()*ct1.CpG_
//      - ct2.nMoles()/eofs.nMoles()*ct2.CpG_,
//        ct1.nMoles()/eofs.nMoles()*ct1.CpVF_
//      - ct2.nMoles()/eofs.nMoles()*ct2.CpVF_,
//        ct1.nMoles()/eofs.nMoles()*ct1.TSpike_
//      - ct2.nMoles()/eofs.nMoles()*ct2.TSpike_,
//        ct1.nMoles()/eofs.nMoles()*ct1.SpikeWidth_
//      - ct2.nMoles()/eofs.nMoles()*ct2.SpikeWidth_,
//        ct1.nMoles()/eofs.nMoles()*ct1.Tband_
//      - ct2.nMoles()/eofs.nMoles()*ct2.Tband_
    );
}


template<class equationOfState>
inline Foam::superH<equationOfState> Foam::operator*
(
    const scalar s,
    const superH<equationOfState>& ct
)
{
    return superH<equationOfState>
    (
        s*static_cast<const equationOfState&>(ct),
        ct.cpL,
        ct.cpG,
        ct.TSpike,
        ct.SpikeWidth1,
        ct.cpSpike1,
        ct.SpikeWidth2,
        ct.cpSpike2,
        ct.SpikeWidth3,
        ct.cpSpike3,
        ct.SpikeWidth4,
        ct.cpSpike4
//        ct.cpG_,
//        ct.cpVF_,
//        ct.TSpike_,
//        ct.SpikeWidth_,
//        ct.Tband_
    );
}


template<class equationOfState>
inline Foam::superH<equationOfState> Foam::operator==
(
    const superH<equationOfState>& ct1,
    const superH<equationOfState>& ct2
)
{
    return ct2 - ct1;
}


// ************************************************************************* //
