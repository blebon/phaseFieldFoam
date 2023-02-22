Theory
============

The transport of two properties are solved with two coupled partial different equations. The phase field variable :math:`\phi` is 0 in the liquid phase and 1 in the solid phase. The dimensionless undercooling :math:`\Delta T` is 0 at the melting point.    

The governing equation for the temperature field is given by the heat flux equation with a source term due to the latent heat of solidification:    

.. math:: \frac{\partial\Delta T}{\partial t} = D_{T} \nabla^{2}\Delta T + \frac{\partial\phi}{\partial t}

The governing equation for the phase field is:    

.. math:: \tau\frac{\partial\phi}{\partial t} = \nabla\cdot D \nabla\phi + \phi\left(1 - \phi\right)\left(\phi - \frac{1}{2} - \frac{\kappa_{1}}{\pi}\arctan\left(\kappa_{2}\Delta T\right)\right)

The anisotropic diffusion tensor is given by:    

.. math:: D = \alpha^{2}\left(1 + c\beta\right)\left[\begin{matrix}1 + c\beta & -c\frac{\partial\beta}{\partial\psi}\\ c\frac{\partial\beta}{\partial\psi} & 1 + c\beta\end{matrix}\right]

where

.. math:: \beta = \frac{1 - \Phi^{2}}{1 + \Phi^{2}}

.. math:: \Phi = \tan\left(\frac{N}{2}\psi\right)

.. math:: \psi = \frac{\pi}{8} + \arctan\frac{\partial\phi/\partial y}{\partial\phi/\partial x}