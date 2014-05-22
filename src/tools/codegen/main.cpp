/*
SoLoud audio engine
Copyright (c) 2013-2014 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <string>

#define VERSION "SoLoud C-Api Code Generator (c)2013-2014 Jari Komppa http://iki.fi/sol/"

#define OUTDIR "../src/c_api/"

using namespace std;

struct Method
{
	string mRetType;
	string mFuncName;
	vector<string> mParmType;
	vector<string> mParmName;
	vector<string> mParmValue;
	vector<int> mRef;
};

struct Enum
{
	string mName;
	string mValue;
};

struct Class
{
	string mName;
	string mParent;
	vector<Method *> mMethod;
	vector<Enum *> mEnum;
};

vector<Class *>gClass;

char * loadfile(const char *aFilename)
{
	FILE * f;
	f = fopen(aFilename, "rb");
	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	fseek(f, 0, SEEK_SET);
	char * buf = new char[len+2];
	buf[len] = '\n';
	buf[len+1] = 0;
	fread(buf,1,len,f);
	fclose(f);
	return buf;
}

int is_whitespace(char c)
{
	if (c == ' ') return 1;
	if (c == '\t') return 1;
//	if (c == '\n') return 1;
	if (c == '\r') return 1;
	return 0;
}

int is_alphanumeric(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c >= 'A' && c <= 'Z')
		return 1;
	if (c == '_')
		return 1;
	return 0;
}

string token(char * buf, int &ofs)
{
	string s = "";

	while (is_whitespace(buf[ofs])) ofs++;
	
	if (is_alphanumeric(buf[ofs]))
	{
		while (is_alphanumeric(buf[ofs]))
		{
			s += buf[ofs];
			ofs++;
		}
	}
	else
	{
		s += buf[ofs];
		ofs++;
		if ((buf[ofs-1] == '/' && buf[ofs] == '/') ||
			(buf[ofs-1] == '/' && buf[ofs] == '*') ||
			(buf[ofs-1] == '*' && buf[ofs] == '/') ||
			(buf[ofs-1] == '=' && buf[ofs] == '=') ||
			(buf[ofs-1] == '!' && buf[ofs] == '=') ||
			(buf[ofs-1] == '<' && buf[ofs] == '=') ||
			(buf[ofs-1] == '>' && buf[ofs] == '=') ||
			(buf[ofs-1] == '-' && buf[ofs] == '=') ||
			(buf[ofs-1] == '+' && buf[ofs] == '=') ||
			(buf[ofs-1] == '+' && buf[ofs] == '+') ||
			(buf[ofs-1] == '-' && buf[ofs] == '-') ||
			(buf[ofs-1] == '/' && buf[ofs] == '=') ||
			(buf[ofs-1] == '*' && buf[ofs] == '=') ||
			(buf[ofs-1] == '%' && buf[ofs] == '='))
		{
			s += buf[ofs];
			ofs++;
		}		
	}
	return s;
}

#if 0
#define NEXTTOKEN { s = token(b, ofs); printf("%s ", s.c_str()); }
#define EXPECT(x) { string t = token(b, ofs); if (t != x) { printf("\n\nOops, expected \"%s\", not \"%s\", line %d\n", x, t.c_str(), __LINE__); PARSEERROR } }
#else
#define NEXTTOKEN { s = token(b, ofs);  }
#define EXPECT(x) if (token(b, ofs) != x) { PARSEERROR }
#endif
#define PARSEERROR { printf("Parse error near \"%s\", parser line %d\n", s.c_str(), __LINE__); exit(0); }
#define IGNORE token(b, ofs);
#define ALLOW(x) { int tofs = ofs; if (token(b, tofs) == x) { NEXTTOKEN; } }

void parse_params(Method *m, char *b, int &ofs)
{
	string s;
	NEXTTOKEN;
	while (s != ")")
	{
		int ref = 0;
		string pt = "";
		if (s == "const")
		{
			pt = s;
			NEXTTOKEN;
		}
		if (s == "unsigned")
		{
			if (pt != "") pt += " ";
			pt += s;
			NEXTTOKEN;
		}
		
		if (pt != "") pt += " ";
		pt += s;
		NEXTTOKEN;
		if (s == ":")
		{
			EXPECT(":");
			NEXTTOKEN;
			pt = s;
			NEXTTOKEN;
		}

		if (s == "*")
		{
			pt += " *";
			NEXTTOKEN;
		}
		if (s == "&")
		{
			pt += " *";
			ref = 1;
			NEXTTOKEN;
		}
		string pn = s;
		NEXTTOKEN;
		string pv = "";
		if (s == "=")
		{
			NEXTTOKEN;
			while (s != "," && s != ")")
			{
				pv += s;
				NEXTTOKEN;
			}
		}
		if (s == ",") NEXTTOKEN;

		m->mParmName.push_back(pn);
		m->mParmType.push_back(pt);
		m->mParmValue.push_back(pv);
		m->mRef.push_back(ref);
	}
}

void parse(const char *aFilename, int aPrintProgress = 0)
{
	printf("Parsing %s..\n", aFilename);
	char *b = loadfile(aFilename);
	int ofs = 0;
	int newline = 0;
	Class *c = NULL;
	string s;
	int omit = 0;
	while (b[ofs])
	{
		NEXTTOKEN;
		if (s == "struct")
		{
			// skip helper structs
			NEXTTOKEN;
			ALLOW("\n");
			NEXTTOKEN;
			if (s == "{")
			{
				while (s != "}") NEXTTOKEN;
				EXPECT(";");
			}
			else
			if (s == ";")
			{
				// okay
			}
			else
			{
				PARSEERROR;
			}
		}
		else
		if (s == "}")
		{
			ALLOW(";");
			if (c->mName == "Soloud")
				omit = !omit;
			else
				omit = 0;					
		}
		else
		if (s == "#" && newline)
		{
			while (token(b,ofs) != "\n") {}
			newline = 1;
		}
		else
		if (s == "//")
		{
			while (token(b,ofs) != "\n") {}
			newline = 1;
		}
		else
		if (s == "/*")
		{
			while (token(b,ofs) != "*/") {}
			newline = 0;
		}
		else
		if (s == "\"")
		{
			while (token(b,ofs) != "\"") {}
		}
		else
		if (s == "\n")
		{
			newline = 1;
		}
		else
		{
			if (s == "typedef")
			{
				// skip typedefs..
				while (s != ";") NEXTTOKEN;
			}
			else
			if (s == "namespace")
			{
				NEXTTOKEN;
				// Okay, kludge time: let's call thread functions a class, even though they're not, so we can ignore it
				if (s == "Thread")
				{
					c = new Class;
					c->mName = "Instance";
					c->mParent = "";
				}
				ALLOW("\n");
				EXPECT("{");
			}
			else
			if (s == "class")
			{
				string classname;
				string parentname = "";
				NEXTTOKEN;
				classname = s;
				ALLOW("\n");
				NEXTTOKEN;
				if (s == ":")
				{
					EXPECT("public");
					NEXTTOKEN;
					parentname = s;
					ALLOW("\n");
					NEXTTOKEN;
				}

				if (s == "{") 
				{
					if (c) 	gClass.push_back(c);
					c = new Class;
					c->mName = classname;
					c->mParent = parentname;
					omit = 1;
				}
			}
			else
			{
				if (s == "enum")
				{
					int enumvalue = 0;
					NEXTTOKEN; // skip enum name
					ALLOW("\n");
					EXPECT("{");
					ALLOW("\n");
					NEXTTOKEN;
					while (s != "}")
					{
						if (s == "//")
						{
							while (s != "\n") NEXTTOKEN;
							NEXTTOKEN;
						}
						else
						{
							Enum *e = new Enum;
							e->mName = s;
							NEXTTOKEN;
							if (s == "=")
							{
								NEXTTOKEN;
								e->mValue = s;
								enumvalue = atoi(s.c_str()) + 1;
							}
							else
							{
								if (s == "," || s == "\n")
								{
									char temp[256];
									sprintf(temp, "%d", enumvalue);
									enumvalue++;
									e->mValue = temp;
								}
							}
							ALLOW(",");
							ALLOW("\n");
							NEXTTOKEN;	
							c->mEnum.push_back(e);
						}
					}
					EXPECT(";");
				}
				else
				if (s == "~")
				{
					// non-virtual DTor
					EXPECT(c->mName);
					EXPECT("(");
					EXPECT(")");
					EXPECT(";");
				}
				else
				if (c && s == c->mName)
				{
					// CTor
					Method *m = new Method;
					m->mFuncName = c->mName;
					m->mRetType = c->mName;					
					EXPECT("(");
					parse_params(m, b, ofs);
					c->mMethod.push_back(m);
					EXPECT(";");
					
				}
				else
				if (s == "public")
				{
					EXPECT(":");
					if (c->mName == "Soloud")
						omit = !omit;
					else
						omit = 0;					
				}
				else
				{
					// possibly function
					string vt1 = s;

					if (s == "const")
					{
						NEXTTOKEN;
						vt1 += " " + s;
					}

					if (s == "virtual")
					{
						NEXTTOKEN;
						vt1 = s;
					}
					if (s == "~")
					{
						// virtual dtor
						EXPECT(c->mName);
						EXPECT("(");
						EXPECT(")");
						ALLOW("const");
						EXPECT(";");
						continue;
					}


					if (s == "unsigned")
					{
						NEXTTOKEN;
						vt1 += " " + s;
					}
					NEXTTOKEN;
					if (s == "*")
					{
						vt1 += " *";
						NEXTTOKEN;
					}
					if (s == "*")
					{
						vt1 += " *";
						NEXTTOKEN;
					}

					string vt2 = s;
					NEXTTOKEN;

					if (s == "(")
					{
						// function
						Method *m = new Method;
						m->mFuncName = vt2;
						m->mRetType = vt1;
						parse_params(m, b, ofs);
						if (!omit)
						{
							c->mMethod.push_back(m);
						}
						else
						{
							// Should clean up m or we'll leak it
							// but what's a little memory leakage between friends?
						}
#ifdef PRINT_FUNCTIONS
						printf("%s %s(", m->mRetType.c_str(), m->mFuncName.c_str());
						int i;
						for (i = 0; i < (signed)m->mParmName.size(); i++)
						{
							if (i != 0)
								printf(", ");
							printf("%s %s", m->mParmType[i].c_str(), m->mParmName[i].c_str());
							if (m->mParmValue[i] != "")
								printf("= %s", m->mParmValue[i].c_str());
						}
						printf(")\n");
#endif
						ALLOW("const");
						ALLOW("=");
						ALLOW("0");
						EXPECT(";");
					}
					else
					{
						// alas, not a function.

						// May be an array or list so let's deal with that
						if (s == "[" || s == ",")
						{
							while (s != ";") NEXTTOKEN;
						}

						if (s != ";") PARSEERROR;
					}
				}
			}
			newline = 0;
		}
	}
	gClass.push_back(c);
}

void fileheader(FILE * f)
{
	fprintf(f, 
		"/* **************************************************\n"
		" *  WARNING: this is a generated file. Do not edit. *\n"
		" *  Any edits will be overwritten by the generator. *\n"
		" ************************************************** */\n"
		"\n"
		"/*\n"
		"SoLoud audio engine\n"
		"Copyright (c) 2013-2014 Jari Komppa\n"
		"\n"
		"This software is provided 'as-is', without any express or implied\n"
		"warranty. In no event will the authors be held liable for any damages\n"
		"arising from the use of this software.\n"
		"\n"
		"Permission is granted to anyone to use this software for any purpose,\n"
		"including commercial applications, and to alter it and redistribute it\n"
		"freely, subject to the following restrictions:\n"
		"\n"
		"   1. The origin of this software must not be misrepresented; you must not\n"
		"   claim that you wrote the original software. If you use this software\n"
		"   in a product, an acknowledgment in the product documentation would be\n"
		"   appreciated but is not required.\n"
		"\n"
		"   2. Altered source versions must be plainly marked as such, and must not be\n"
		"   misrepresented as being the original software.\n"
		"\n"
		"   3. This notice may not be removed or altered from any source\n"
		"   distribution.\n"
		"*/\n"
		"\n"
		"/* " VERSION " */\n"
		"\n"
		);
}

void emit_cppstart(FILE * f)
{
	fprintf(f,		
		"#include \"../include/soloud.h\"\n"
		"#include \"../include/soloud_audiosource.h\"\n"
		"#include \"../include/soloud_biquadresonantfilter.h\"\n"
		"#include \"../include/soloud_lofifilter.h\"\n"
		"#include \"../include/soloud_bus.h\"\n"
		"#include \"../include/soloud_echofilter.h\"\n"
		"#include \"../include/soloud_fader.h\"\n"
		"#include \"../include/soloud_fftfilter.h\"\n"
		"#include \"../include/soloud_filter.h\"\n"
		"#include \"../include/soloud_speech.h\"\n"
		"#include \"../include/soloud_thread.h\"\n"
		"#include \"../include/soloud_wav.h\"\n"
		"#include \"../include/soloud_wavstream.h\"\n"
		"#include \"../include/soloud_sfxr.h\"\n"
#if defined(WITH_MODPLUG)
		"#include \"../include/soloud_modplug.h\"\n"
#endif

		"\n"
		"using namespace SoLoud;\n"
		"\n"
		"extern \"C\"\n"
		"{\n\n"
	);
}

void emit_ctor(FILE * f, const char * cl)
{
	fprintf(f,
		"void * %s_create()\n"
		"{\n"
		"  return (void *)new %s;\n"
		"}\n"
		"\n", cl, cl);
}

void emit_dtor(FILE * f, const char * cl)
{
	fprintf(f,
		"void %s_destroy(void * aClassPtr)\n"
		"{\n"
		"  delete (%s *)aClassPtr;\n"
		"}\n"
		"\n", cl, cl);
}

void emit_cppend(FILE * f)
{
	fprintf(f,
		"} // extern \"C\"\n"
		"\n");
}

void emit_func(FILE * f, int aClass, int aMethod)
{
	int i;
	int initfunc = 0;
	Class *c = gClass[aClass];
	Method *m = c->mMethod[aMethod];

	if (c->mName == "Soloud" && m->mFuncName.find("_init") != string::npos)
	{
		// Init function, needs "a bit" of special handling.
		initfunc = 1;
		string fn = OUTDIR "soloud_c_" + m->mFuncName.substr(0, m->mFuncName.find_first_of('_')) + ".cpp";
		f = fopen(fn.c_str(), "w");
		fileheader(f);
		emit_cppstart(f);
	}

	if (initfunc)
	{
		fprintf(f, 
			"%s %s_%s(", 
			m->mRetType.c_str(), 
			c->mName.c_str(), 
			m->mFuncName.c_str());
	}
	else
	{
		fprintf(f, 
			"%s %s_%s(void * aClassPtr", 
			m->mRetType.c_str(), 
			c->mName.c_str(), 
			m->mFuncName.c_str());
	}

	int had_defaults = 0;
	for (i = 0; i < (signed)m->mParmName.size(); i++)
	{
		if (m->mParmValue[i] == "")
		{
			if (!(i == 0 && initfunc))
				fprintf(f, ", ");
			fprintf(f, 
				"%s %s",
				m->mParmType[i].c_str(),
				m->mParmName[i].c_str());
		}
		else
		{
			had_defaults = 1;
		}
	}
	
	if (initfunc)
	{
		fprintf(f, 
			")\n"
			"{\n"
			"\t%s%s(",
			(m->mRetType == "void")?"":"return ",
			m->mFuncName.c_str());
	}
	else
	{
		fprintf(f, 
			")\n"
			"{\n"
			"\t%s * cl = (%s *)aClassPtr;\n"
			"\t%scl->%s(",
			c->mName.c_str(),
			c->mName.c_str(),
			(m->mRetType == "void")?"":"return ",
			m->mFuncName.c_str());
	}

	for (i = 0; i < (signed)m->mParmName.size(); i++)
	{
		if (m->mParmValue[i] == "")
		{
			if (i != 0)
				fprintf(f, ", ");
			if (m->mRef[i])
				fprintf(f, "*");
			fprintf(f, 
				"%s",				
				m->mParmName[i].c_str());
		}
	}
	fprintf(f,
		");\n"
		"}\n"
		"\n");

	if (had_defaults)
	{
		if (initfunc)
		{
			fprintf(f, 
				"%s %s_%sEx(", 
				m->mRetType.c_str(), 
				c->mName.c_str(), 
				m->mFuncName.c_str());
		}
		else
		{
			fprintf(f, 
				"%s %s_%sEx(void * aClassPtr", 
				m->mRetType.c_str(), 
				c->mName.c_str(), 
				m->mFuncName.c_str());
		}
		for (i =0; i < (signed)m->mParmName.size(); i++)
		{
			if (!(i == 0 && initfunc))
				fprintf(f, ", ");
			fprintf(f, 
				"%s %s",
				m->mParmType[i].c_str(),
				m->mParmName[i].c_str());
		}
		
		if (initfunc)
		{
			fprintf(f, 
				")\n"
				"{\n"
				"\t%s%s(",
				(m->mRetType == "void")?"":"return ",
				m->mFuncName.c_str());
		}
		else
		{
			fprintf(f, 
				")\n"
				"{\n"
				"\t%s * cl = (%s *)aClassPtr;\n"
				"\t%scl->%s(",
				c->mName.c_str(),
				c->mName.c_str(),
				(m->mRetType == "void")?"":"return ",
				m->mFuncName.c_str());
		}

		for (i = 0; i < (signed)m->mParmName.size(); i++)
		{
			if (i != 0)
				fprintf(f, ", ");
			if (m->mRef[i])
				fprintf(f, "*");
			fprintf(f, 
				"%s",				
				m->mParmName[i].c_str());
		}
		fprintf(f,
			");\n"
			"}\n"
			"\n");
	}

	if (initfunc)
	{
		emit_cppend(f);
		fclose(f);
	}
}


void generate()
{
	FILE * f, *cppf, *deff;
	f = fopen("../include/soloud_c.h", "w");
	cppf = fopen(OUTDIR "soloud_c.cpp", "w");
	deff = fopen(OUTDIR "soloud.def", "w");
	fileheader(f);
	fileheader(cppf);

	fprintf(deff,
//		"LIBRARY soloud\n"
		"EXPORTS\n");

	emit_cppstart(cppf);

	fprintf(f,
		"#ifndef SOLOUD_C_H_INCLUDED\n"
		"#define SOLOUD_C_H_INCLUDED\n"
		"\n"
		"#ifdef  __cplusplus\n"
		"extern \"C\" {\n"
		"#endif\n");


	int i, j, k;
	int first = 1;
	fprintf(f, 
		"// Collected enumerations\n"
		"enum SOLOUD_ENUMS\n"
		"{\n");
	for (i = 0; i < (signed)gClass.size(); i++)
	{
		if (gClass[i]->mName.find("Instance") == string::npos &&
			gClass[i]->mName != "Filter" &&
			gClass[i]->mName != "AudioSource")
		{
			string cn = gClass[i]->mName;
		
			int j;

			for (j = 0; j < (signed)cn.length(); j++)
				cn[j] = toupper(cn[j]);

			for (j = 0; j < (signed)gClass[i]->mEnum.size(); j++)
			{
				if (!first)
				{
					fprintf(f, ",\n");				
				}
				else
				{
					first = 0;
				}
				fprintf(f, "\t%s_%s = %s", cn.c_str(), gClass[i]->mEnum[j]->mName.c_str(), gClass[i]->mEnum[j]->mValue.c_str());
			}
		}
	}
	fprintf(f,
		"\n"
		"};\n"
		"\n");

	fprintf(f, "// Object handle typedefs\n");
	for (i = 0; i < (signed)gClass.size(); i++)
	{
		if (gClass[i]->mName.find("Instance") == string::npos)// &&
			//gClass[i]->mName != "Filter" &&
			//gClass[i]->mName != "AudioSource")
		{
			fprintf(f, "typedef void * %s;\n", gClass[i]->mName.c_str());
		}
	}

	for (i = 0; i < (signed)gClass.size(); i++)
	{
		if (gClass[i]->mName.find("Instance") == string::npos &&
			gClass[i]->mName != "Filter" &&
			gClass[i]->mName != "AudioSource" &&
			gClass[i]->mName != "Fader")
		{
			fprintf(f,
				"\n"
				"/*\n"
				" * %s\n"
				" */\n",
				gClass[i]->mName.c_str());
			fprintf(f, "void %s_destroy(%s * a%s);\n", gClass[i]->mName.c_str(), gClass[i]->mName.c_str(), gClass[i]->mName.c_str());
			fprintf(deff, "\t%s_destroy\n", gClass[i]->mName.c_str());
			emit_dtor(cppf, gClass[i]->mName.c_str());
			
			for (j = 0; j < (signed)gClass[i]->mMethod.size(); j++)
			{
				if (gClass[i]->mMethod[j]->mFuncName.find("Instance") == string::npos &&
					gClass[i]->mMethod[j]->mFuncName.find("interlace") == string::npos) 
				{
					if (gClass[i]->mName == gClass[i]->mMethod[j]->mRetType)
					{
						// CTor
						fprintf(f, "%s * %s_create();\n", gClass[i]->mName.c_str(), gClass[i]->mName.c_str());
						fprintf(deff, "\t%s_create\n", gClass[i]->mName.c_str());
						// TODO: ctors with params? none in soloud so far..
						emit_ctor(cppf, gClass[i]->mName.c_str());
					}
					else
					{
						emit_func(cppf, i, j);
						int has_defaults;
						has_defaults = 0;
						fprintf(f, 
							"%s %s_%s(%s * a%s",
							gClass[i]->mMethod[j]->mRetType.c_str(), 
							gClass[i]->mName.c_str(),
							gClass[i]->mMethod[j]->mFuncName.c_str(),
							gClass[i]->mName.c_str(),
							gClass[i]->mName.c_str());
						fprintf(deff, "\t%s_%s\n", gClass[i]->mName.c_str(), gClass[i]->mMethod[j]->mFuncName.c_str());


						for (k = 0; k < (signed)gClass[i]->mMethod[j]->mParmName.size(); k++)
						{
							if (gClass[i]->mMethod[j]->mParmValue[k] == "")
							{
								if (gClass[i]->mMethod[j]->mParmType[k] != "Soloud *")
								{
									fprintf(f, ", %s %s", 
										gClass[i]->mMethod[j]->mParmType[k].c_str(),
										gClass[i]->mMethod[j]->mParmName[k].c_str());
								}
							}
							else
							{
								has_defaults = 1;
							}
						}
						fprintf(f, ");\n");
						if (has_defaults)
						{
							fprintf(f, 
								"%s %s_%sEx(%s * a%s",
								gClass[i]->mMethod[j]->mRetType.c_str(), 
								gClass[i]->mName.c_str(),
								gClass[i]->mMethod[j]->mFuncName.c_str(),
								gClass[i]->mName.c_str(),
								gClass[i]->mName.c_str());
							fprintf(deff, "\t%s_%sEx\n", gClass[i]->mName.c_str(), gClass[i]->mMethod[j]->mFuncName.c_str());
							int had_defaults = 0;
							for (k = 0; k < (signed)gClass[i]->mMethod[j]->mParmName.size(); k++)
							{
								if (gClass[i]->mMethod[j]->mParmType[k] != "Soloud *")
								{
									fprintf(f, ", %s %s", 
										gClass[i]->mMethod[j]->mParmType[k].c_str(),
										gClass[i]->mMethod[j]->mParmName[k].c_str());
									if (gClass[i]->mMethod[j]->mParmValue[k] != "")
									{
										fprintf(f, " /* = %s */", gClass[i]->mMethod[j]->mParmValue[k].c_str());
										had_defaults = 1;
									}
								}
							}
							fprintf(f, ");\n");
						}
					}
				}
			}
		}
	}

	fprintf(f,
		"#ifdef  __cplusplus\n"
		"} // extern \"C\"\n"
		"#endif\n"
		"\n"
		"#endif // SOLOUD_C_H_INCLUDED\n"
		"\n");

	emit_cppend(cppf);

	fclose(f);
	fclose(cppf);
	fclose(deff);
}

void generate_cpp()
{
	FILE * f;
	f = fopen("soloud_c.cpp", "w");
	fileheader(f);

	fclose(f);
}

void inherit_stuff()
{
	int i, j, k, l;

	for (i = 0; i < (signed)gClass.size(); i++)
	{
		if (gClass[i]->mParent != "")
		{
			for (j = 0; j < (signed)gClass.size(); j++)
			{
				if (gClass[j]->mName == gClass[i]->mParent)
				{
					// Only handle single level of inheritance for now. If more are needed,
					// this needs to be recursive.
					for (k = 0; k < (signed)gClass[j]->mMethod.size(); k++)
					{
						if (gClass[j]->mMethod[k]->mFuncName != gClass[j]->mName)
						{
							int found = 0;
							for (l = 0; !found && l < (signed)gClass[i]->mMethod.size(); l++)
							{
								if (gClass[i]->mMethod[l]->mFuncName == gClass[j]->mMethod[k]->mFuncName)
								{
									found = 1;
								}
							}

							if (!found)
							{
								gClass[i]->mMethod.push_back(gClass[j]->mMethod[k]);
							}
						}
					}
				}
			}
		}
	}
}

int main(int parc, char ** pars)
{
	printf(VERSION "\n");
	
	if (parc < 2 || _stricmp(pars[1], "go") != 0)
	{
		printf("\nThis program will generate the 'C' api wrapper code.\n"
			   "You probably ran this by mistake.\n"
			   "Use parameter 'go' to actually do something.\n"
			   "\n"			   
			   "Note that output will be ../include/soloud_c.h and " OUTDIR "soloud_c*.cpp.\n"
			   "\n");
		return 0;
	}

	parse("../include/soloud.h");
	parse("../include/soloud_audiosource.h");
	parse("../include/soloud_biquadresonantfilter.h");
	parse("../include/soloud_lofifilter.h");
	parse("../include/soloud_bus.h");
	parse("../include/soloud_echofilter.h");
	parse("../include/soloud_fader.h");
	parse("../include/soloud_fftfilter.h");
	parse("../include/soloud_filter.h");
	parse("../include/soloud_speech.h");
	parse("../include/soloud_thread.h");
	parse("../include/soloud_wav.h");
	parse("../include/soloud_wavstream.h");
	parse("../include/soloud_sfxr.h");
#if defined(WITH_MODPLUG)
	parse("../include/soloud_modplug.h");
#endif
	printf("Handling inheritance..\n");
	inherit_stuff();

	printf("Generating header and cpp..\n");
	generate();

	printf("All done.\n");
}


