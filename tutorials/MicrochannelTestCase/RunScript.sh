#!/bin/bash 
source /home/mnabil/OpenFOAM/OpenFOAM-v1612+/etc/bashrc

m4 system/blockMeshDict.m4 > system/blockMeshDict
blockMesh
checkMesh
cp -r orig0 0
m4 constant/perturbUDict1.m4 > constant/perturbUDict
perturbUChannel
rm constant/perturbUDict
m4 constant/perturbUDict2.m4 > constant/perturbUDict
perturbUChannel
decomposePar -force

# Run
mpirun -np 8 buoyantPimpleFoam_GAdaptCyc -parallel
