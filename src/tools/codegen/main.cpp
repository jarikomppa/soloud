#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <string>

#define VERSION "SoLoud C-Api Code Generator (c)2013 Jari Komppa http://iki.fi/sol/"

using namespace std;

struct Method
{
	string mRetType;
	string mFuncName;
	vector<string> mParmType;
	vector<string> mParmName;
	vector<string> mParmValue;
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

		if (s == "*")
		{
			pt += " *";
			NEXTTOKEN;
		}
		if (s == "&")
		{
			pt += " &";
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

//		printf("\npn:\"%s\"\npt:\"%s\"\npv:\"%s\"\n", pn.c_str(), pt.c_str(), pv.c_str());
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
				// Okay, kludge time: let's call thread functions a class, even though they're not.
				if (s == "Thread")
				{
					c = new Class;
					c->mName = s;
					c->mParent = s;
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
				}
				else
				{
					// possibly function
					string vt1 = s;
					if (s == "virtual")
					{
						NEXTTOKEN;
						vt1 += " " + s;
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
						c->mMethod.push_back(m);
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

void print_enums()
{
	int i;
	for (i = 0; i < (signed)gClass.size(); i++)
	{
		string cn = gClass[i]->mName;
		
		int j;

		for (j = 0; j < (signed)cn.length(); j++)
			cn[j] = toupper(cn[j]);

		for (j = 0; j < (signed)gClass[i]->mEnum.size(); j++)
		{
			printf("%s_%s = %s\n", cn.c_str(), gClass[i]->mEnum[j]->mName.c_str(), gClass[i]->mEnum[j]->mValue.c_str());
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
			   "Use parameter 'go' to actually do something.\n\n");
		return 0;
	}

	parse("../include/soloud.h");
	parse("../include/soloud_audiosource.h");
	parse("../include/soloud_biquadresonantfilter.h");
	parse("../include/soloud_bus.h");
	parse("../include/soloud_echofilter.h");
	parse("../include/soloud_fader.h");
	parse("../include/soloud_fftfilter.h");
	parse("../include/soloud_filter.h");
	parse("../include/soloud_speech.h");
	parse("../include/soloud_thread.h");
	parse("../include/soloud_wav.h");
	parse("../include/soloud_wavstream.h");

}


