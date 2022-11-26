# CorrFunc
Codes from bachelor thesis on correlation functions.

lustre          -> **readLustre.c**     -> outputLustre
- reads folders from lustre
- gets important histograms from root files (1D and 3D)

outputLustre    -> **makeCorrFunc.c**   -> outputCorrFunc
- creates correlation functions from outputLustre histograms
- performs fits
- comparison of fit results into txt file

outputCorrFunc -> **compFixedPairs** -> outputFixedPairs
- compares epsilons for fixed pair system
- corr. functions are with fit

outputLustre    -> **makeCorrFunc3d.c**   -> outputCorrFunc3d
- creates correlation functions from outputLustre 3d histograms
- performs fits on 3d histograms
- comparison of fit results into txt file

outputLustre    -> **makeCorrFunc3dProj.c**   -> outputCorrFunc3dProj
- creates correlation functions from outputLustre 3d histograms
- performs fits on 1d projections from 3d histograms
- comparison of fit results into txt file
                                       
