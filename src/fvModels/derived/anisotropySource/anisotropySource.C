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

\*---------------------------------------------------------------------------*/

#include "anisotropySource.H"
#include "fvModels.H"
#include "fvMatrix.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{
    defineTypeNameAndDebug(anisotropySource, 0);
    addToRunTimeSelectionTable
    (
        fvModel,
        anisotropySource,
        dictionary
    );
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::fv::anisotropySource::readCoeffs()
{
    kappa1_ = coeffs().lookupOrDefault<scalar>("kappa1", 0.9);
    kappa2_ = coeffs().lookupOrDefault<scalar>("kappa2", 20.0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fv::anisotropySource::anisotropySource
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    fvModel(name, modelType, dict, mesh),
    kappa1_(NaN),
    kappa2_(NaN)
{
    readCoeffs();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fv::anisotropySource::~anisotropySource()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::wordList Foam::fv::anisotropySource::addSupFields() const
{
    return wordList();
}


void Foam::fv::anisotropySource::addSup
(
    fvMatrix<scalar>& eqn
) const
{
    //- Cell volumes
    const scalarField& V = mesh().V();
    //- Phase field variable &phi;
    const volScalarField& fi = mesh().lookupObject<volScalarField>("fi");
    //- Undercooling variable &Delta;T
    const volScalarField& dT = mesh().lookupObject<volScalarField>("dT");

    // No &phi;* as source is implicit
    eqn.diag() += V*(1.0 - fi) *
        (fi - 0.5 - kappa1_/constant::mathematical::pi
        * Foam::atan(kappa2_ * dT));
}


void Foam::fv::anisotropySource::addSup
(
    fvMatrix<scalar>& eqn,
    const word& fieldName
) const
{
    //- Cell volumes
    const scalarField& V = mesh().V();
    //- Phase field variable &phi;
    const volScalarField& fi = mesh().lookupObject<volScalarField>(fieldName);
    //- Undercooling variable &Delta;T
    const volScalarField& dT = mesh().lookupObject<volScalarField>("dT");

    // No &phi;* as source is implicit
    eqn.diag() += V*(1.0 - fi) *
        (fi - 0.5 - kappa1_/constant::mathematical::pi
        * Foam::atan(kappa2_ * dT));
}


bool Foam::fv::anisotropySource::movePoints()
{
    return true;
}


void Foam::fv::anisotropySource::topoChange(const polyTopoChangeMap& map)
{
}


void Foam::fv::anisotropySource::mapMesh(const polyMeshMap& map)
{
}


void Foam::fv::anisotropySource::distribute(const polyDistributionMap& map)
{
}


bool Foam::fv::anisotropySource::read(const dictionary& dict)
{
    if (fvModel::read(dict))
    {
        readCoeffs();
        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
