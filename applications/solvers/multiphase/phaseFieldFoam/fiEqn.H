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
    

    nvtxRangePushA("phaseFieldFoam_fiEqn_D");
    std::transform(
        std::execution::par,
        (*gradFiy).cbegin(),
        (*gradFiy).cend(),
        (*gradFix).cbegin(),
        (*D).begin(),
        [pi_=constant::mathematical::pi,
         N_=N.value(),
         alpha_=alpha.value(),
         c_=c.value(),
         tau_=tau.value()]
        (scalar gradFiyc, scalar gradFixc)
        {       
            /**
            @var
                @brief &psi;
            */
            const scalar psic = pi_/8. + Foam::atan2(gradFiyc, gradFixc);
            /**
            @var
                @brief &Phi;
            */
            const scalar Phic = Foam::tan(N_ * psic/2);
            /**
            @var
                @brief &beta;
            */
            const scalar betac = (1 - sqr(Phic))/(1 + sqr(Phic));
            
            /**
            @var
                @brief Da
            */
            const scalar Dac = sqr(alpha_) * (1 + c_*betac)/tau_;

            const scalar Ddiagc = Dac * (1 + c_*betac);
            const scalar Doffdiagc = Dac * c_ * (-N_ * 2 * Phic / (1 + sqr(Phic)));
            return Ddiagc*tensor(1,0,0,0,1,0,0,0,0) + Doffdiagc*tensor(0,1,0,-1,0,0,0,0,0);
        }
    );
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
