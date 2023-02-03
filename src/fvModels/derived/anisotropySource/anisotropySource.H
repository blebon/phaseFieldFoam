/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2021-2022 OpenFOAM Foundation
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

Class
    Foam::fv::anisotropySource

Description
    Model for applying the anistropy source.

Usage
    Example usage:
    \verbatim
    anisotropySource
    {
        type            anisotropySource;

        tau             0.0003;
        kappa1          0.9;
        kappa2          20;

    }
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef anisotropySource_H
#define anisotropySource_H

#include "fvModel.H"
#include "fvMesh.H"
#include "volFields.H"
#include "NamedEnum.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                Class anisotropySource Declaration
\*---------------------------------------------------------------------------*/

class anisotropySource
:
    public fvModel
{
    // Private data

        //- &tau;
        scalar tau_;

        //- Anisotropy coefficient
        scalar kappa1_;

        //- Anisotropy coefficient
        scalar kappa2_;


    // Private member functions

        //- Non-virtual read
        void readCoeffs();


public:

    //- Runtime type information
    TypeName("anisotropySource");


    // Constructors

        //- Construct from dictionary
        anisotropySource
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~anisotropySource();


    // Member Functions

        // Return coefficients

            const scalar& tau() const
            {
                return tau_;
            }
            
            const scalar& kappa1() const
            {
                return kappa1_;
            }

            const scalar& kappa2() const
            {
                return kappa2_;
            }


        // Checks

            //- Return the list of fields for which the fvModel adds source term
            //  to the transport equation
            virtual wordList addSupFields() const;


        // Sources

            //- Add implicit contribution to &phi; equation
            virtual void addSup
            (
                fvMatrix<scalar>& eqn,
                const word& fieldName
            ) const;


        // Mesh changes

            //- Update for mesh motion
            virtual bool movePoints();

            //- Update topology using the given map
            virtual void topoChange(const polyTopoChangeMap&);

            //- Update from another mesh using the given map
            virtual void mapMesh(const polyMeshMap&);

            //- Redistribute or update using the given distribution map
            virtual void distribute(const polyDistributionMap&);


        // IO

            //- Read dictionary
            virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
