#include <TCtrlLib/TCtrlLib.h>
#pragma hdrstop

#ifndef flagIDERW
#define EXETITLE "iw"

//////////////////////////////////////////////////////////////////////

void AppMain()
{
	String cfg = ConfigFile();
	if(!LoadFromFile(Configuration::Main(), cfg))
		Exclamation("Konfigura�n� soubor [* \1" + cfg + "\1] je neplatn�.");

	void RunDlgImage();
	RunDlgImage();

	if(!StoreToFile(Configuration::Main(), cfg))
		Exclamation("Nelze aktualizovat konfigura�n� soubor [* \1" + cfg + "\1].");
}
#endif//flagIDERW

//////////////////////////////////////////////////////////////////////
