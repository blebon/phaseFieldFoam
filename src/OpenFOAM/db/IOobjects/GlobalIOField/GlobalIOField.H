/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2015-2021 OpenFOAM Foundation
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
    Foam::GlobalIOField

Description
    IOField with global data (so optionally read from master)

SourceFiles
    GlobalIOField.C

\*---------------------------------------------------------------------------*/

#ifndef GlobalIOField_H
#define GlobalIOField_H

#include "regIOobject.H"
#include "Field.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class GlobalIOField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class GlobalIOField
:
    public regIOobject,
    public Field<Type>
{

public:

    TypeName("Field");


    // Constructors

        //- Construct from IOobject
        GlobalIOField(const IOobject&);

        //- Construct from IOobject and size (does not set values)
        GlobalIOField(const IOobject&, const label size);

        //- Construct from components
        GlobalIOField(const IOobject&, const Field<Type>&);

        //- Move construct by transferring the Field contents
        GlobalIOField(const IOobject&, Field<Type>&&);

        //- Copy constructor
        GlobalIOField(const GlobalIOField<Type>&);

        //- Move constructor
        GlobalIOField(GlobalIOField<Type>&&);


    //- Destructor
    virtual ~GlobalIOField();


    // Member Functions

        //- Is object global
        virtual bool global() const
        {
            return true;
        }

        //- ReadData function required for regIOobject read operation
        virtual bool readData(Istream&);

        //- WriteData function required for regIOobject write operation
        bool writeData(Ostream&) const;


    // Member Operators

        void operator=(const GlobalIOField<Type>&);
        void operator=(GlobalIOField<Type>&&);

        void operator=(const Field<Type>&);
        void operator=(Field<Type>&&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "GlobalIOField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
