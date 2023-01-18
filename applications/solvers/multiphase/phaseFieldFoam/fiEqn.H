{
    volVectorField gradFi(fvc::grad(fi));

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
    
    volScalarField psi =
        constant::mathematical::pi/8.
        + Foam::atan2(gradFiy, gradFix);

    volScalarField Phi = Foam::tan(N * psi/2);

    volScalarField beta = (1 - sqr(Phi))/(1 + sqr(Phi));
    
    volScalarField fiSource =
        fi * (1.0 - fi) *
        (fi - 0.5 - kappa1/constant::mathematical::pi
        * Foam::atan(kappa2 * dT))/tau;

    volScalarField Da = sqr(alpha) * (1 + c*beta)/tau;
    volScalarField Ddiag = Da * (1 + c*beta);
    volScalarField Doffdiag = Da * c * (-N * 2 * Phi / (1 + sqr(Phi)));

    D = Ddiag*tensor(1,0,0,0,1,0,0,0,0) + Doffdiag*tensor(0,1,0,-1,0,0,0,0,0);
    
    fvScalarMatrix fiEqn
    (
        fvm::ddt(fi) 
      - fvm::laplacian(D, fi)
     ==
        fiSource
    );

    fiEqn.relax();

    fiEqn.solve();
}
