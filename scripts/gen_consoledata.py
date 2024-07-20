#!/usr/bin/env python3
""" SoLoud console data generator """

import soloud_codegen

banlist = [
    "Soloud_getSpeakerPosition",
    "Soloud_calcFFT",
    "Soloud_getWave",
    "Bus_calcFFT",
    "Bus_getWave",
    "Soloud_getErrorString",
    "Soloud_getBackendString",
    "BassboostFilter_getParamName",
    "BiquadResonantFilter_getParamName",
    "DCRemovalFilter_getParamName",
    "EchoFilter_getParamName",
    "FFTFilter_getParamName",
    "FlangerFilter_getParamName",
    "FreeverbFilter_getParamName",
    "LofiFilter_getParamName",
    "RobotizeFilter_getParamName",
    "WaveShaperFilter_getParamName",

]

fo = open("../demos/console/generated.inc", "w")
fo.write("// This is a generated file (via gen_consoledata.py). Do not edit.\n\n")
def cify(x):
    s = ""
    s += x[0] + " " + x[1] + "(" + " \\n    "
    for y in x[2]:
        if len(y) > 0:
            s += y[0] + " "+ y[1] + ",\\n    "
    s = s[:-7] + ")"
    return s

def parcount(x):
    if len(x) != 1:
        return len(x)
    if len(x[0]) != 0:
        return 1
    return 0

for obj in soloud_codegen.soloud_type:
    for func in soloud_codegen.soloud_func:
        if func[1] in banlist:
            continue
        if (obj + "_") == func[1][0:len(obj)+1:]:
            fo.write(f'void call_{func[1]}()\n')
            fo.write(f'{{\n')
            fo.write(f'  if (gCmd.size() != {parcount(func[2]) + 3})')
            fo.write(f'  {{\n')
            fo.write(f'    printf("Invalid number of parameters ({parcount(func[2])} expected)\\n");\n')
            fo.write(f'    return;\n')
            fo.write(f'  }}\n')

            i = 3
            for param in func[2]:
                if len(param) > 0:
                    if 'char *' in param[0]:
                        pass
                    elif '*' in param[0]:                    
                        fo.write(f'  if (gVar.count(gCmd[{i}]) == 0)\n')
                        fo.write(f'  {{\n')
                        fo.write(f'    printf("Variable \\"%s\\" not found\\n", gCmd[{i}].c_str());\n')
                        fo.write(f'    return;\n')
                        fo.write(f'  }}\n')
                i += 1

            fo.write(f'  ')
            if func[0] != 'void':
                fo.write(f'{func[0]} res = ({func[0]})')
            fo.write(f'{func[1]}(')
            i = 3
            for param in func[2]:
                if len(param) > 0:
                    if 'char *' in param[0]:
                        fo.write(f'({param[0]})gCmd[{i}].c_str()')
                    elif '*' in param[0]:                    
                        fo.write(f'({param[0]})gVar[gCmd[{i}]].p')
                    elif 'int' in param[0] or 'char' in param[0]:
                        fo.write(f'({param[0]})getVarOrInt(gCmd[{i}])')
                    elif 'double' in param[0] or 'float' in param[0]:
                        fo.write(f'({param[0]})getVarOrFloat(gCmd[{i}])')
                    else:
                        fo.write("XXXXXXXXXX")
                    if i < parcount(func[2]) + 2:
                        fo.write(", ")
                    i += 1
            fo.write(f');\n')
            if func[0] != 'void':
                fo.write(f'  gVar[gCmd[0].c_str()]')
                if 'int' in func[0] or 'char' in func[0]:
                    fo.write(f'.i = (int)')
                elif 'float' in func[0] or 'double' in func[0]:
                    fo.write(f'.f = (int)')
                elif '*' in func[0]:
                    fo.write(f'.p = (void *)')
                else:
                    fo.write("xxxxxxxxxx")

                fo.write(f'res;\n')

            #fo.write(f'  printf("Hello from {func}\\n");\n')
            fo.write(f'}}\n')
            fo.write(f'\n')

fo.write("void setup_soloud_calls()\n{\n")
for obj in soloud_codegen.soloud_type:
    for func in soloud_codegen.soloud_func:
        if func[1] in banlist:
            continue
        if (obj + "_") == func[1][0:len(obj)+1:]:
            fo.write(f'  gSoloudCalls["{obj}"]["{func[1][len(obj)+1:]}"].p = call_{func[1]};\n')
            fo.write(f'  gSoloudCalls["{obj}"]["{func[1][len(obj)+1:]}"].info = "{cify(func)}";\n')

fo.write("}\n")



print("generated.inc generated")

fo.close()
