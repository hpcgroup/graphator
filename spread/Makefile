##############################################################################
# Copyright (c) 2014, Lawrence Livermore National Security, LLC.
# Produced at the Lawrence Livermore National Laboratory.
#
# Written by:
#     Nikhil Jain <nikhil.jain@acm.org>
#     Abhinav Bhatele <bhatele@llnl.gov>
#
# LLNL-CODE-678959. All rights reserved.
#
# This file is part of Graphator. For details, see:
# https://github.com/LLNL/graphator
# Please also read the LICENSE file for our notice and the LGPL.
##############################################################################

include ../Makefile.include

all: spread

spread: spread.C
	$(CXX) $(COPTS) -o spread $<

clean:
	rm -f *.o spread
