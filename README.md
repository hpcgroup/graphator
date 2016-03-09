Graphator v1.0
==============

Graphator is a collection of relatively simple sequential programs that
generate communication graphs/matrices for commonly occurring patterns in
parallel programs. Currently, there is support for five communication patterns:
two-dimensional 4-point stencil, four-dimensional 8-point stencil, all-to-alls
over sub-communicators, random near-neighbor communication, and near-neighbor
communication.

### Directory Structure

2D Structured Grid	2dstencil
4D Structured Grid	4dstencil
Many to many		many2many
Unstructured Mesh	umesh
Uniform Spread		spread

### Release

Copyright (c) 2014, Lawrence Livermore National Security, LLC.
Produced at the Lawrence Livermore National Laboratory.

Written by:
```
    Nikhil Jain <nikhil.jain@acm.org>
    Abhinav Bhatele <bhatele@llnl.gov>
```

LLNL-CODE-678959. All rights reserved.

This file is part of Graphator. For details, see:
https://github.com/LLNL/graphator.
Please also read the LICENSE file for our notice and the LGPL.
