/**
@file
    @brief The undercooling equation
*/
{   
/**
@var
    @brief The undercooling equation
*/
    nvtxRangePushA("phaseFieldFoam_dTEqn_assemble");
    fvScalarMatrix dTEqn
    (
        fvm::ddt(dT) 
      - fvm::laplacian(DT, dT)
     ==
        fvc::ddt(fi)
    );
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_dTEqn_relax");
    dTEqn.relax(); //!< Apply relaxation to the undercooling equation
    nvtxRangePop();

    nvtxRangePushA("phaseFieldFoam_dTEqn_solve");
    dTEqn.solve(); //!< Solve the undercooling equation
    nvtxRangePop();
}
