/**
@file
    @brief The undercooling equation
*/
{   
/**
@var
    @brief The undercooling equation
*/
    fvScalarMatrix dTEqn
    (
        fvm::ddt(dT)
      + fvm::div(phi, dT)
      - fvm::laplacian(DT, dT)
     ==
        fvc::ddt(fi)
    );

    dTEqn.relax(); //!< Apply relaxation to the undercooling equation

    dTEqn.solve(); //!< Solve the undercooling equation
}
