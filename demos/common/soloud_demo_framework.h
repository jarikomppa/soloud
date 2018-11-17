/*  SoLoud demo framework
*  version 1.0, April 2015
*
*  Copyright (C) 2015 Jari Komppa
*
*  This software is provided 'as-is', without any express or implied
*  warranty.  In no event will the authors be held liable for any damages
*  arising from the use of this software.
*
*  Permission is granted to anyone to use this software for any purpose,
*  including commercial applications, and to alter it and redistribute it
*  freely, subject to the following restrictions:
*
*  1. The origin of this software must not be misrepresented; you must not
*     claim that you wrote the original software. If you use this software
*     in a product, an acknowledgment in the product documentation would be
*     appreciated but is not required.
*  2. Altered source versions must be plainly marked as such, and must not be
*     misrepresented as being the original software.
*  3. This notice may not be removed or altered from any source distribution.
*
* Jari Komppa http://iki.fi/sol/
*
*************************************
* Parts copied from ImGui examples
*/

#define ONCE(x) { static int oncetrigger = 1; if (oncetrigger) { x; oncetrigger = 0; }}

extern int gPressed[256], gWasPressed[256];
extern int gMouseX, gMouseY;
unsigned int DemoLoadTexture(const char * aFilename);
void DemoInit();
void DemoUpdateStart();
void DemoUpdateEnd();
int DemoTick();
void DemoYield();
void DemoTriangle(float x0, float y0, float x1, float y1, float x2, float y2, unsigned int color);
void DemoQuad(float x0, float y0, float x1, float y1, unsigned int color);
void DemoTexQuad(int tex, float x0, float y0, float x1, float y1);