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


    Info<< "Reading diffusivity DT\n" << endl;

/**
@var
    @brief Heat conductivity
*/
    dimensionedScalar DT
    (
        "DT",
        dimArea/dimTime,
        physicalProperties.lookup("DT")
    );


    Info<< "Creating scalar field beta\n" << endl;

/**
@var
    @brief &beta;
*/
volScalarField beta
    (
        IOobject
        (
            "beta",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar(
            "_beta",
            dimless,
            0.
        )
    );

    Info<< "Creating tensor field D\n" << endl;

/**
@var
    @brief Anisotropy diffusion tensor
*/
    volTensorField D
    (
        IOobject
        (
            "D",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedTensor
        (
            "D",
            dimArea/dimTime,
            tensor::zero
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

    Info<< "Reading alpha\n" << endl;

/**
@var
    @brief &alpha;
*/
    dimensionedScalar alpha
    (
        "alpha",
        dimLength,
        physicalProperties.lookup("alpha")
    );

    Info<< "Reading c\n" << endl;

/**
@var
    @brief c
*/
    dimensionedScalar c
    (
        "c",
        dimless,
        physicalProperties.lookup("c")
    );

    Info<< "Reading symmetry N\n" << endl;

/**
@var
    @brief Crystal symmetry
*/
    dimensionedScalar N
    (
        "N",
        dimless,
        physicalProperties.lookup("N")
    );

    Info<< "Reading symmetry kappa1\n" << endl;
/**
@var
    @brief &kappa;_1
*/
dimensionedScalar kappa1
    (
        "kappa1",
        dimless,
        physicalProperties.lookup("kappa1")
    );

    Info<< "Reading symmetry kappa2\n" << endl;

/**
@var
    @brief &kappa;_2
*/
    dimensionedScalar kappa2
    (
        "kappa2",
        dimless,
        physicalProperties.lookup("kappa2")
    );


#include "createFvModels.H"
