    Info<< "Reading field fi\n" << endl;

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

    dimensionedScalar DT
    (
        "DT",
        dimArea/dimTime,
        physicalProperties.lookup("DT")
    );

    Info<< "Creating tensor field D\n" << endl;

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

    dimensionedScalar tau
    (
        "tau",
        dimTime,
        physicalProperties.lookup("tau")
    );

    Info<< "Reading alpha\n" << endl;

    dimensionedScalar alpha
    (
        "alpha",
        dimLength,
        physicalProperties.lookup("alpha")
    );

    Info<< "Reading c\n" << endl;

    dimensionedScalar c
    (
        "c",
        dimless,
        physicalProperties.lookup("c")
    );

    Info<< "Reading symmetry N\n" << endl;

    dimensionedScalar N
    (
        "N",
        dimless,
        physicalProperties.lookup("N")
    );

    Info<< "Reading symmetry kappa1\n" << endl;

    dimensionedScalar kappa1
    (
        "kappa1",
        dimless,
        physicalProperties.lookup("kappa1")
    );

    Info<< "Reading symmetry kappa2\n" << endl;

    dimensionedScalar kappa2
    (
        "kappa2",
        dimless,
        physicalProperties.lookup("kappa2")
    );

