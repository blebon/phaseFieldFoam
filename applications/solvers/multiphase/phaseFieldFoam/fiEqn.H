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


    nvtxRangePushA("phaseFieldFoam_fiEqn_Phi");
    std::transform(
        std::execution::par,
        (gradFiy).cbegin(),
        (gradFiy).cend(),
        (gradFix).cbegin(),
        Phi.begin(),
        [=, _pi=constant::mathematical::pi, _N=N.value()]
        (scalar gradFiyc, scalar gradFixc)
        {       
            /**
            @var
                @brief &psi;
            */ 
            scalar _psi = _pi/8. + Foam::atan2(gradFiyc, gradFixc);
            return Foam::tan(_N * _psi/2);
        }
    );
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_fiEqn_beta");
    auto* Phi_p = &Phi;
    auto* beta_p = &beta;
    std::transform(
        std::execution::par,
        (*Phi_p).cbegin(),
        (*Phi_p).cend(),
        (*beta_p).begin(),
        [=](scalar Phic)
        {        
            return (1 - sqr(Phic))/(1 + sqr(Phic));
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
