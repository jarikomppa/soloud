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


#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

// Shader variables
static int shader_handle;
static int texture_location, proj_mtx_location;
static int position_location, uv_location, color_location;
static size_t vbo_max_size = 20000;
static unsigned int vbo_handle;
static unsigned int desktop_tex;

// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
static void ImImpl_RenderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
{
	if (cmd_lists_count == 0)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glEnableVertexAttribArray(position_location);
	glEnableVertexAttribArray(uv_location);
	glEnableVertexAttribArray(color_location);

	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(color_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	// Setup orthographic projection matrix
	const float width = ImGui::GetIO().DisplaySize.x;
	const float height = ImGui::GetIO().DisplaySize.y;
	const float ortho_projection[4][4] =
	{
		{ 2.0f / width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / -height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
	};
	glUseProgram(shader_handle);
	glUniform1i(texture_location, 0);
	glUniformMatrix4fv(proj_mtx_location, 1, GL_FALSE, &ortho_projection[0][0]);

	// Grow our buffer according to what we need
	size_t total_vtx_count = 0;
	for (int n = 0; n < cmd_lists_count; n++)
		total_vtx_count += cmd_lists[n]->vtx_buffer.size();
	
	size_t neededBufferSize = total_vtx_count * sizeof(ImDrawVert);

	// Copy and convert all vertices into a single contiguous buffer
	unsigned char* buf = new unsigned char[neededBufferSize];
	unsigned char* buffer_data = buf;
	if (!buffer_data)
		return;
	for (int n = 0; n < cmd_lists_count; n++)
	{
		const ImDrawList* cmd_list = cmd_lists[n];
		memcpy(buffer_data, &cmd_list->vtx_buffer[0], cmd_list->vtx_buffer.size() * sizeof(ImDrawVert));
		buffer_data += cmd_list->vtx_buffer.size() * sizeof(ImDrawVert);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glBufferData(GL_ARRAY_BUFFER, neededBufferSize, buf, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] buf;

	int cmd_offset = 0;
	for (int n = 0; n < cmd_lists_count; n++)
	{
		const ImDrawList* cmd_list = cmd_lists[n];
		int vtx_offset = cmd_offset;
		const ImDrawCmd* pcmd_end = cmd_list->commands.end();
		for (const ImDrawCmd* pcmd = cmd_list->commands.begin(); pcmd != pcmd_end; pcmd++)
		{
			glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->texture_id);
			glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
			glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
			vtx_offset += pcmd->vtx_count;
		}
		cmd_offset = vtx_offset;
	}

	// Restore modified state
	glUseProgram(0);
	glDisable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(position_location);
	glDisableVertexAttribArray(uv_location);
	glDisableVertexAttribArray(color_location);
}


static void LoadFontsTexture()
{
	ImGuiIO& io = ImGui::GetIO();
	//ImFont* my_font1 = io.Fonts->AddFontDefault();
	//ImFont* my_font2 = io.Fonts->AddFontFromFileTTF("extra_fonts/Karla-Regular.ttf", 15.0f);
	//ImFont* my_font3 = io.Fonts->AddFontFromFileTTF("extra_fonts/ProggyClean.ttf", 13.0f); my_font3->DisplayOffset.y += 1;
	//ImFont* my_font4 = io.Fonts->AddFontFromFileTTF("extra_fonts/ProggyTiny.ttf", 10.0f); my_font4->DisplayOffset.y += 1;
	//ImFont* my_font5 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 20.0f, io.Fonts->GetGlyphRangesJapanese());

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	GLuint tex_id;
	if (io.Fonts->TexID)
	{
		tex_id = (int)io.Fonts->TexID;
	}
	else
	{
		glGenTextures(1, &tex_id);
	}
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)tex_id;
}

void ImImpl_InitGL()
{
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

	shader_handle = createProgram(vertex_shader, fragment_shader);

	texture_location = glGetUniformLocation(shader_handle, "Texture");
	proj_mtx_location = glGetUniformLocation(shader_handle, "ProjMtx");
	position_location = glGetAttribLocation(shader_handle, "Position");
	uv_location = glGetAttribLocation(shader_handle, "UV");
	color_location = glGetAttribLocation(shader_handle, "Color");

	glGenBuffers(1, &vbo_handle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glBufferData(GL_ARRAY_BUFFER, vbo_max_size, NULL, GL_DYNAMIC_DRAW);

	LoadFontsTexture();
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


	glVertexAttribPointer(tex_position_location, 2, GL_FLOAT, GL_FALSE, 0, buf);
	glVertexAttribPointer(tex_uv_location, 2, GL_FLOAT, GL_FALSE, 0, uvbuf);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(tex_texture_position, 0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(tex_position_location);
	glDisableVertexAttribArray(tex_uv_location);
	glUseProgram(0);
}


void InitImGui()
{
	ImImpl_InitGL();

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.0f / 60.0f;                     
	io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;              
	io.KeyMap[ImGuiKey_LeftArrow] = SDLK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDLK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDLK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDLK_DOWN;
	io.KeyMap[ImGuiKey_Home] = SDLK_HOME;
	io.KeyMap[ImGuiKey_End] = SDLK_END;
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

struct UIState
{
	int mousex;
	int mousey;
	int mousedown;
	int scroll;

	int keychar;
};

static UIState gUIState;


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

	if (gUIState.keychar)
	{
		io.AddInputCharacter(gUIState.keychar);
		gUIState.keychar = 0;
	}
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
		DemoMainloop();
	}
#endif
	return 0;
}