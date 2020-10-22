doskey magick = c:\Program Files\ImageMagick-7.0.10-Q8\magick.exe

Release\Stage5.exe -runs 1 -blockSize 8 -size 8 8 -samples 1 -threads 1 -output Outputs/stage5_01.bmp -input Scenes/cornell.txt           
Release\Stage5.exe -runs 1 -blockSize 8 -size 8 8 -samples 1 -threads 1 -output Outputs/stage5_02.bmp -input Scenes/allmaterials.txt 
Release\Stage5.exe -runs 1 -blockSize 8 -size 8 8 -samples 1 -threads 1 -output Outputs/stage5_03.bmp -input Scenes/5000spheres.txt 
Release\Stage5.exe -runs 1 -blockSize 8 -size 8 8 -samples 1 -threads 1 -output Outputs/stage5_04.bmp -input Scenes/dudes.txt 
Release\Stage5.exe -runs 1 -blockSize 8 -size 8 8 -samples 1 -threads 1 -output Outputs/stage5_05.bmp -input Scenes/cornell-199lights.txt

magick compare -metric mae Outputs\stage5_01.bmp Outputs_REFERENCE\stage4_01_MathSIMD.bmp Outputs\stage5diff_01.bmp
magick compare -metric mae Outputs\stage5_02.bmp Outputs_REFERENCE\stage4_02_MathSIMD.bmp Outputs\stage5diff_02.bmp
magick compare -metric mae Outputs\stage5_03.bmp Outputs_REFERENCE\stage4_03_MathSIMD.bmp Outputs\stage5diff_03.bmp
magick compare -metric mae Outputs\stage5_04.bmp Outputs_REFERENCE\stage4_04_MathSIMD.bmp Outputs\stage5diff_04.bmp
magick compare -metric mae Outputs\stage5_05.bmp Outputs_REFERENCE\stage4_05_MathSIMD.bmp Outputs\stage5diff_05.bmp

Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 1 -output Outputs/stage5_06.bmp -input Scenes/cornell.txt           
Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 1 -output Outputs/stage5_07.bmp -input Scenes/allmaterials.txt 
Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 1 -output Outputs/stage5_08.bmp -input Scenes/5000spheres.txt 
Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 1 -output Outputs/stage5_09.bmp -input Scenes/dudes.txt 
Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 1 -output Outputs/stage5_10.bmp -input Scenes/cornell-199lights.txt

magick compare -metric mae Outputs\stage5_06.bmp Outputs_REFERENCE\stage4_06_MathSIMD.bmp Outputs\stage5diff_06.bmp
magick compare -metric mae Outputs\stage5_07.bmp Outputs_REFERENCE\stage4_07_MathSIMD.bmp Outputs\stage5diff_07.bmp
magick compare -metric mae Outputs\stage5_08.bmp Outputs_REFERENCE\stage4_08_MathSIMD.bmp Outputs\stage5diff_08.bmp
magick compare -metric mae Outputs\stage5_09.bmp Outputs_REFERENCE\stage4_09_MathSIMD.bmp Outputs\stage5diff_09.bmp
magick compare -metric mae Outputs\stage5_10.bmp Outputs_REFERENCE\stage4_10_MathSIMD.bmp Outputs\stage5diff_10.bmp

Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 2 -output Outputs/stage5_11.bmp -input Scenes/cornell.txt           
Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 2 -output Outputs/stage5_12.bmp -input Scenes/allmaterials.txt 
Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 2 -output Outputs/stage5_13.bmp -input Scenes/5000spheres.txt 
Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 2 -output Outputs/stage5_14.bmp -input Scenes/dudes.txt 
Release\Stage5.exe -runs 1 -blockSize 16 -size 256 256 -samples 2 -output Outputs/stage5_15.bmp -input Scenes/cornell-199lights.txt
                                                                                
magick compare -metric mae Outputs\stage5_11.bmp Outputs_REFERENCE\stage4_11_MathSIMD.bmp Outputs\stage5diff_11.bmp
magick compare -metric mae Outputs\stage5_12.bmp Outputs_REFERENCE\stage4_12_MathSIMD.bmp Outputs\stage5diff_12.bmp
magick compare -metric mae Outputs\stage5_13.bmp Outputs_REFERENCE\stage4_13_MathSIMD.bmp Outputs\stage5diff_13.bmp
magick compare -metric mae Outputs\stage5_14.bmp Outputs_REFERENCE\stage4_14_MathSIMD.bmp Outputs\stage5diff_14.bmp
magick compare -metric mae Outputs\stage5_15.bmp Outputs_REFERENCE\stage4_15_MathSIMD.bmp Outputs\stage5diff_15.bmp
