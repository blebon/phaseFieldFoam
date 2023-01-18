"""Converts all .vtk files to .vtp.

This script converts all VTK files in the postProcessing directory.

Example:
    Run python script in case root directory:

        $ python convert_vtk.py

Todo:
    * None

.. _Google Python Style Guide:
   http://google.github.io/styleguide/pyguide.html

"""

from __future__ import division, print_function

from itertools import chain
import multiprocessing
from os import path, remove, walk
from natsort import natsorted, ns
from sys import argv

import vtk

__author__ = "Bruno Lebon"
__copyright__ = "Copyright, 2022, Brunel University London"
__credits__ = ["Bruno Lebon"]
__email__ = "Bruno.Lebon@brunel.ac.uk"
__status__ = "Production"


def vtk2vtp(invtkfile, outvtpfile):
    """Converts a vtk file to vtp format.

    :param invtkfile: VTK file path and name.
    :type invtkfile: str
    :param outvtpfile: VTP file path and name.
    :type outvtpfile: str
    :return: 0 if successful.
    :rtype: int

    """
    reader = vtk.vtkPolyDataReader()
    reader.SetFileName(invtkfile)
    reader.Update()
    writer = vtk.vtkXMLPolyDataWriter()
    writer.SetFileName(outvtpfile)
    writer.SetDataModeToBinary()
    writer.SetInputData(reader.GetOutput())
    writer.Update()
    return 0


def convert_surfaces(vtkFile):
    """Converts a vtk file to vtp format if it does not exist.
    Deletes the vtk file upon successful conversion or if vtp
    file exists.

    :param vtkFile: VTK file path and name.
    :type vtkFile: str
    :return: 0 if successful.
    :rtype: int

    """
    vtpFile = vtkFile.replace(".vtk", ".vtp")
    if path.exists(vtkFile) and not path.exists(vtpFile):
        print(vtkFile)
        vtk2vtp(vtkFile, vtpFile)
    if path.exists(vtkFile) and path.exists(vtpFile):
        remove(vtkFile)
    return 0


if __name__ == "__main__":
    with multiprocessing.Pool(8) as pool:
        try:
            Walk = walk(f"postProcessing/{argv[1]}")
        except:
            Walk = walk(f"postProcessing")
        files_gen = chain.from_iterable(
            (path.join(root, f) for f in files if ".vtk" in f)
            for root, _, files in Walk
        )
        pool.map(convert_surfaces, natsorted(files_gen, alg=ns.FLOAT))
