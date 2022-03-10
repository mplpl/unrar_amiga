/* lng2ct.rexx <rar.lng >deutsch.ct */
do while ~eof(stdin)
	pull line
	if line=":STRINGS" then leave
end
say "## version $VER: unrar.catalog 6.02 ("translate(date(e),'.','/')")"
say "## codeset 0"
say "## language deutsch"
say "## chunk AUTH Michael Leimer"
say ";"
say "MSG_MAmigaPortBy"
say "\n%s-Port von Marcin Labenski\n"
say "; \n%s port by Marcin Labenski\n"
say ";"
say "MSG_MAmigaConvErr"
say "WARNUNG: Einige Zeichen, die in Dateinamen in diesem Archiv verwendet werden, wurden nicht konvertiert, da sie in der %s-Kodierung nicht verfügbar sind. Setzen Sie die Umgebungsvariable RAR_CODEPAGE, um die richtige Kodierung auszuwählen."
say "; WARNING: Some characters used in file names in this archive have not been converted because they are not available in %s encoding. Set RAR_CODEPAGE environment variable to select the right encoding."
say ";"
say "MSG_MAmigaConvInitErr"
say "WARNUNG: %s-Kodierung wird nicht unterstützt. Wählen Sie eine andere, indem Sie die Umgebungsvariable RAR_CODEPAGE setzen."
say "; WARNING: %s encoding is not supported. Select a different one by setting RAR_CODEPAGE environment variable."
do while ~eof(stdin)
	parse pull "M"key translation
	if key>"" then do
		say ";"
		say "MSG_M"key
		say strip(strip(strip(strip(strip(translation),,"09"x)),,"09"x),,'"')
		say "; "key
	end
end
