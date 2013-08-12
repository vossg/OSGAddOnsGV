%~d0
cd %~p0

mkdir new_fcd_output
python D:\Work\dev\OpenSG2_2011_05\OpenSG2_git_2011_05\opensg\Tools\fcd2code\fcd2code -d OSGTRIPSAudioSystem.fcd -p .\new_fcd_output -r D:\Work\dev\OpenSG2_2011_05\OpenSG2_git_2011_05\opensg -b -f                                                                             

python D:\Work\dev\OpenSG2_2011_05\OpenSG2_git_2011_05\opensg\Tools\fcd2code\fcd2code -d OSGTRIPSSound.fcd -p .\new_fcd_output -r D:\Work\dev\OpenSG2_2011_05\OpenSG2_git_2011_05\opensg -b -f