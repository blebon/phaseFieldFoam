#!/usr/bin/env python3
"""Generates a .pvd XML file to post-process surfaces function
objects.

This script assembles the VTK files in separate time directories.

Example:
    Run python script in case root directory:

        $ python generate_pvd.py

Todo:
    * None

.. _Google Python Style Guide:
   http://google.github.io/styleguide/pyguide.html

"""

from __future__ import division, print_function

import os
import xml.etree.cElementTree as ET

from natsort import natsorted


def create_xml(variable="U", surface="zNormal", file_format="vtp"):
    """Create <variable>.pvd file with existing VTK files.

    :param variable: Name of field.
    :type variable: str
    :param surface: Surface of vtk/vtp files.
    :type surface: str
    :param file_format: vtp or vtk.
    :type file_format: str
    :return: 0 if successful.
    :rtype: int

    """
    root = ET.Element(
        "VTKFile",
        type="Collection",
        version="0.1",
        byte_order="LittleEndian",
        compressor="vtkZLibDataCompressor",
    )
    root.text = "\n    "
    collection = ET.SubElement(root, "Collection")
    collection.text = "\n        "
    collection.tail = "\n"
    for time in natsorted(os.listdir(f"postProcessing/{surface}")):
        # print(time)
        try:
            dataset.tail = "\n        "
        except:
            pass
        dataset = ET.SubElement(
            collection,
            "DataSet",
            timestep="{0:s}".format(time),
            group="",
            part="0",
            file=f"{surface}/{time}/{variable}_cutPlane.{file_format}",
        )
    dataset.tail = "\n    "
    tree = ET.ElementTree(root)
    tree.write(
        "postProcessing/{0:s}_{1:s}.pvd".format(variable, surface), xml_declaration=True
    )
    return 0


def write_pvd(
    variables=("fi", "dT",),
    surfaces=("zNormal",),
):
    """Write PVD files for requested slices."""
    for variable in variables:
        for surface in surfaces:
            create_xml(variable, surface)


if __name__ == "__main__":
    write_pvd()
