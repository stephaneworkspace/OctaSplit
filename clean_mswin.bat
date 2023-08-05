@echo off
echo Suppression des répertoires...
rd /s /q "_deps" "CMakeFiles" "OctaSplit.dir" "OctaSplit_artefacts" "x64" ".vs" "build"

echo Suppression des fichiers...
del /q ALL_BUILD.vcxproj ALL_BUILD.vcxproj.filters ALL_BUILD.vcxproj.user cmake_install.cmake CMakeCache.txt CPackConfig.cmake CPackSourceConfig.cmake INSTALL.vcxproj INSTALL.vcxproj.filters OctaSplit.sln OctaSplit.vcxproj OctaSplit.vcxproj.filters PACKAGE.vcxproj PACKAGE.vcxproj.filters ZERO_CHECK.vcxproj ZERO_CHECK.vcxproj.filters

echo Le nettoyage est terminé.
