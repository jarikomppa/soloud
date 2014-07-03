""" builds documentation files from multimarkdown (mmd) source
    to various formats, including the web site and pdf.
"""

import subprocess
import glob
import os
import sys

src = [
    "intro.mmd", 
    "legal.mmd",
    "downloads.mmd",
    "quickstart.mmd",
    "premake.mmd",
    "concepts.mmd",
    "faq.mmd",
    "examples.mmd",
    "codegen.mmd",
    "c_api.mmd",
    "python_api.mmd",
    "ruby_api.mmd",
    "bmx_api.mmd",
    "basics.mmd",
    "attributes.mmd",
    "faders.mmd",
    "voicegroups.mmd", 
    "coremisc.mmd",
    "mixbus.mmd",
    "audiosource.mmd",
    "wav.mmd",
    "wavstream.mmd",
    "speech.mmd",
    "sfxr.mmd",
    "modplug.mmd",
    "newsoundsources.mmd",
    "filters.mmd",
    "biquadfilter.mmd",
    "echofilter.mmd",
    "fftfilter.mmd",
    "lofifilter.mmd",
    "flangerfilter.mmd",
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
	
print "- -- --- -- - Generating single-file HTML docs"

callp = ["pandoc", "-s", "--toc", "--default-image-extension=png", "-o", "soloud.html"]
for x in src:
	if x not in website_only:
		callp.append(x)
subprocess.call(callp)

print "- -- --- -- - Generating web site"
for x in src:
	subprocess.call(["pandoc", "--template=html.pandoc", "-B", "htmlpre.txt", "-A", "htmlpost.txt", "--default-image-extension=png", x, "-o", x[:len(x)-3]+"html.bak"])
	with open(x[:len(x)-3]+"html", "w") as file_out:
		with open(x[:len(x)-3]+"html.bak", "r") as file_in:
			for line in file_in:
				file_out.write(line.replace('code>', 'code>\n'))
	if x == "intro.mmd":
		os.remove("index.html")
		os.rename("intro.html", "index.html")

print "- -- --- -- - Generating epub"

callp = ["pandoc", "-N", "--toc", "--epub-cover-image=images/cover.png", "-t", "epub3", "--default-image-extension=png", "-S", "--epub-stylesheet=epub.css", "--epub-metadata=metadata.xml", "-o", "SoLoud.epub", "title.txt"]
for x in src:
	if x not in website_only:
		callp.append(x)
subprocess.call(callp)

print "- -- --- -- - Converting epub -> mobi (kindlegen_output.txt)"
with open('kindlegen_output.txt', 'w') as outfile:
    subprocess.call(["kindlegen", "SoLoud.epub", "-c2"], stdout=outfile)

print "- -- --- -- - Generating LaTex"

for x in src:
	if x not in website_only:
		subprocess.call(["pandoc", "--listings", "--default-image-extension=pdf", "--chapters", x, "-o", x[:len(x)-3]+"tex"])

print "- -- --- -- - Generating pdf (xelatex_output.txt)"

with open('xelatex_output.txt', 'w') as outfile:
    subprocess.call(["xelatex", "SoLoud.tex"], stdout=outfile)
    subprocess.call(["xelatex", "SoLoud.tex"], stdout=outfile)

print "- -- --- -- - Cleanup.."
tempsuffix = ["aux", "toc", "out", "log", "lg", "4ct", "4tc", "idv", "tmp", "xdv", "xref", "bak"]
for suffix in tempsuffix:
	for file in glob.glob("*."+suffix):
		os.remove(file)

print "- -- --- -- - Done"
