/**
@file
    @brief Variables and fields initialization
*/

    Info<< "Reading field fi\n" << endl;

/**
@var
    @brief The phase field volume scalar field
*/
    volScalarField fi
    (
        IOobject
        (
            "fi",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field dT\n" << endl;

/**
@var
    @brief The undercooling volume scalar field
*/
    volScalarField dT
    (
        IOobject
        (
            "dT",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading physicalProperties\n" << endl;

/**
@var
    @brief The physical properties dictionary file
*/
    IOdictionary physicalProperties
    (
        IOobject
        (
            "physicalProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    );

    Info<< "Reading tau\n" << endl;

/**
@var
    @brief &tau;
*/
    dimensionedScalar tau
    (
        "tau",
        dimTime,
        physicalProperties.lookup("tau")
    );

#include "createFvModels.H"