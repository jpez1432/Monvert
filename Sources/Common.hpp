
#ifndef COMMON_HPP
#define COMMON_HPP

#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <SOIL2.h>

std::string GetInfo();
std::string FileOpen(HWND hWnd, char *Filter, char *InitPath, char *DefaultExt);
std::string FileSave(HWND hWnd, char *Filter, char *InitPath, char *DefaultExt);

#endif
