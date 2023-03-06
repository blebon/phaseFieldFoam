/**
@file
    @brief The phase field equation
*/
{
/**
@var
    @brief Phase field gradients
*/
    nvtxRangePushA("phaseFieldFoam_fiEqn_gradFi");
    volVectorField gradFi(fvc::grad(fi));
    nvtxRangePop();

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
    nvtxRangePushA("phaseFieldFoam_fiEqn_psi");
    volScalarField _psi =
        constant::mathematical::pi/8.
        + Foam::atan2(gradFiy, gradFix);
    nvtxRangePop();

/**
@var
    @brief &Phi;
*/
    nvtxRangePushA("phaseFieldFoam_fiEqn_Phi");
    volScalarField Phi = Foam::tan(N * _psi/2);
    nvtxRangePop();

/**
@var
    @brief &beta;
*/
    nvtxRangePushA("phaseFieldFoam_fiEqn_beta");
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
    
    std::for_each_n(
        std::execution::par,
        (beta).begin(),
        (beta).size(),
        [&](label celli)
        {        
            const scalar Phic = Phi[celli];
            beta[celli] = (1 - sqr(Phic))/(1 + sqr(Phic));
        }
    );
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_fiEqn_Da");
    volScalarField Da = sqr(alpha) * (1 + c*beta)/tau;
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_fiEqn_D");
    volScalarField Ddiag = Da * (1 + c*beta);
    volScalarField Doffdiag = Da * c * (-N * 2 * Phi / (1 + sqr(Phi)));
    D = Ddiag*tensor(1,0,0,0,1,0,0,0,0) + Doffdiag*tensor(0,1,0,-1,0,0,0,0,0);
    nvtxRangePop();
    
/**
@var
    @brief The phase field equation
*/
    nvtxRangePushA("phaseFieldFoam_fiEqn_assemble");
    fvScalarMatrix fiEqn
    (
        fvm::ddt(fi) 
      - fvm::laplacian(D, fi)
     ==
        fvModels.source(fi)
    );
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_fiEqn_relax");
    fiEqn.relax(); //!< Apply relaxation to the phase field equation
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_fiEqn_solve");
    fiEqn.solve(); //!< Solve the phase field equation
    nvtxRangePop();
}
