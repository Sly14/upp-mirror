@ZAZNAM "Soci�ln� z�znam" _
    "Soci�ln� z�znam: <<PRIJETI>>, <<Osoby DITE>>, otec: <<Osoby OTEC>>, matka: <<Osoby MATKA>>"
    ^
;
osoby;
update;
PRIJETI    "Datum p�ijet� do p��e"   date today;
/*
OTEC_ZAM   "Zam�stn�n� otce"         text +;
MATKA_ZAM  "Zam�stn�n� matky"        text;
OTEC_AZAM  "  adresa zam�stmavatele" text +;
MATKA_AZAM "  adresa zam�stmavatele" text;
OTEC_DUCH  "  po�ivatel d�chodu"     list ##ANONE = N +;
MATKA_DUCH "  po�ivatel d�chodu"     list ##ANONE = N;
OTEC_ZPS   "  ZPS" list ##ZPS = NE +;
MATKA_ZPS  "  ZPS" list ##ZPS = NE;
*/

@UKON "Z�znam o �konu" _
	^
	^
;

@ZAPIS "Jin� z�pis" _
	"<<POPIS>>"
;
POPIS "Popis z�pisu" text;

@NAVRH "N�vrh" _
	"N�vrh na <<POPIS>>"
	NAVRH "N�vrh"
;
deti;
ZNACKA     "Na�e zna�ka" text;
OTEC       "Otec"        osoba OTEC;
MATKA      "Matka"       osoba MATKA;
POPIS      "N�vrh na"    text;

@VYCHOVAVYZIVA     "Ur�en� v�chovy a v��ivy" VV ##STDNAVRH;
@NEPROVDANAMATKA   "Ur�en� v��ivn�ho pro neprovdanou matku" VV ##STDNAVRH;
@EXEKUCE           "Exekuce" VV ##STDNAVRH;
@URCENIVYZIVNEHO   "Ur�en� v��ivn�ho" VV ##STDNAVRH;
@SNIZENIVYZIVNEHO  "Sn�en� v��ivn�ho" VV ##STDNAVRH;
@VVROZVOD          "Ur�en� v�chovy a v��ivy po rozvodu" VV ##STDNAVRH;
@ZMENAVYCHOVY      "Zm�na v�chovy" VV ##STDNAVRH;
@ZRUSENIVYZIVNEHO  "Zru�en� v��ivn�ho" VV ##STDNAVRH;
@ZVYSENIVYZIVNEHO  "Zv��en� v��ivn�ho" VV ##STDNAVRH;

@POPRENIOTCOVSTVI  "Pop�en� otcovstv�" VZTAH ##STDNAVRH;
@URCENIOTCOVSTVI   "Ur�en� otcovstv�" VZTAH ##STDNAVRH;
@ZMENAPRIJMENI     "Zm�na p��jmen�" VZTAH ##STDNAVRH;

@MIMOSOUDNISTYK    "Mimosoudn� dohoda o styku" STYK ##STDNAVRH;
@NAVRHSTYK         "�pravu styku" STYK ##STDNAVRH;

@PROTOKOLOSVOJENI "Protokol o osvojeni" VZTAH
	^
	^
;
deti;
OSVOJITEL   "Osvojitel"   osoba;
MATKA       "Matka"       osoba MATKA;
OTEC        "Otec"        osoba OTEC;

@OSVOJENI "Osvojeni man�elem matky" VZTAH
    "N�vrh na osvojeni: <<Osoba DITE>>, otec: <<Osoba MATKA>>, navrhovatel: <<Osoba NAVRHOVATEL>>"
	^
;
deti;
NAVRHOVATEL "Navrhovatel" osoba;
MATKA       "Matka"       osoba MATKA;
OTEC        "Otec"        osoba OTEC;


@NARIZENIDOHLEDU   "Na��zen� dohledu" DOHLED ##STDNAVRH;
@POUSTAV           "PO a �stavn� p��e" DOHLED ##STDNAVRH;
@POZMENA           "PO a zm�na v�chovy" DOHLED ##STDNAVRH;
@PO                "PO" DOHLED ##STDNAVRH;
@NEMOCNICE         "Propu�t�n� do p��e matky" DOHLED ##STDNAVRH;

@REJSTRIK_SVERENYCH "Rejst��k d�t� sv��en�ch do v�chovy jin� fyzick� osob� ne� rodi�e" REJSTRIK
    "Rejst��k sv��en�ch d�t�: Datum p�ijet�: <<PRIJETI>>: <<Osoba DITE>>, odpov�dn� osoba: <<Osoba OSOBA>>"
;
rejstrik "{{1:1:1:2:1:1:2:1:2:2:1:1 "
         "Po�adov� ��slo:: Datum z�pisu:: Spisov� zna�ka OM"
         ":: Jm�no a p��jmen� d�t�te:: Rodn� ��slo:: Forma p��e"
         ":: Jm�no a p��jmen� odpov�dn� osoby (p��buzensk� vztah):: Rodn� ��slo odpov�dn� osoby"
         ":: M�sto trval�ho pobytu:: Ozna�en� org�nu&�.j. rozhodnut�&datum pr. moci"
         ":: Datum p�ijet� d�t�te do p��e:: Datum ukon�en� p��e",
         "<<PC>>:: <<DATUM>>:: <<SPIS>>:: <<Osoba DITE>>:: <<OsobaRC DITE>>:: <<FORMA>>:: <<Osoba OSOBA>> "
         ":: <<OsobaRC OSOBA>>:: <<Adresa ADRESA>>:: <<ORGAN>>&<<CJ>>&<<NPM>>:: <<PRIJETI>>:: <<UKONCENI>>"
		PRIJETI UKONCENI
;
DITE     "D�t�" osoba DITE;
FORMA    "Forma p��e" text;
OSOBA    "Odpov�dn� osoba" osoba;
ADRESA   " m�sto trval�ho pobytu" adresa auto;
VZTAH    "P��buzensk� vztah" text;
ORGAN    "Ozna�en� org�nu" text;
CJ       "�.j. rozhodnuti" text +;
NPM      "Datum pr�vn� moci" date;
PRIJETI  "Datum p�ijet� do p��e" date;
UKONCENI "Datum ukon�en� p��e" date;
