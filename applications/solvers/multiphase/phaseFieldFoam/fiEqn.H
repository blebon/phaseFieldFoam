/**
@file
    @brief The phase field equation
*/
{
/**
@var
    @brief Phase field gradients
*/
    volVectorField gradFi(fvc::grad(fi));

/**
@var
    @brief x component of the phase field gradients
*/
    volScalarField gradFix
    (
        IOobject
        (
            "gradFix",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        gradFi.component(vector::X)
    );

/**
@var
    @brief y component of the phase field gradients
*/
    volScalarField gradFiy
    (
        IOobject
        (
            "gradFiy",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        gradFi.component(vector::Y)
    );

/**
@var
    @brief &psi;
*/
    volScalarField psi =
        constant::mathematical::pi/8.
        + Foam::atan2(gradFiy, gradFix);

/**
@var
    @brief &Phi;
*/
    volScalarField Phi = Foam::tan(N * psi/2);

/**
@var
    @brief &beta;
*/
    volScalarField beta = (1 - sqr(Phi))/(1 + sqr(Phi));

// /**
// @var
//     @brief Phase field anisotropy source (explicit)
// */
//     volScalarField fiSourceExplicit =
//         fi * (1.0 - fi) *
//         (fi - 0.5 - kappa1/constant::mathematical::pi
//         * Foam::atan(kappa2 * dT));

/**
@var
    @brief Phase field anisotropy source (implicit)
*/
    volScalarField fiSourceImplicit =
        (1.0 - fi) *
        (fi - 0.5 - kappa1/constant::mathematical::pi
        * Foam::atan(kappa2 * dT));

    volScalarField Da = sqr(alpha) * (1 + c*beta)/tau;
    volScalarField Ddiag = Da * (1 + c*beta);
    volScalarField Doffdiag = Da * c * (-N * 2 * Phi / (1 + sqr(Phi)));
    D = Ddiag*tensor(1,0,0,0,1,0,0,0,0) + Doffdiag*tensor(0,1,0,-1,0,0,0,0,0);
    
/**
@var
    @brief The phase field equation
*/
    fvScalarMatrix fiEqn
    (
        fvm::ddt(fi) 
      - fvm::laplacian(D, fi)
     ==
        // fiSourceExplicit
        fvm::Sp(fiSourceImplicit, fi)
        // fvModels.source(fi)
    );

    fiEqn.relax(); //!< Apply relaxation to the phase field equation

    fiEqn.solve(); //!< Solve the phase field equation
}
