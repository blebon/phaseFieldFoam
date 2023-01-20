#!/usr/bin/env python3
"""Plots the sump profile.

This script plots the sump profile using the output in the
postProcessing directory.

Example:
    Run python script in case root directory:

        $ python3 plot_sump.py

Todo:
    * None

.. _Google Python Style Guide:
   http://google.github.io/styleguide/pyguide.html

"""

from __future__ import division, print_function

from os import listdir, path, sep
from natsort import natsorted, ns

import vtk
from vtk.numpy_interface import dataset_adapter as dsa
from vtk.util.numpy_support import vtk_to_numpy

import matplotlib.tri as tr
import matplotlib.pyplot as plt

from natsort import natsorted, ns
from numpy import (
    linspace,
    sqrt,
    around,
    where,
)

__author__ = "Bruno Lebon"
__copyright__ = "Copyright, 2023, Brunel University London"
__credits__ = ["Bruno Lebon"]
__email__ = "Bruno.Lebon@brunel.ac.uk"
__status__ = "Production"


def load_slice(filename):
    """Read field from .vtp file.

    :param filename: Filename of .vtp file, including path.
    :type filename: str
    :return: A tuple containing points, triangulation and scalar field.
    :rtype: tuple[numpy.array]

    """
    if not path.exists(filename):
        return None
    reader = vtk.vtkXMLPolyDataReader()
    reader.SetFileName(filename)
    reader.Update()

    data = reader.GetOutput()
    arrays = dsa.WrapDataObject(data)

    # Extracting triangular information
    cells = arrays.GetPolys()
    triangles = vtk_to_numpy(cells.GetData())

    ntri = cells.GetNumberOfCells()
    tris = [None] * ntri
    tri = [None] * ntri

    j = 0
    offset = 0

    for _ in range(ntri):
        points = triangles[j]
        tris[_] = list(triangles[j : j + points + 1])

        tri[_ + offset] = list([triangles[j + 1], triangles[j + 2], triangles[j + 3]])

        if points == 4:
            offset += 1
            tri.insert(
                _ + offset, list([triangles[j + 3], triangles[j + 4], triangles[j + 1]])
            )

        j += points + 1

    # Extracting points
    points = vtk_to_numpy(arrays.GetPoints())
    x, y, z = points.T

    # Extract data
    keys = arrays.GetPointData().keys()
    _dict = {}
    for i, key in enumerate(keys):
        _dict[key] = vtk_to_numpy(arrays.GetPointData().GetArray(i))

    return (x, y, z, tri, _dict)


def plot_crystal(image_name="Crystal.png", cmap="jet"):
    """Plots crystal contour.

    :param image_name: Filename of image, including path.
    :type image_name: str
    :param cmap: Color map for plotting contour.
    :type cmap: str or matplotlib.colormaps
    :return: 0 if successful.
    :rtype: int

    """
    plt.clf()
    # plt.rc('text', usetex=True)
    fig, ax = plt.subplots(nrows=1, ncols=1)

    casedir = f"postProcessing{sep}zNormal"
    times = natsorted(listdir(casedir), alg=ns.FLOAT)
    time = times[-1]

    # fi
    vtkFile = f"{casedir}/{time}/cutPlane.vtp"
    x, y, z, tri, _dict = load_slice(vtkFile)
    levels = linspace(-0.10, 0.05, 12)
    levels = around(levels, decimals=2)
    triang = tr.Triangulation(x, y, triangles=tri)
    fi = _dict["fi"]
    dT = _dict["dT"]
    ax.tricontour(triang, fi, [0.5,], colors=["k",])
    cs = ax.tricontourf(triang, dT, cmap=cmap, levels=levels, extend="min")

    cbar = plt.colorbar(cs)
    cbar.set_label(r"$\Delta T$")

    ratio = 1.0
    ax.set_ylim([-6.25, 6.25])
    ax.set_xlim([-6.25, 6.25])
    ax.set_aspect(1.0 * ratio)

    ax.tick_params(direction="out", which="both")
    ax.minorticks_on()

    fig.set_size_inches(6, 4)
    plt.savefig(
        image_name,
        transparent=False,
        dpi=1000,
        # bbox_extra_artists=(cbar,),
        bbox_inches="tight",
    )

    return 0


if __name__ == "__main__":
    plot_crystal(image_name="Crystal.png", cmap="hot")
