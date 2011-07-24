#include <CtrlLib/CtrlLib.h>
#include "Fonts.h"

NAMESPACE_UPP
const String fontDefTahoma_0 = "\
<?xml version=\"1.0\"?>\
<font>\
  <info face=\"Tahoma\" size=\"64\" bold=\"0\" italic=\"0\" charset=\"\" unicode=\"1\" stretchH=\"100\" smooth=\"0\" aa=\"1\" padding=\"64,64,64,64\" spacing=\"1,1\" outline=\"0\" />\
  <common lineHeight=\"64\" base=\"53\" scaleW=\"512\" scaleH=\"512\" pages=\"1\" packed=\"0\" alphaChnl=\"0\" redChnl=\"0\" greenChnl=\"0\" blueChnl=\"0\" />\
  <pages>\
    <page id=\"0\" file=\"tahoma.png\" />\
  </pages>\
  <chars count=\"95\">\
    <char id=\"32\" x=\"195,125\" y=\"380,5\" width=\"16,125\" height=\"16,125\" xoffset=\"-8\" yoffset=\"45\" xadvance=\"16,625\" page=\"0\" chnl=\"15\" />\
    <char id=\"33\" x=\"487,875\" y=\"172,75\" width=\"21,75\" height=\"54,5\" xoffset=\"-2,125\" yoffset=\"6,5\" xadvance=\"17,625\" page=\"0\" chnl=\"15\" />\
    <char id=\"34\" x=\"478,125\" y=\"332\" width=\"31,375\" height=\"31\" xoffset=\"-5\" yoffset=\"4,75\" xadvance=\"21,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"35\" x=\"299,5\" y=\"177,375\" width=\"47,25\" height=\"54,5\" xoffset=\"-4,375\" yoffset=\"6,5\" xadvance=\"38,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"36\" x=\"218,25\" y=\"0\" width=\"39,75\" height=\"66,125\" xoffset=\"-5,25\" yoffset=\"4,125\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"37\" x=\"336,75\" y=\"65,5\" width=\"62,5\" height=\"56\" xoffset=\"-5,375\" yoffset=\"5,75\" xadvance=\"51,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"38\" x=\"399,375\" y=\"64,75\" width=\"51,625\" height=\"56\" xoffset=\"-6,25\" yoffset=\"5,75\" xadvance=\"35,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"39\" x=\"0\" y=\"390,5\" width=\"21,625\" height=\"31\" xoffset=\"-5,25\" yoffset=\"4,75\" xadvance=\"11,125\" page=\"0\" chnl=\"15\" />\
    <char id=\"40\" x=\"0\" y=\"0\" width=\"31,625\" height=\"66,875\" xoffset=\"-5,25\" yoffset=\"4,75\" xadvance=\"20,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"41\" x=\"31,75\" y=\"0\" width=\"31,625\" height=\"66,875\" xoffset=\"-6,125\" yoffset=\"4,75\" xadvance=\"20,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"42\" x=\"319\" y=\"334,625\" width=\"39,375\" height=\"40\" xoffset=\"-5,125\" yoffset=\"4,75\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"43\" x=\"138\" y=\"289\" width=\"46,5\" height=\"46,375\" xoffset=\"-4\" yoffset=\"12,875\" xadvance=\"38,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"44\" x=\"406,25\" y=\"332\" width=\"27\" height=\"33\" xoffset=\"-6,125\" yoffset=\"37,625\" xadvance=\"16\" page=\"0\" chnl=\"15\" />\
    <char id=\"45\" x=\"118,625\" y=\"390,125\" width=\"30,625\" height=\"20,625\" xoffset=\"-5,625\" yoffset=\"25,75\" xadvance=\"19,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"46\" x=\"96,375\" y=\"390,125\" width=\"22,125\" height=\"23,375\" xoffset=\"-3,125\" yoffset=\"37,625\" xadvance=\"16\" page=\"0\" chnl=\"15\" />\
    <char id=\"47\" x=\"338,25\" y=\"0\" width=\"35,5\" height=\"64,625\" xoffset=\"-8,625\" yoffset=\"4,75\" xadvance=\"20,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"48\" x=\"217,125\" y=\"122,875\" width=\"40,625\" height=\"56\" xoffset=\"-5,875\" yoffset=\"5,75\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"49\" x=\"56\" y=\"179,75\" width=\"34,875\" height=\"54,625\" xoffset=\"-2,25\" yoffset=\"6,375\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"50\" x=\"383,5\" y=\"121,625\" width=\"39,75\" height=\"55,25\" xoffset=\"-5,125\" yoffset=\"5,75\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"51\" x=\"257,875\" y=\"122,625\" width=\"39,375\" height=\"56\" xoffset=\"-5,5\" yoffset=\"5,75\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"52\" x=\"267,625\" y=\"233,625\" width=\"42,625\" height=\"54,5\" xoffset=\"-7\" yoffset=\"6,5\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"53\" x=\"423,375\" y=\"120,875\" width=\"39\" height=\"55,25\" xoffset=\"-4,75\" yoffset=\"6,5\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"54\" x=\"134,875\" y=\"122,875\" width=\"41\" height=\"56\" xoffset=\"-5,75\" yoffset=\"5,75\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"55\" x=\"352,625\" y=\"231,625\" width=\"40,5\" height=\"54,5\" xoffset=\"-5,375\" yoffset=\"6,5\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"56\" x=\"295,5\" y=\"65,5\" width=\"41,125\" height=\"56,125\" xoffset=\"-6,125\" yoffset=\"5,625\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"57\" x=\"176\" y=\"122,875\" width=\"41\" height=\"56\" xoffset=\"-6,25\" yoffset=\"5,75\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"58\" x=\"296,75\" y=\"334,625\" width=\"22,125\" height=\"44,875\" xoffset=\"-1,75\" yoffset=\"16,125\" xadvance=\"18,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"59\" x=\"77,75\" y=\"289,75\" width=\"27\" height=\"54,5\" xoffset=\"-4,875\" yoffset=\"16,125\" xadvance=\"18,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"60\" x=\"0\" y=\"344,75\" width=\"43,75\" height=\"45,625\" xoffset=\"-2,75\" yoffset=\"13,375\" xadvance=\"38,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"61\" x=\"433,375\" y=\"332\" width=\"44,625\" height=\"31\" xoffset=\"-3\" yoffset=\"20,625\" xadvance=\"38,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"62\" x=\"435,625\" y=\"286,25\" width=\"43,75\" height=\"45,625\" xoffset=\"-2,375\" yoffset=\"13,375\" xadvance=\"38,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"63\" x=\"462,5\" y=\"117,375\" width=\"36,75\" height=\"55,25\" xoffset=\"-5,125\" yoffset=\"5,75\" xadvance=\"25,125\" page=\"0\" chnl=\"15\" />\
    <char id=\"64\" x=\"409,5\" y=\"0\" width=\"57,75\" height=\"61,125\" xoffset=\"-4,625\" yoffset=\"5,75\" xadvance=\"48,125\" page=\"0\" chnl=\"15\" />\
    <char id=\"65\" x=\"202,875\" y=\"179\" width=\"48,25\" height=\"54,5\" xoffset=\"-8,25\" yoffset=\"6,5\" xadvance=\"31,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"66\" x=\"310,375\" y=\"232\" width=\"42,125\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"31,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"67\" x=\"47,25\" y=\"123,625\" width=\"45,25\" height=\"56\" xoffset=\"-6,25\" yoffset=\"5,75\" xadvance=\"31,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"68\" x=\"441,375\" y=\"176,25\" width=\"46,375\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"36\" page=\"0\" chnl=\"15\" />\
    <char id=\"69\" x=\"433,75\" y=\"231,625\" width=\"40,125\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"29,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"70\" x=\"0\" y=\"290,125\" width=\"39,125\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"27,625\" page=\"0\" chnl=\"15\" />\
    <char id=\"71\" x=\"0\" y=\"124,125\" width=\"47,125\" height=\"56\" xoffset=\"-6,25\" yoffset=\"5,75\" xadvance=\"35,375\" page=\"0\" chnl=\"15\" />\
    <char id=\"72\" x=\"136,625\" y=\"234,375\" width=\"44\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"35,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"73\" x=\"474\" y=\"230,875\" width=\"30,875\" height=\"54,5\" xoffset=\"-5,5\" yoffset=\"6,5\" xadvance=\"19,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"74\" x=\"0\" y=\"180,25\" width=\"34,25\" height=\"55,125\" xoffset=\"-7,75\" yoffset=\"6,5\" xadvance=\"22,125\" page=\"0\" chnl=\"15\" />\
    <char id=\"75\" x=\"92,375\" y=\"234,375\" width=\"44,125\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"31,125\" page=\"0\" chnl=\"15\" />\
    <char id=\"76\" x=\"39,25\" y=\"290,125\" width=\"38,375\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"26,375\" page=\"0\" chnl=\"15\" />\
    <char id=\"77\" x=\"153,625\" y=\"179\" width=\"49,125\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"40,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"78\" x=\"180,75\" y=\"233,625\" width=\"43,625\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"35,375\" page=\"0\" chnl=\"15\" />\
    <char id=\"79\" x=\"451,125\" y=\"61,25\" width=\"50\" height=\"56\" xoffset=\"-6,25\" yoffset=\"5,75\" xadvance=\"37,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"80\" x=\"393,25\" y=\"231,625\" width=\"40,375\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"29,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"81\" x=\"288\" y=\"0\" width=\"50,125\" height=\"65,375\" xoffset=\"-6,25\" yoffset=\"5,75\" xadvance=\"37,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"82\" x=\"46,375\" y=\"235,125\" width=\"45,875\" height=\"54,5\" xoffset=\"-4,125\" yoffset=\"6,5\" xadvance=\"32,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"83\" x=\"92,625\" y=\"123,125\" width=\"42,125\" height=\"56\" xoffset=\"-6\" yoffset=\"5,75\" xadvance=\"29,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"84\" x=\"394,125\" y=\"177\" width=\"47,125\" height=\"54,5\" xoffset=\"-8,125\" yoffset=\"6,5\" xadvance=\"31\" page=\"0\" chnl=\"15\" />\
    <char id=\"85\" x=\"339,625\" y=\"121,625\" width=\"43,75\" height=\"55,25\" xoffset=\"-4,5\" yoffset=\"6,5\" xadvance=\"34,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"86\" x=\"251,25\" y=\"179\" width=\"48,125\" height=\"54,5\" xoffset=\"-8,25\" yoffset=\"6,5\" xadvance=\"31,625\" page=\"0\" chnl=\"15\" />\
    <char id=\"87\" x=\"91\" y=\"179,75\" width=\"62,5\" height=\"54,5\" xoffset=\"-7,375\" yoffset=\"6,5\" xadvance=\"47,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"88\" x=\"0\" y=\"235,5\" width=\"46,25\" height=\"54,5\" xoffset=\"-7,75\" yoffset=\"6,5\" xadvance=\"30,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"89\" x=\"346,875\" y=\"177\" width=\"47,125\" height=\"54,5\" xoffset=\"-8,25\" yoffset=\"6,5\" xadvance=\"30,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"90\" x=\"224,5\" y=\"233,625\" width=\"43\" height=\"54,5\" xoffset=\"-6,5\" yoffset=\"6,5\" xadvance=\"29,625\" page=\"0\" chnl=\"15\" />\
    <char id=\"91\" x=\"168,125\" y=\"0\" width=\"29,5\" height=\"66,375\" xoffset=\"-3,75\" yoffset=\"4,75\" xadvance=\"20,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"92\" x=\"373,875\" y=\"0\" width=\"35,5\" height=\"64,625\" xoffset=\"-6,625\" yoffset=\"4,75\" xadvance=\"20,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"93\" x=\"138,5\" y=\"0\" width=\"29,5\" height=\"66,375\" xoffset=\"-5,5\" yoffset=\"4,75\" xadvance=\"20,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"94\" x=\"358,5\" y=\"333\" width=\"47,625\" height=\"36,875\" xoffset=\"-4,5\" yoffset=\"6,5\" xadvance=\"38,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"95\" x=\"149,375\" y=\"387,875\" width=\"45,625\" height=\"19,375\" xoffset=\"-8,375\" yoffset=\"49,375\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"96\" x=\"69,375\" y=\"390,5\" width=\"26,875\" height=\"25,625\" xoffset=\"-1,625\" yoffset=\"1,625\" xadvance=\"28,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"97\" x=\"226,5\" y=\"288,25\" width=\"39\" height=\"46,375\" xoffset=\"-6,5\" yoffset=\"15,375\" xadvance=\"27,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"98\" x=\"0\" y=\"67\" width=\"40,25\" height=\"57\" xoffset=\"-4,5\" yoffset=\"4,75\" xadvance=\"29,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"99\" x=\"306,625\" y=\"288,25\" width=\"38,125\" height=\"46,25\" xoffset=\"-6,5\" yoffset=\"15,375\" xadvance=\"24,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"100\" x=\"467,375\" y=\"0\" width=\"40,25\" height=\"57\" xoffset=\"-6,5\" yoffset=\"4,75\" xadvance=\"29,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"101\" x=\"265,625\" y=\"288,25\" width=\"40,875\" height=\"46,25\" xoffset=\"-6,5\" yoffset=\"15,375\" xadvance=\"27,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"102\" x=\"80,75\" y=\"66,5\" width=\"34\" height=\"56,5\" xoffset=\"-7,25\" yoffset=\"4,5\" xadvance=\"16,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"103\" x=\"40,375\" y=\"67\" width=\"40,25\" height=\"56,5\" xoffset=\"-6,5\" yoffset=\"15,375\" xadvance=\"29,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"104\" x=\"235,75\" y=\"66,25\" width=\"38,625\" height=\"56,25\" xoffset=\"-4,5\" yoffset=\"4,75\" xadvance=\"29,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"105\" x=\"34,375\" y=\"180,25\" width=\"21,5\" height=\"54,75\" xoffset=\"-4,625\" yoffset=\"6,25\" xadvance=\"12,125\" page=\"0\" chnl=\"15\" />\
    <char id=\"106\" x=\"258,125\" y=\"0\" width=\"29,75\" height=\"65,625\" xoffset=\"-10,375\" yoffset=\"6,25\" xadvance=\"14,875\" page=\"0\" chnl=\"15\" />\
    <char id=\"107\" x=\"195,625\" y=\"66,5\" width=\"40\" height=\"56,25\" xoffset=\"-4,5\" yoffset=\"4,75\" xadvance=\"26,375\" page=\"0\" chnl=\"15\" />\
    <char id=\"108\" x=\"274,5\" y=\"65,75\" width=\"20,875\" height=\"56,25\" xoffset=\"-4,375\" yoffset=\"4,75\" xadvance=\"12,125\" page=\"0\" chnl=\"15\" />\
    <char id=\"109\" x=\"381,875\" y=\"286,25\" width=\"53,625\" height=\"45,625\" xoffset=\"-4,5\" yoffset=\"15,375\" xadvance=\"44,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"110\" x=\"43,875\" y=\"344,75\" width=\"38,625\" height=\"45,625\" xoffset=\"-4,5\" yoffset=\"15,375\" xadvance=\"29,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"111\" x=\"184,625\" y=\"288,25\" width=\"41,75\" height=\"46,375\" xoffset=\"-6,5\" yoffset=\"15,375\" xadvance=\"28,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"112\" x=\"155,25\" y=\"66,5\" width=\"40,25\" height=\"56,25\" xoffset=\"-4,5\" yoffset=\"15,375\" xadvance=\"29,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"113\" x=\"114,875\" y=\"66,5\" width=\"40,25\" height=\"56,25\" xoffset=\"-6,5\" yoffset=\"15,375\" xadvance=\"29,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"114\" x=\"479,5\" y=\"285,5\" width=\"31,375\" height=\"44,875\" xoffset=\"-4,5\" yoffset=\"16,125\" xadvance=\"19,125\" page=\"0\" chnl=\"15\" />\
    <char id=\"115\" x=\"344,875\" y=\"286,625\" width=\"36,875\" height=\"46,25\" xoffset=\"-6,25\" yoffset=\"15,375\" xadvance=\"23,625\" page=\"0\" chnl=\"15\" />\
    <char id=\"116\" x=\"104,875\" y=\"289\" width=\"33\" height=\"53,75\" xoffset=\"-7,375\" yoffset=\"7,875\" xadvance=\"17,75\" page=\"0\" chnl=\"15\" />\
    <char id=\"117\" x=\"82,625\" y=\"344,375\" width=\"38,625\" height=\"45,625\" xoffset=\"-4,625\" yoffset=\"16,125\" xadvance=\"29,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"118\" x=\"175,375\" y=\"335,5\" width=\"42,125\" height=\"44,875\" xoffset=\"-7,875\" yoffset=\"16,125\" xadvance=\"26,375\" page=\"0\" chnl=\"15\" />\
    <char id=\"119\" x=\"121,375\" y=\"342,875\" width=\"53,875\" height=\"44,875\" xoffset=\"-7,25\" yoffset=\"16,125\" xadvance=\"39,375\" page=\"0\" chnl=\"15\" />\
    <char id=\"120\" x=\"217,625\" y=\"334,75\" width=\"41,75\" height=\"44,875\" xoffset=\"-7,75\" yoffset=\"16,125\" xadvance=\"26,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"121\" x=\"297,375\" y=\"121,75\" width=\"42,125\" height=\"55,5\" xoffset=\"-7,875\" yoffset=\"16,125\" xadvance=\"26,375\" page=\"0\" chnl=\"15\" />\
    <char id=\"122\" x=\"259,5\" y=\"334,75\" width=\"37,125\" height=\"44,875\" xoffset=\"-6,75\" yoffset=\"16,125\" xadvance=\"23,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"123\" x=\"63,5\" y=\"0\" width=\"37,375\" height=\"66,375\" xoffset=\"-6,5\" yoffset=\"4,75\" xadvance=\"25,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"124\" x=\"197,75\" y=\"0\" width=\"20,375\" height=\"66,375\" xoffset=\"0\" yoffset=\"4,75\" xadvance=\"20,25\" page=\"0\" chnl=\"15\" />\
    <char id=\"125\" x=\"101\" y=\"0\" width=\"37,375\" height=\"66,375\" xoffset=\"-5,5\" yoffset=\"4,75\" xadvance=\"25,5\" page=\"0\" chnl=\"15\" />\
    <char id=\"126\" x=\"21,75\" y=\"390,5\" width=\"47,5\" height=\"29,75\" xoffset=\"-4,5\" yoffset=\"21\" xadvance=\"38,5\" page=\"0\" chnl=\"15\" />\
  </chars>\
";

const String fontDefTahoma_1 = "\
  <kernings count=\"360\">\
    <kerning first=\"123\" second=\"106\" amount=\"1,25\" />\
    <kerning first=\"123\" second=\"84\" amount=\"0,5\" />\
    <kerning first=\"39\" second=\"65\" amount=\"-2,375\" />\
    <kerning first=\"122\" second=\"113\" amount=\"-0,25\" />\
    <kerning first=\"40\" second=\"84\" amount=\"0,5\" />\
    <kerning first=\"40\" second=\"106\" amount=\"1,25\" />\
    <kerning first=\"44\" second=\"41\" amount=\"-2,375\" />\
    <kerning first=\"44\" second=\"55\" amount=\"-1,25\" />\
    <kerning first=\"44\" second=\"93\" amount=\"-2,375\" />\
    <kerning first=\"44\" second=\"125\" amount=\"-2,375\" />\
    <kerning first=\"122\" second=\"111\" amount=\"-0,25\" />\
    <kerning first=\"122\" second=\"103\" amount=\"-0,25\" />\
    <kerning first=\"45\" second=\"65\" amount=\"-1,25\" />\
    <kerning first=\"45\" second=\"73\" amount=\"-0,75\" />\
    <kerning first=\"45\" second=\"74\" amount=\"-2,375\" />\
    <kerning first=\"45\" second=\"83\" amount=\"-0,5\" />\
    <kerning first=\"45\" second=\"84\" amount=\"-3,75\" />\
    <kerning first=\"45\" second=\"86\" amount=\"-1,25\" />\
    <kerning first=\"45\" second=\"87\" amount=\"-1,25\" />\
    <kerning first=\"45\" second=\"88\" amount=\"-2\" />\
    <kerning first=\"45\" second=\"89\" amount=\"-3,375\" />\
    <kerning first=\"45\" second=\"90\" amount=\"-0,75\" />\
    <kerning first=\"45\" second=\"97\" amount=\"-0,5\" />\
    <kerning first=\"45\" second=\"118\" amount=\"-1\" />\
    <kerning first=\"45\" second=\"119\" amount=\"-0,5\" />\
    <kerning first=\"45\" second=\"120\" amount=\"-1,25\" />\
    <kerning first=\"45\" second=\"121\" amount=\"-1\" />\
    <kerning first=\"45\" second=\"122\" amount=\"-1\" />\
    <kerning first=\"122\" second=\"101\" amount=\"-0,25\" />\
    <kerning first=\"122\" second=\"100\" amount=\"-0,25\" />\
    <kerning first=\"46\" second=\"41\" amount=\"-2,625\" />\
    <kerning first=\"46\" second=\"44\" amount=\"-3,5\" />\
    <kerning first=\"46\" second=\"45\" amount=\"-4,25\" />\
    <kerning first=\"46\" second=\"55\" amount=\"-1,25\" />\
    <kerning first=\"46\" second=\"93\" amount=\"-2,625\" />\
    <kerning first=\"46\" second=\"125\" amount=\"-2,625\" />\
    <kerning first=\"122\" second=\"99\" amount=\"-0,25\" />\
    <kerning first=\"122\" second=\"45\" amount=\"-0,5\" />\
    <kerning first=\"121\" second=\"113\" amount=\"-0,25\" />\
    <kerning first=\"121\" second=\"111\" amount=\"-0,375\" />\
    <kerning first=\"52\" second=\"44\" amount=\"-0,5\" />\
    <kerning first=\"52\" second=\"46\" amount=\"-0,5\" />\
    <kerning first=\"55\" second=\"44\" amount=\"-3,875\" />\
    <kerning first=\"55\" second=\"46\" amount=\"-3,875\" />\
    <kerning first=\"121\" second=\"103\" amount=\"-0,25\" />\
    <kerning first=\"65\" second=\"45\" amount=\"-1,25\" />\
    <kerning first=\"65\" second=\"83\" amount=\"-0,125\" />\
    <kerning first=\"65\" second=\"84\" amount=\"-3\" />\
    <kerning first=\"65\" second=\"85\" amount=\"-0,25\" />\
    <kerning first=\"65\" second=\"86\" amount=\"-1,375\" />\
    <kerning first=\"65\" second=\"87\" amount=\"-1,25\" />\
    <kerning first=\"65\" second=\"89\" amount=\"-2\" />\
    <kerning first=\"65\" second=\"116\" amount=\"-0,375\" />\
    <kerning first=\"65\" second=\"117\" amount=\"-0,25\" />\
    <kerning first=\"65\" second=\"118\" amount=\"-0,75\" />\
    <kerning first=\"65\" second=\"119\" amount=\"-0,25\" />\
    <kerning first=\"65\" second=\"121\" amount=\"-0,75\" />\
    <kerning first=\"121\" second=\"101\" amount=\"-0,375\" />\
    <kerning first=\"121\" second=\"100\" amount=\"-0,25\" />\
    <kerning first=\"121\" second=\"99\" amount=\"-0,375\" />\
    <kerning first=\"121\" second=\"97\" amount=\"-1\" />\
    <kerning first=\"121\" second=\"46\" amount=\"-4,625\" />\
    <kerning first=\"121\" second=\"45\" amount=\"-1\" />\
    <kerning first=\"66\" second=\"44\" amount=\"-0,5\" />\
    <kerning first=\"66\" second=\"45\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"46\" amount=\"-0,5\" />\
    <kerning first=\"66\" second=\"67\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"71\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"79\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"81\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"84\" amount=\"-1,375\" />\
    <kerning first=\"66\" second=\"97\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"101\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"104\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"105\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"106\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"108\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"111\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"114\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"117\" amount=\"0,25\" />\
    <kerning first=\"66\" second=\"119\" amount=\"0,25\" />\
    <kerning first=\"121\" second=\"44\" amount=\"-4,625\" />\
    <kerning first=\"120\" second=\"111\" amount=\"-0,625\" />\
    <kerning first=\"120\" second=\"103\" amount=\"-0,25\" />\
    <kerning first=\"120\" second=\"101\" amount=\"-0,625\" />\
    <kerning first=\"67\" second=\"45\" amount=\"-1,25\" />\
    <kerning first=\"120\" second=\"100\" amount=\"-0,25\" />\
    <kerning first=\"120\" second=\"99\" amount=\"-0,5\" />\
    <kerning first=\"120\" second=\"45\" amount=\"-1,25\" />\
    <kerning first=\"68\" second=\"44\" amount=\"-1,25\" />\
    <kerning first=\"68\" second=\"46\" amount=\"-1,25\" />\
    <kerning first=\"68\" second=\"74\" amount=\"-0,5\" />\
    <kerning first=\"68\" second=\"84\" amount=\"-1,25\" />\
    <kerning first=\"68\" second=\"87\" amount=\"-0,5\" />\
    <kerning first=\"68\" second=\"88\" amount=\"-0,25\" />\
    <kerning first=\"68\" second=\"89\" amount=\"-0,5\" />\
    <kerning first=\"68\" second=\"90\" amount=\"-0,5\" />\
    <kerning first=\"119\" second=\"97\" amount=\"-0,5\" />\
    <kerning first=\"70\" second=\"44\" amount=\"-7,375\" />\
    <kerning first=\"70\" second=\"46\" amount=\"-7,375\" />\
    <kerning first=\"70\" second=\"58\" amount=\"-1,375\" />\
    <kerning first=\"119\" second=\"46\" amount=\"-1,75\" />\
    <kerning first=\"70\" second=\"63\" amount=\"1,375\" />\
    <kerning first=\"70\" second=\"65\" amount=\"-2,375\" />\
    <kerning first=\"70\" second=\"74\" amount=\"-1,5\" />\
    <kerning first=\"70\" second=\"84\" amount=\"0,75\" />\
    <kerning first=\"70\" second=\"97\" amount=\"-2,375\" />\
    <kerning first=\"70\" second=\"101\" amount=\"-1,25\" />\
    <kerning first=\"70\" second=\"111\" amount=\"-1,25\" />\
    <kerning first=\"119\" second=\"45\" amount=\"-0,5\" />\
    <kerning first=\"119\" second=\"44\" amount=\"-1,75\" />\
    <kerning first=\"118\" second=\"111\" amount=\"-0,375\" />\
    <kerning first=\"118\" second=\"101\" amount=\"-0,375\" />\
    <kerning first=\"118\" second=\"97\" amount=\"-1\" />\
    <kerning first=\"73\" second=\"45\" amount=\"-0,75\" />\
    <kerning first=\"73\" second=\"97\" amount=\"-0,5\" />\
    <kerning first=\"73\" second=\"99\" amount=\"-0,625\" />\
    <kerning first=\"73\" second=\"100\" amount=\"-0,5\" />\
    <kerning first=\"73\" second=\"101\" amount=\"-0,625\" />\
    <kerning first=\"73\" second=\"103\" amount=\"-0,5\" />\
    <kerning first=\"73\" second=\"109\" amount=\"-0,25\" />\
    <kerning first=\"73\" second=\"110\" amount=\"-0,25\" />\
    <kerning first=\"73\" second=\"111\" amount=\"-0,625\" />\
    <kerning first=\"73\" second=\"112\" amount=\"-0,25\" />\
    <kerning first=\"73\" second=\"114\" amount=\"-0,25\" />\
    <kerning first=\"73\" second=\"115\" amount=\"-0,5\" />\
    <kerning first=\"73\" second=\"118\" amount=\"-0,75\" />\
    <kerning first=\"118\" second=\"46\" amount=\"-4,375\" />\
    <kerning first=\"118\" second=\"45\" amount=\"-1\" />\
    <kerning first=\"74\" second=\"44\" amount=\"-0,5\" />\
    <kerning first=\"74\" second=\"46\" amount=\"-0,5\" />\
    <kerning first=\"74\" second=\"65\" amount=\"-0,25\" />\
    <kerning first=\"118\" second=\"44\" amount=\"-4,375\" />\
    <kerning first=\"116\" second=\"121\" amount=\"-0,5\" />\
    <kerning first=\"75\" second=\"45\" amount=\"-2,75\" />\
    <kerning first=\"75\" second=\"79\" amount=\"-0,5\" />\
    <kerning first=\"75\" second=\"97\" amount=\"-1,125\" />\
    <kerning first=\"75\" second=\"101\" amount=\"-1,5\" />\
    <kerning first=\"75\" second=\"111\" amount=\"-1,5\" />\
    <kerning first=\"75\" second=\"117\" amount=\"-1\" />\
    <kerning first=\"75\" second=\"118\" amount=\"-1,375\" />\
    <kerning first=\"75\" second=\"119\" amount=\"-1,5\" />\
    <kerning first=\"75\" second=\"121\" amount=\"-1,375\" />\
    <kerning first=\"116\" second=\"118\" amount=\"-0,5\" />\
    <kerning first=\"116\" second=\"116\" amount=\"-0,375\" />\
    <kerning first=\"116\" second=\"45\" amount=\"-1\" />\
    <kerning first=\"114\" second=\"113\" amount=\"-0,25\" />\
    <kerning first=\"114\" second=\"111\" amount=\"-0,25\" />\
    <kerning first=\"114\" second=\"103\" amount=\"-0,25\" />\
    <kerning first=\"114\" second=\"101\" amount=\"-0,25\" />\
    <kerning first=\"76\" second=\"39\" amount=\"-3\" />\
    <kerning first=\"76\" second=\"45\" amount=\"-3,875\" />\
    <kerning first=\"76\" second=\"65\" amount=\"-1\" />\
    <kerning first=\"76\" second=\"67\" amount=\"-0,5\" />\
    <kerning first=\"76\" second=\"71\" amount=\"-0,5\" />\
    <kerning first=\"76\" second=\"74\" amount=\"2,375\" />\
    <kerning first=\"76\" second=\"79\" amount=\"-0,5\" />\
    <kerning first=\"76\" second=\"81\" amount=\"-0,5\" />\
    <kerning first=\"76\" second=\"84\" amount=\"-4,25\" />\
    <kerning first=\"76\" second=\"86\" amount=\"-2,75\" />\
    <kerning first=\"76\" second=\"87\" amount=\"-2,375\" />\
    <kerning first=\"76\" second=\"89\" amount=\"-3,875\" />\
    <kerning first=\"76\" second=\"118\" amount=\"-2,75\" />\
    <kerning first=\"76\" second=\"121\" amount=\"-2,75\" />\
    <kerning first=\"114\" second=\"100\" amount=\"-0,25\" />\
    <kerning first=\"114\" second=\"99\" amount=\"-0,25\" />\
    <kerning first=\"114\" second=\"97\" amount=\"-0,875\" />\
    <kerning first=\"114\" second=\"46\" amount=\"-7,125\" />\
    <kerning first=\"114\" second=\"45\" amount=\"-0,625\" />\
    <kerning first=\"114\" second=\"44\" amount=\"-7,125\" />\
    <kerning first=\"112\" second=\"121\" amount=\"-0,125\" />\
    <kerning first=\"112\" second=\"46\" amount=\"-0,5\" />\
    <kerning first=\"79\" second=\"44\" amount=\"-0,75\" />\
    <kerning first=\"79\" second=\"46\" amount=\"-0,75\" />\
    <kerning first=\"79\" second=\"74\" amount=\"-0,25\" />\
    <kerning first=\"79\" second=\"84\" amount=\"-1,25\" />\
    <kerning first=\"79\" second=\"88\" amount=\"-0,25\" />\
    <kerning first=\"79\" second=\"89\" amount=\"-0,5\" />\
    <kerning first=\"79\" second=\"90\" amount=\"-0,5\" />\
    <kerning first=\"112\" second=\"44\" amount=\"-0,75\" />\
    <kerning first=\"80\" second=\"44\" amount=\"-7,375\" />\
    <kerning first=\"80\" second=\"46\" amount=\"-7,375\" />\
    <kerning first=\"80\" second=\"65\" amount=\"-1,25\" />\
    <kerning first=\"80\" second=\"74\" amount=\"-1,75\" />\
    <kerning first=\"80\" second=\"89\" amount=\"0,5\" />\
    <kerning first=\"80\" second=\"97\" amount=\"-1,25\" />\
";

const String fontDefTahoma_2 = "\
    <kerning first=\"80\" second=\"101\" amount=\"-1,25\" />\
    <kerning first=\"80\" second=\"111\" amount=\"-1,25\" />\
    <kerning first=\"111\" second=\"121\" amount=\"-0,25\" />\
    <kerning first=\"111\" second=\"120\" amount=\"-0,5\" />\
    <kerning first=\"111\" second=\"118\" amount=\"-0,25\" />\
    <kerning first=\"111\" second=\"46\" amount=\"-0,5\" />\
    <kerning first=\"111\" second=\"44\" amount=\"-0,75\" />\
    <kerning first=\"81\" second=\"44\" amount=\"-0,75\" />\
    <kerning first=\"81\" second=\"46\" amount=\"-0,75\" />\
    <kerning first=\"110\" second=\"121\" amount=\"-0,5\" />\
    <kerning first=\"82\" second=\"45\" amount=\"-2,375\" />\
    <kerning first=\"82\" second=\"84\" amount=\"-1,375\" />\
    <kerning first=\"82\" second=\"89\" amount=\"-0,5\" />\
    <kerning first=\"82\" second=\"97\" amount=\"-0,5\" />\
    <kerning first=\"82\" second=\"101\" amount=\"-1\" />\
    <kerning first=\"82\" second=\"111\" amount=\"-1\" />\
    <kerning first=\"82\" second=\"117\" amount=\"-0,25\" />\
    <kerning first=\"82\" second=\"121\" amount=\"-1,25\" />\
    <kerning first=\"110\" second=\"119\" amount=\"-0,25\" />\
    <kerning first=\"110\" second=\"118\" amount=\"-0,5\" />\
    <kerning first=\"109\" second=\"121\" amount=\"-0,5\" />\
    <kerning first=\"109\" second=\"119\" amount=\"-0,25\" />\
    <kerning first=\"109\" second=\"118\" amount=\"-0,5\" />\
    <kerning first=\"108\" second=\"116\" amount=\"0,125\" />\
    <kerning first=\"108\" second=\"108\" amount=\"0,125\" />\
    <kerning first=\"83\" second=\"44\" amount=\"-0,5\" />\
    <kerning first=\"83\" second=\"46\" amount=\"-0,5\" />\
    <kerning first=\"83\" second=\"65\" amount=\"-0,375\" />\
    <kerning first=\"83\" second=\"83\" amount=\"-0,625\" />\
    <kerning first=\"83\" second=\"99\" amount=\"0,125\" />\
    <kerning first=\"83\" second=\"101\" amount=\"0,125\" />\
    <kerning first=\"83\" second=\"111\" amount=\"0,125\" />\
    <kerning first=\"83\" second=\"113\" amount=\"0,125\" />\
    <kerning first=\"83\" second=\"118\" amount=\"-0,75\" />\
    <kerning first=\"83\" second=\"119\" amount=\"-0,5\" />\
    <kerning first=\"83\" second=\"121\" amount=\"-0,75\" />\
    <kerning first=\"107\" second=\"111\" amount=\"-0,5\" />\
    <kerning first=\"107\" second=\"101\" amount=\"-0,5\" />\
    <kerning first=\"84\" second=\"41\" amount=\"0,5\" />\
    <kerning first=\"84\" second=\"44\" amount=\"-7,125\" />\
    <kerning first=\"84\" second=\"45\" amount=\"-3,75\" />\
    <kerning first=\"84\" second=\"46\" amount=\"-7,125\" />\
    <kerning first=\"84\" second=\"58\" amount=\"-4,875\" />\
    <kerning first=\"107\" second=\"45\" amount=\"-2,375\" />\
    <kerning first=\"84\" second=\"63\" amount=\"1,375\" />\
    <kerning first=\"84\" second=\"65\" amount=\"-3\" />\
    <kerning first=\"84\" second=\"67\" amount=\"-1\" />\
    <kerning first=\"84\" second=\"71\" amount=\"-1\" />\
    <kerning first=\"84\" second=\"74\" amount=\"-1,5\" />\
    <kerning first=\"84\" second=\"79\" amount=\"-1,25\" />\
    <kerning first=\"84\" second=\"83\" amount=\"-0,25\" />\
    <kerning first=\"84\" second=\"84\" amount=\"-2,625\" />\
    <kerning first=\"84\" second=\"89\" amount=\"-1,75\" />\
    <kerning first=\"84\" second=\"93\" amount=\"0,5\" />\
    <kerning first=\"84\" second=\"97\" amount=\"-5,875\" />\
    <kerning first=\"84\" second=\"99\" amount=\"-5,375\" />\
    <kerning first=\"84\" second=\"101\" amount=\"-5,375\" />\
    <kerning first=\"84\" second=\"103\" amount=\"-5,25\" />\
    <kerning first=\"84\" second=\"111\" amount=\"-5,375\" />\
    <kerning first=\"84\" second=\"114\" amount=\"-4,875\" />\
    <kerning first=\"84\" second=\"115\" amount=\"-4,375\" />\
    <kerning first=\"84\" second=\"117\" amount=\"-4,875\" />\
    <kerning first=\"84\" second=\"118\" amount=\"-4,875\" />\
    <kerning first=\"84\" second=\"119\" amount=\"-4,875\" />\
    <kerning first=\"84\" second=\"121\" amount=\"-4,875\" />\
    <kerning first=\"84\" second=\"122\" amount=\"-4,25\" />\
    <kerning first=\"84\" second=\"125\" amount=\"0,5\" />\
    <kerning first=\"105\" second=\"116\" amount=\"0,125\" />\
    <kerning first=\"105\" second=\"102\" amount=\"0,125\" />\
    <kerning first=\"104\" second=\"121\" amount=\"-0,5\" />\
    <kerning first=\"104\" second=\"119\" amount=\"-0,25\" />\
    <kerning first=\"104\" second=\"118\" amount=\"-0,5\" />\
    <kerning first=\"103\" second=\"106\" amount=\"0,25\" />\
    <kerning first=\"102\" second=\"125\" amount=\"2,625\" />\
    <kerning first=\"102\" second=\"121\" amount=\"-0,625\" />\
    <kerning first=\"102\" second=\"117\" amount=\"-0,5\" />\
    <kerning first=\"102\" second=\"115\" amount=\"-0,625\" />\
    <kerning first=\"85\" second=\"44\" amount=\"-0,5\" />\
    <kerning first=\"85\" second=\"46\" amount=\"-0,5\" />\
    <kerning first=\"85\" second=\"65\" amount=\"-0,25\" />\
    <kerning first=\"102\" second=\"114\" amount=\"-0,25\" />\
    <kerning first=\"102\" second=\"113\" amount=\"-0,25\" />\
    <kerning first=\"86\" second=\"44\" amount=\"-7,125\" />\
    <kerning first=\"86\" second=\"45\" amount=\"-1,25\" />\
    <kerning first=\"86\" second=\"46\" amount=\"-7,125\" />\
    <kerning first=\"86\" second=\"58\" amount=\"-2\" />\
    <kerning first=\"102\" second=\"112\" amount=\"-0,25\" />\
    <kerning first=\"86\" second=\"65\" amount=\"-1,375\" />\
    <kerning first=\"86\" second=\"97\" amount=\"-2,375\" />\
    <kerning first=\"86\" second=\"101\" amount=\"-2,375\" />\
    <kerning first=\"86\" second=\"111\" amount=\"-2,375\" />\
    <kerning first=\"86\" second=\"117\" amount=\"-1,375\" />\
    <kerning first=\"86\" second=\"121\" amount=\"-1,625\" />\
    <kerning first=\"102\" second=\"111\" amount=\"-0,5\" />\
    <kerning first=\"102\" second=\"110\" amount=\"-0,25\" />\
    <kerning first=\"102\" second=\"109\" amount=\"-0,25\" />\
    <kerning first=\"102\" second=\"106\" amount=\"-0,5\" />\
    <kerning first=\"102\" second=\"103\" amount=\"-0,25\" />\
    <kerning first=\"102\" second=\"102\" amount=\"-0,375\" />\
    <kerning first=\"102\" second=\"101\" amount=\"-0,5\" />\
    <kerning first=\"87\" second=\"44\" amount=\"-7,125\" />\
    <kerning first=\"87\" second=\"45\" amount=\"-1,25\" />\
    <kerning first=\"87\" second=\"46\" amount=\"-5,375\" />\
    <kerning first=\"87\" second=\"58\" amount=\"-2\" />\
    <kerning first=\"102\" second=\"99\" amount=\"-0,5\" />\
    <kerning first=\"87\" second=\"65\" amount=\"-1,25\" />\
    <kerning first=\"87\" second=\"97\" amount=\"-2\" />\
    <kerning first=\"87\" second=\"101\" amount=\"-2\" />\
    <kerning first=\"87\" second=\"111\" amount=\"-2\" />\
    <kerning first=\"87\" second=\"114\" amount=\"-1\" />\
    <kerning first=\"87\" second=\"117\" amount=\"-1\" />\
    <kerning first=\"87\" second=\"121\" amount=\"-1,375\" />\
    <kerning first=\"102\" second=\"97\" amount=\"-0,625\" />\
    <kerning first=\"102\" second=\"93\" amount=\"2,625\" />\
    <kerning first=\"102\" second=\"92\" amount=\"2,375\" />\
    <kerning first=\"102\" second=\"63\" amount=\"3\" />\
    <kerning first=\"102\" second=\"46\" amount=\"-3,875\" />\
    <kerning first=\"102\" second=\"45\" amount=\"-1,75\" />\
    <kerning first=\"102\" second=\"44\" amount=\"-3,875\" />\
    <kerning first=\"88\" second=\"45\" amount=\"-2\" />\
    <kerning first=\"88\" second=\"67\" amount=\"-0,25\" />\
    <kerning first=\"88\" second=\"71\" amount=\"-0,25\" />\
    <kerning first=\"88\" second=\"79\" amount=\"-0,25\" />\
    <kerning first=\"88\" second=\"97\" amount=\"-1\" />\
    <kerning first=\"88\" second=\"101\" amount=\"-1,125\" />\
    <kerning first=\"88\" second=\"111\" amount=\"-1,125\" />\
    <kerning first=\"88\" second=\"117\" amount=\"-0,75\" />\
    <kerning first=\"88\" second=\"121\" amount=\"-1,75\" />\
    <kerning first=\"102\" second=\"42\" amount=\"1,75\" />\
    <kerning first=\"102\" second=\"41\" amount=\"2,625\" />\
    <kerning first=\"102\" second=\"39\" amount=\"2,25\" />\
    <kerning first=\"102\" second=\"34\" amount=\"2,25\" />\
    <kerning first=\"101\" second=\"84\" amount=\"-3,375\" />\
    <kerning first=\"99\" second=\"99\" amount=\"-0,125\" />\
    <kerning first=\"99\" second=\"84\" amount=\"-2,5\" />\
    <kerning first=\"99\" second=\"45\" amount=\"-0,5\" />\
    <kerning first=\"89\" second=\"44\" amount=\"-7,125\" />\
    <kerning first=\"89\" second=\"45\" amount=\"-3,375\" />\
    <kerning first=\"89\" second=\"46\" amount=\"-7,125\" />\
    <kerning first=\"89\" second=\"58\" amount=\"-4,875\" />\
    <kerning first=\"98\" second=\"121\" amount=\"-0,125\" />\
    <kerning first=\"89\" second=\"65\" amount=\"-2\" />\
    <kerning first=\"89\" second=\"74\" amount=\"-1,5\" />\
    <kerning first=\"89\" second=\"79\" amount=\"-0,5\" />\
    <kerning first=\"89\" second=\"97\" amount=\"-3,375\" />\
    <kerning first=\"89\" second=\"100\" amount=\"-3\" />\
    <kerning first=\"89\" second=\"101\" amount=\"-3,125\" />\
    <kerning first=\"89\" second=\"103\" amount=\"-3,125\" />\
    <kerning first=\"89\" second=\"109\" amount=\"-2,375\" />\
    <kerning first=\"89\" second=\"110\" amount=\"-2,375\" />\
    <kerning first=\"89\" second=\"111\" amount=\"-3,125\" />\
    <kerning first=\"89\" second=\"112\" amount=\"-2,375\" />\
    <kerning first=\"89\" second=\"113\" amount=\"-3,125\" />\
    <kerning first=\"89\" second=\"114\" amount=\"-2,375\" />\
    <kerning first=\"89\" second=\"115\" amount=\"-2,75\" />\
    <kerning first=\"89\" second=\"117\" amount=\"-2,75\" />\
    <kerning first=\"89\" second=\"118\" amount=\"-2,375\" />\
    <kerning first=\"98\" second=\"46\" amount=\"-0,5\" />\
    <kerning first=\"98\" second=\"44\" amount=\"-0,75\" />\
    <kerning first=\"97\" second=\"121\" amount=\"-0,375\" />\
    <kerning first=\"97\" second=\"119\" amount=\"-0,25\" />\
    <kerning first=\"97\" second=\"118\" amount=\"-0,375\" />\
    <kerning first=\"91\" second=\"106\" amount=\"1,25\" />\
    <kerning first=\"91\" second=\"84\" amount=\"0,5\" />\
    <kerning first=\"90\" second=\"121\" amount=\"-1,625\" />\
    <kerning first=\"90\" second=\"119\" amount=\"-1\" />\
    <kerning first=\"90\" second=\"45\" amount=\"-1,375\" />\
    <kerning first=\"90\" second=\"67\" amount=\"-0,5\" />\
    <kerning first=\"90\" second=\"71\" amount=\"-0,5\" />\
    <kerning first=\"90\" second=\"79\" amount=\"-0,5\" />\
    <kerning first=\"90\" second=\"90\" amount=\"-0,25\" />\
    <kerning first=\"90\" second=\"97\" amount=\"-1,25\" />\
    <kerning first=\"90\" second=\"101\" amount=\"-1,375\" />\
    <kerning first=\"90\" second=\"111\" amount=\"-1,375\" />\
  </kernings>\
</font>\
";
END_UPP_NAMESPACE