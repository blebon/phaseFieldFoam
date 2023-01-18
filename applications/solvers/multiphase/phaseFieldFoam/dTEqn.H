{   
    fvScalarMatrix dTEqn
    (
        fvm::ddt(dT) 
      - fvm::laplacian(DT, dT)
     ==
        fvc::ddt(fi)
    );

    dTEqn.relax();

    dTEqn.solve();
}
