# CorrFunc
Codes from bachelor thesis on correlation functions.

lustre          -> **readLustre.c**     -> outputLustre
- reads folders from lustre
- gets important histograms from root files

outputLustre    -> **makeCorrFunc.c**   -> outputCorrFunc
- creates correlation functions from outputLustre histograms
- performs fits
- comparison of fit results

outputCorrFunc -> **compFixedPairs** -> outputFixedPairs
- compares epsilons for fixed pair system
- corr. functions are with fit
                                       
