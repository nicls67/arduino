cd ../doxygen
doxygen
latex/make.bat

cd /D %~dp0
copy %cd%\latex\refman.pdf %cd%