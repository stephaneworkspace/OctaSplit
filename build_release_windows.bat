@echo off
echo Suppression des répertoires...
rd /s /q "_deps" "CMakeFiles" "OctaSplit.dir" "OctaSplit_artefacts" "x64"

echo Suppression des fichiers...
del /q ALL_BUILD.vcxproj ALL_BUILD.vcxproj.filters ALL_BUILD.vcxproj.user cmake_install.cmake CMakeCache.txt CPackConfig.cmake CPackSourceConfig.cmake INSTALL.vcxproj INSTALL.vcxproj.filters OctaSplit.sln OctaSplit.vcxproj OctaSplit.vcxproj.filters PACKAGE.vcxproj PACKAGE.vcxproj.filters ZERO_CHECK.vcxproj ZERO_CHECK.vcxproj.filters

echo Le nettoyage est terminé.

REM Vérifier si le dossier build existe et le créer si nécessaire
IF NOT EXIST build (mkdir build)
cd build

REM Supprimer le dossier 'bin' s'il existe
IF EXIST bin (rmdir /S /Q bin)

REM Générer les fichiers de projet avec CMake
cmake -DCMAKE_BUILD_TYPE=Release ..
REM vous pouvez spécifier les architectures cibles comme ceci si nécessaire
REM cmake -DCMAKE_BUILD_TYPE=Release -A x64 ..

REM Compiler le projet
cmake --build . --config Release -- /m /project OctaSplit
echo Compilation terminé