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

    Info<< "Reading kinematic viscosity nu\n" << endl;

/**
@var
    @brief Kinematic viscosity
*/

    dimensionedScalar nu
    (
        "nu",
        dimViscosity,
        physicalProperties.lookup("nu")
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

    Info<< "Reading field p\n" << endl;

/**
@var
    @brief Pressure volume scalar field
*/
    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field U\n" << endl;
/**
@var
    @brief Velocity volume vector field
*/
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    #include "createPhi.H"

    label pRefCell = 0;
    scalar pRefValue = 0.0;
    setRefCell(p, mesh.solution().dict().subDict("PISO"), pRefCell, pRefValue);
    mesh.schemes().setFluxRequired(p.name());

    #include "createFvModels.H"