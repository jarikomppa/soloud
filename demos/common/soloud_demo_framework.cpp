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

#include <stdlib.h>
#if defined(_MSC_VER)
#include "SDL.h"
#include <windows.h>
#else
#include "SDL2/SDL.h"
#endif
#ifndef __EMSCRIPTEN__
#include "GL/glew.h"
#endif

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#include <GLES2/gl2.h>
#endif

#include <math.h>
#include <stdio.h>
#include "imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int gPressed[256], gWasPressed[256];
int gMouseX = 0;
int gMouseY = 0;
SDL_Window *gSDLWindow;
GLuint desktop_tex;

unsigned int DemoLoadTexture(char * aFilename)
{
	int x, y, comp;
	unsigned char *image = stbi_load(aFilename, &x, &y, &comp, 4);
	if (!image)
		return 0;
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(image);
	return tex;
}

GLuint loadShader(GLenum aShaderType, const char* aSource)
{
	GLuint shader = glCreateShader(aShaderType);
	if (shader)
	{
		glShaderSource(shader, 1, &aSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				char* buf = (char*)malloc(infoLen);
				if (buf)
				{
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					printf("Could not compile shader %d:\n%s\n", aShaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

GLuint createProgram(const char *aVertexSource, const char *aFragmentSource)
{
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, aVertexSource);
	if (!vertexShader)
	{
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, aFragmentSource);
	if (!pixelShader)
	{
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program)
	{
		glAttachShader(program, vertexShader);
		glAttachShader(program, pixelShader);
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE)
		{
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength)
			{
				char *buf = (char*)malloc(bufLength);
				if (buf)
				{
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					printf("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}

static GLuint       g_FontTexture = 0;
static int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;

// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
void ImImpl_RenderDrawLists(ImDrawData* draw_data)
{
	// Backup GL state
	GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
	GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
	GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
	GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
	GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);
	
	// Handle cases of screen coordinates != from framebuffer coordinates (e.g. retina displays)
	ImGuiIO& io = ImGui::GetIO();
	float fb_height = io.DisplaySize.y * io.DisplayFramebufferScale.y;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Setup orthographic projection matrix
	const float ortho_projection[4][4] =
	{
		{ 2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
	};
	glUseProgram(g_ShaderHandle);
	glUniform1i(g_AttribLocationTex, 0);
	glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);

	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

		glEnableVertexAttribArray(g_AttribLocationPosition);
		glEnableVertexAttribArray(g_AttribLocationUV);
		glEnableVertexAttribArray(g_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
		for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
		{
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	// Restore modified GL state
	glUseProgram(last_program);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
	if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
	if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
	if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
	if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
}


void ImImpl_CreateFontsTexture()
{
	ImGuiIO& io = ImGui::GetIO();

	// Build texture atlas
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

	// Create OpenGL texture
	glGenTextures(1, &g_FontTexture);
	glBindTexture(GL_TEXTURE_2D, g_FontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

	// Cleanup (don't clear the input data if you want to append new fonts later)
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();
}

bool ImImpl_CreateDeviceObjects()
{
	// Backup GL state
	GLint last_texture, last_array_buffer;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);

	const GLchar *vertex_shader =
#ifdef __EMSCRIPTEN__
		"precision highp float;\n"
#endif
		"uniform mat4 ProjMtx;\n"
		"attribute vec2 Position;\n"
		"attribute vec2 UV;\n"
		"attribute vec4 Color;\n"
		"varying vec2 Frag_UV;\n"
		"varying vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
#ifdef __EMSCRIPTEN__
		"precision mediump float;\n"
#endif
		"uniform sampler2D Texture;\n"
		"varying vec2 Frag_UV;\n"
		"varying vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = Frag_Color * texture2D( Texture, Frag_UV.st);\n"
		"}\n";

	g_ShaderHandle = glCreateProgram();
	g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
	g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(g_VertHandle, 1, &vertex_shader, 0);
	glShaderSource(g_FragHandle, 1, &fragment_shader, 0);
	glCompileShader(g_VertHandle);
	glCompileShader(g_FragHandle);
	glAttachShader(g_ShaderHandle, g_VertHandle);
	glAttachShader(g_ShaderHandle, g_FragHandle);
	glLinkProgram(g_ShaderHandle);

	g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
	g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
	g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
	g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
	g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

	glGenBuffers(1, &g_VboHandle);
	glGenBuffers(1, &g_ElementsHandle);

	glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
	glEnableVertexAttribArray(g_AttribLocationPosition);
	glEnableVertexAttribArray(g_AttribLocationUV);
	glEnableVertexAttribArray(g_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

	ImImpl_CreateFontsTexture();

	// Restore modified GL state
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);

	return true;
}

static unsigned int flat_shader_handle, flat_position_location, flat_color_location;

void framework_init_flat()
{
	const GLchar *vertex_shader =
#ifdef __EMSCRIPTEN__
		"precision highp float;\n"
#endif
		"attribute vec2 Position;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
#ifdef __EMSCRIPTEN__
		"precision mediump float;\n"
#endif
		"uniform vec4 Color;\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = Color;\n"
		"}\n";

	flat_shader_handle = createProgram(vertex_shader, fragment_shader);

	flat_position_location = glGetAttribLocation(flat_shader_handle, "Position");
	flat_color_location = glGetUniformLocation(flat_shader_handle, "Color");
}

static unsigned int tex_shader_handle, tex_position_location, tex_uv_location, tex_texture_position;
void framework_init_tex()
{
	const GLchar *vertex_shader =
#ifdef __EMSCRIPTEN__
		"precision highp float;\n"
#endif
		"attribute vec2 Position;\n"
		"attribute vec2 TexCoord;\n"
		"varying vec2 Frag_UV;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = TexCoord;\n"
		"	gl_Position = vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
#ifdef __EMSCRIPTEN__
		"precision mediump float;\n"
#endif
		"uniform sampler2D Texture;\n"
		"varying vec2 Frag_UV;\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = texture2D(Texture, Frag_UV.st);\n"
		"}\n";

	tex_shader_handle = createProgram(vertex_shader, fragment_shader);

	tex_position_location = glGetAttribLocation(tex_shader_handle, "Position");
	tex_uv_location = glGetAttribLocation(tex_shader_handle, "TexCoord");
	tex_texture_position = glGetUniformLocation(flat_shader_handle, "Texture");
}

void DemoTriangle(float x0, float y0, float x1, float y1, float x2, float y2, unsigned int color)
{
	glEnableVertexAttribArray(flat_position_location);

	glUseProgram(flat_shader_handle);	

	float buf[3 * 2];
	buf[0] = x0;
	buf[1] = y0;
	buf[2] = x1;
	buf[3] = y1;
	buf[4] = x2;
	buf[5] = y2;

	int i;
	for (i = 0; i < 3; i++)
	{
		buf[i * 2 + 0] = (buf[i * 2 + 0] / 400) - 1;
		buf[i * 2 + 1] = 1 - (buf[i * 2 + 1] / 200);
	}

	glVertexAttribPointer(flat_position_location, 2, GL_FLOAT, GL_FALSE, 0, buf);

	glUniform4f(flat_color_location, ((color >> 16) & 0xff) * (1 / 255.0f), ((color >> 8) & 0xff) * (1 / 255.0f), ((color >> 0) & 0xff) * (1 / 255.0f), ((color >> 24) & 0xff) * (1 / 255.0f));	

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(flat_position_location);
	glUseProgram(0);
}

void DemoQuad(float x0, float y0, float x1, float y1, unsigned int color)
{
	DemoTriangle(x0, y0, x0, y1, x1, y1, color);
	DemoTriangle(x0, y0, x1, y0, x1, y1, color);
}

void DemoTexQuad(int tex, float x0, float y0, float x1, float y1)
{
	glEnableVertexAttribArray(tex_position_location);
	glEnableVertexAttribArray(tex_uv_location);

	glUseProgram(tex_shader_handle);

	float buf[4 * 2];
	buf[0] = x0;
	buf[1] = y0;
	buf[2] = x1;
	buf[3] = y0;
	buf[4] = x0;
	buf[5] = y1;
	buf[6] = x1;
	buf[7] = y1;

	int i;
	for (i = 0; i < 4; i++)
	{
		buf[i * 2 + 0] = (buf[i * 2 + 0] / 400) - 1;
		buf[i * 2 + 1] = 1 - (buf[i * 2 + 1] / 200);
	}

	float uvbuf[4 * 2] =
	{
		0, 0,
		1, 0,
		0, 1,
		1, 1
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glVertexAttribPointer(tex_position_location, 2, GL_FLOAT, GL_FALSE, 0, buf);
	glVertexAttribPointer(tex_uv_location, 2, GL_FLOAT, GL_FALSE, 0, uvbuf);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(tex_texture_position, 0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(tex_position_location);
	glDisableVertexAttribArray(tex_uv_location);
	glUseProgram(0);
	glDisable(GL_BLEND);
}


struct UIState
{
	int mousex;
	int mousey;
	int mousedown;
	int scroll;

	char textinput[32];
};

static UIState gUIState;

void InitImGui()
{
	gUIState.mousex = gUIState.mousey = gUIState.mousedown = gUIState.scroll = 0;
	memset(gUIState.textinput, 0, sizeof(gUIState.textinput));

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.0f / 60.0f;                     
	io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = SDLK_a;
	io.KeyMap[ImGuiKey_C] = SDLK_c;
	io.KeyMap[ImGuiKey_V] = SDLK_v;
	io.KeyMap[ImGuiKey_X] = SDLK_x;
	io.KeyMap[ImGuiKey_Y] = SDLK_y;
	io.KeyMap[ImGuiKey_Z] = SDLK_z;
	io.RenderDrawListsFn = ImImpl_RenderDrawLists;
	io.IniFilename = 0;

	ImGuiStyle&s = ImGui::GetStyle();
	s.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.50f, 1.00f);
}


void UpdateImGui()
{
	ImGuiIO& io = ImGui::GetIO();

	// Setup resolution (every frame to accommodate for window resizing)
	io.DisplaySize = ImVec2((float)800, (float)400); 

	// Setup time step
	static double time = 0.0f;
	const double current_time = SDL_GetTicks() / 1000.0;
	if (current_time == time)
		return;
	io.DeltaTime = (float)(current_time - time);
	time = current_time;

	io.MousePos = ImVec2((float)gUIState.mousex, (float)gUIState.mousey);
	io.MouseDown[0] = gUIState.mousedown != 0;
	io.MouseDown[1] = 0;

	if (gUIState.scroll)
	{
		io.MouseWheel += (float)gUIState.scroll * 0.5f;
		gUIState.scroll = 0;
	}

	if (gUIState.textinput[0])
	{
		io.AddInputCharactersUTF8(gUIState.textinput);
		gUIState.textinput[0] = 0;
	}

	for (int n = 0; n < 256; n++)
		io.KeysDown[n] = gPressed[n] != 0;
    io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
    io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
    io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
}


void DemoInit()
{
	int i;
	for (i = 0; i < 256; i++)
		gPressed[i] = gWasPressed[i] = 0;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
	{
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		SDL_Quit();
		exit(0);
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;


	gSDLWindow = SDL_CreateWindow(
		"",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		400,
		flags);

	SDL_GLContext glcontext = SDL_GL_CreateContext(gSDLWindow);

	SDL_GL_SetSwapInterval(1);


	glViewport(0, 0, 800, 400);

	// set window title
	SDL_SetWindowTitle(gSDLWindow, "http://soloud-audio.com");

#ifndef __EMSCRIPTEN__	
	glewInit();
#endif

	InitImGui();
	framework_init_flat();
	framework_init_tex();
	desktop_tex = DemoLoadTexture("graphics/soloud_bg.png");

	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);

}

void DemoUpdateStart()
{
	if (!g_FontTexture)
		ImImpl_CreateDeviceObjects();

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			// If a key is pressed, report it to the widgets
			//			gUIState.keyentered = event.key.keysym.sym;
			//			gUIState.keymod = event.key.keysym.mod;
			// if key is ASCII, accept it as character input
			//			if ((event.key.keysym.unicode & 0xFF80) == 0)
			//				gUIState.keychar = event.key.keysym.unicode & 0x7f;
			if (event.key.keysym.sym < 256)
				gPressed[event.key.keysym.sym] = 1;

			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym < 256)
				gPressed[event.key.keysym.sym] = 0;
			//			handle_key(event.key.keysym.sym, 0);
			break;
	    case SDL_TEXTINPUT:
			strncpy(gUIState.textinput, event.text.text, sizeof(gUIState.textinput));
			gUIState.textinput[sizeof(gUIState.textinput)-1] = 0;
			break;
		case SDL_MOUSEMOTION:
			// update mouse position
			gUIState.mousex = event.motion.x;
			gUIState.mousey = event.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			// update button down state if left-clicking
			if (event.button.button == 1)
			{
				gUIState.mousedown = 1;
			}
			if (event.button.button == 4)
			{
				gUIState.scroll = +1;
			}
			if (event.button.button == 5)
			{
				gUIState.scroll = -1;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			// update button down state if left-clicking
			if (event.button.button == 1)
			{
				gUIState.mousedown = 0;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0)
			{
				gUIState.scroll += 1;
			}
			else if (event.wheel.y < 0)
			{
				gUIState.scroll -= 1;
			}
			break;
		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;
		}
	}
	glClearColor(0.2f, 0.2f, 0.4f, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	DemoTexQuad(desktop_tex, 0, 0, 800, 400);
	UpdateImGui();

	gMouseX = gUIState.mousex;
	gMouseY = gUIState.mousey;

	// Start the frame
	ImGui::NewFrame();
	//ImGui::ShowTestWindow();
}

void DemoUpdateEnd()
{
	// End frame
	ImGui::Render();
	SDL_GL_SwapWindow(gSDLWindow);
}

int DemoTick()
{
	return SDL_GetTicks();
}

void DemoYield()
{
	SDL_Delay(1);
}

extern int DemoEntry(int argc, char *argv[]);
extern void DemoMainloop();

// Entry point
int main(int argc, char *argv[])
{
	DemoInit();
	int res = DemoEntry(argc, argv);
	if (res != 0)
		return res;
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(DemoMainloop, 60, 0);
#else
	while (1)
	{
		if (ImGui::IsKeyPressed(SDLK_ESCAPE) && !ImGui::GetIO().WantCaptureKeyboard)
			break;

		DemoMainloop();
	}
#endif
	return 0;
}