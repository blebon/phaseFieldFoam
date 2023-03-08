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
    *gradFi = fvc::grad(fi);
    nvtxRangePop();

/**
@var
    @brief x component of the phase field gradients
*/
    volScalarField* gradFix;
    gradFix = new volScalarField
    (
        IOobject
        (
            "gradFix",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        (*gradFi).component(vector::X)
    );

/**
@var
    @brief y component of the phase field gradients
*/
    volScalarField* gradFiy;
    gradFiy = new volScalarField
    (
        IOobject
        (
            "gradFiy",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        (*gradFi).component(vector::Y)
    );
    

    nvtxRangePushA("phaseFieldFoam_fiEqn_Phi");
    std::transform(
        std::execution::par,
        (*gradFiy).cbegin(),
        (*gradFiy).cend(),
        (*gradFix).cbegin(),
        (*Phi).begin(),
        [pi_=constant::mathematical::pi, N_=N.value()]
        (scalar gradFiyc, scalar gradFixc)
        {       
            /**
            @var
                @brief &psi;
            */ 
            scalar psi_ = pi_/8. + Foam::atan2(gradFiyc, gradFixc);
            return Foam::tan(N_ * psi_/2);
        }
    );
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_fiEqn_beta");
    std::transform(
        std::execution::par,
        (*Phi).cbegin(),
        (*Phi).cend(),
        (*beta).begin(),
        [](scalar Phic)
        {        
            return (1 - sqr(Phic))/(1 + sqr(Phic));
        }
    );
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_fiEqn_Da");
    std::transform(
        std::execution::par,
        (*beta).cbegin(),
        (*beta).cend(),
        (*Da).begin(),
        [alpha_=alpha.value(), c_=c.value(), tau_=tau.value()]
        (scalar betac)
        {        
            return sqr(alpha_) * (1 + c_*betac)/tau_;
        }
    );
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_fiEqn_D");
    volScalarField Ddiag = *Da * (1 + c*(*beta));
    volScalarField Doffdiag = *Da * c * (-N * 2 * (*Phi) / (1 + sqr(*Phi)));
    *D = Ddiag*tensor(1,0,0,0,1,0,0,0,0) + Doffdiag*tensor(0,1,0,-1,0,0,0,0,0);
    nvtxRangePop();

    delete gradFix;
    delete gradFiy;
    
/**
@var
    @brief The phase field equation
*/
    nvtxRangePushA("phaseFieldFoam_fiEqn_assemble");
    fvScalarMatrix fiEqn
    (
        fvm::ddt(fi) 
      - fvm::laplacian(*D, fi)
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
