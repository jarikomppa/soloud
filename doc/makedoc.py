""" builds documentation files from multimarkdown (mmd) source
    to various formats, including the web site and pdf.
"""

import subprocess
import glob
import os
import sys
import time
import shutil

src = [
    "intro.mmd", 
    "downloads.mmd",
    "quickstart.mmd",
    "faq.mmd",
    "dirstruct.mmd",
    "premake.mmd",
    "legal.mmd",
    "concepts.mmd",
    "concepts3d.mmd",
    "voicemanagement.mmd",
    "examples.mmd",
    "foreign_interface.mmd",
    "c_api.mmd",
    "python_api.mmd",
    "ruby_api.mmd",
    "rpgmaker_api.mmd",
    "bmx_api.mmd",
    "gamemaker_api.mmd",
    "cs_api.mmd",
    "d_api.mmd",
    "codegen.mmd",
    "basics.mmd",
    "attributes.mmd",
    "faders.mmd",
    "voicegroups.mmd", 
    "coremisc.mmd",
    "core3d.mmd",
    "audiosource.mmd",
    "newsoundsources.mmd",
    "wav.mmd",
    "wavstream.mmd",
    "speech.mmd",
    "sfxr.mmd",
    "modplug.mmd",
    "monotone.mmd",
    "tedsid.mmd",
    "filters.mmd",
    "biquadfilter.mmd",
    "echofilter.mmd",
    "lofifilter.mmd",
    "flangerfilter.mmd",
    "dcremovalfilter.mmd",
    "fftfilter.mmd",
    "bassboostfilter.mmd",
    "mixbus.mmd",
    "collider.mmd",
    "attenuator.mmd",
    "file.mmd",
    "backends.mmd"
    ]

website_only = [
    "downloads.mmd"
    ]

unknown = 0
for file in glob.glob("*.mmd"):
    if file not in src:
        unknown = 1
        print file + " not included in docs!"
        
if unknown:
    print "Add the new files to makedoc.py, soloud.tex and htmlpre.txt."
    sys.exit()

datestring = time.strftime("%Y%m%d")
if not os.path.exists(datestring + "/web"):
  os.makedirs(datestring + "/web")
if not os.path.exists("temp/"):
  os.makedirs("temp/")
    
print "- -- --- -- - Generating single-file HTML docs"

callp = ["pandoc", "-s", "-t", "html5", "-H", "singlehtml_head.txt", "-B", "singlehtml_body.txt", "--toc", "--self-contained", "--default-image-extension=png", "-o", datestring + "/soloud_" + datestring + ".html"]
for x in src:
    if x not in website_only:
        callp.append(x)
subprocess.call(callp)

print "- -- --- -- - Generating web site"
for x in src:
    subprocess.call(["pandoc", "--template=html.pandoc", "-B", "htmlpre.txt", "-A", "htmlpost.txt", "--default-image-extension=png", x, "-o", datestring + "/web/" + x[:len(x)-3]+"html.bak"])
    with open(datestring + "/web/" + x[:len(x)-3]+"html", "w") as file_out:
        with open(datestring + "/web/" + x[:len(x)-3]+"html.bak", "r") as file_in:
            for line in file_in:
                file_out.write(line.replace('code>', 'code>\n').replace('::','::<wbr>').replace('\xc2','').replace('\xa0',''))
    if x == "intro.mmd":
        if os.path.isfile(datestring + "/web/index.html"):
            os.remove(datestring + "/web/index.html")
        os.rename(datestring + "/web/intro.html", datestring + "/web/index.html")

print "- -- --- -- - Generating epub"

callp = ["pandoc", "-N", "--toc", "--epub-cover-image=images/cover.png", "-t", "epub3", "--default-image-extension=png", "-S", "--epub-stylesheet=epub.css", "--epub-metadata=metadata.xml", "-o", datestring + "/soloud_" + datestring + ".epub", "title.txt"]
for x in src:
    if x not in website_only:
        callp.append(x)
subprocess.call(callp)

print "- -- --- -- - Converting epub -> mobi (kindlegen_output.txt)"
with open('kindlegen_output.txt', 'w') as outfile:
    subprocess.call(["kindlegen", datestring + "/soloud_" + datestring + ".epub", "-c2"], stdout=outfile)

print "- -- --- -- - Generating LaTex"

for x in src:
    if x not in website_only:
        subprocess.call(["pandoc", "-t", "latex", "--listings", "--default-image-extension=pdf", "--chapters", x, "-o", "temp/" + x[:len(x)-3]+"tex.orig"])
        with open("temp/" + x[:len(x)-3]+"tex", "w") as file_out:
            with open("temp/" + x[:len(x)-3]+"tex.orig", "r") as file_in:
                for line in file_in:
                    file_out.write(line.replace('\\begin{longtable}[c]{@{}ll@{}}', '\\begin{tabulary}{\\textwidth}{lJ}').replace('\\begin{longtable}[c]{@{}lll@{}}', '\\begin{tabulary}{\\textwidth}{lJJ}').replace('\\begin{longtable}[c]{@{}llll@{}}', '\\begin{tabulary}{\\textwidth}{lJJJ}').replace('\\endhead','').replace('\\end{longtable}','\\end{tabulary}'))

print "- -- --- -- - Generating pdf (xelatex_output.txt)"

with open('xelatex_output.txt', 'w') as outfile:
    subprocess.call(["xelatex", "SoLoud.tex"], stdout=outfile)
    print "- -- --- -- - Generating pdf pass 2.."
    subprocess.call(["xelatex", "SoLoud.tex"], stdout=outfile)

shutil.move("SoLoud.pdf", datestring + "/soloud_" + datestring + ".pdf")

print "- -- --- -- - Cleanup.."
tempsuffix = ["aux", "toc", "out", "log", "lg", "4ct", "4tc", "idv", "tmp", "xdv", "xref", "bak"]
for suffix in tempsuffix:
    for file in glob.glob("*."+suffix):
        os.remove(file)
    for file in glob.glob(datestring + "/web/*."+suffix):
        os.remove(file)
for file in glob.glob("temp/*"):
   os.remove(file)
os.rmdir("temp")

print "- -- --- -- - Done - " + datestring
