@ECHO OFF
set runs=%1
if "%1"=="" set runs=5
@ECHO ON
Release\Stage0_SIMDSpheres.exe -runs %runs% -blockSize 16 -size 1024 1024 -samples 4 -input Scenes/cornell.txt  
Release\Stage0_SIMDSpheres.exe -runs %runs% -blockSize 16 -size 1000 1000 -samples 4 -input Scenes/allmaterials.txt 
Release\Stage0_SIMDSpheres.exe -runs %runs% -blockSize 16 -size 1280  720 -samples 1 -input Scenes/5000spheres.txt 
Release\Stage0_SIMDSpheres.exe -runs %runs% -blockSize 16 -size 1024 1024 -samples 1 -input Scenes/dudes.txt 
Release\Stage0_SIMDSpheres.exe -runs %runs% -blockSize 16 -size 1024 1024 -samples 1 -input Scenes/cornell-199lights.txt
cmd \k