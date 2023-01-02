# CorrFunc
Codes from bachelor thesis on correlation functions.

lustre          -> **readLustre1d/3d.c**     -> outputLustre1d/3d
- reads folders from lustre
- gets important histograms from root files (1D and 3D, separately)

outputLustre1d/3d    -> **makeCorrFunc1d/3d.c**   -> outputCorrFunc1d/3d
- creates correlation functions from outputLustre histograms
- performs fits
- comparison of fit results into txt file and latex table
- for 3d histograms - saving projectiles of histograms and fits

outputCorrFunc -> **compFixedPairs** -> outputFixedPairs/ladne
- compares epsilons for fixed pair system
- corr. functions are with fit

outputCorrFunc -> **compFixedEpsilon** -> outputFixedEpsilon/ladne
- compares pairs for fixed epsilon
- corr. functions are with fit

outputCorrFunc -> **compCases** -> outputCompCases/ladne
- compares cases for each system, epsilon fixed
- corr. functions are with fit


outputCorrFunc3d -> **compOutSideLong** -> outputOutSideLong/ladne
- compares projectiles from 3d histograms
- corr. functions are with fit



                                       
