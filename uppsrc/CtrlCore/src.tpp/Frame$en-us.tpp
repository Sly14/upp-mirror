TITLE("Frame")
REF("::CtrlFrame::class")
REF("::CtrlFrame::CtrlFrame()")
REF("::CtrlFrame::FrameLayout(::Rect&)")
REF("::CtrlFrame::FrameAddSize(::Size&)")
REF("::CtrlFrame::FramePaint(::Draw&,const::Rect&)")
REF("::CtrlFrame::FrameAdd(::Ctrl&)")
REF("::CtrlFrame::FrameRemove()")
REF("::FrameCtrl::class")
REF("::FrameLR::class")
REF("::FrameLR::FrameLR()")
REF("::FrameLeft::class")
REF("::FrameRight::class")
REF("::FrameTB::class")
REF("::FrameTB::Height(int)")
REF("::FrameTop::class")
REF("::FrameBottom::class")
REF("::LayoutFrameLeft(::Rect&,::Ctrl*,int)")
REF("::LayoutFrameRight(::Rect&,::Ctrl*,int)")
REF("::LayoutFrameTop(::Rect&,::Ctrl*,int)")
REF("::LayoutFrameBottom(::Rect&,::Ctrl*,int)")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i1120;a17;O9;~~~.1408;2 "
"$$1,0#10431211400427159095818037425705:param][a83;*R6 $$2,5#31310162474203024125188417583966:caption][b83;*4 "
"$$3,5#07864147445237544204411237157677:title][i288;O9;C2 $$4,6#40027414424643823182269349404212:item][b42;a42;2 "
"$$5,5#45413000475342174754091244180557:text][l288;b17;a17;2 $$6,6#27521748481378242620020725143825:")
TOPIC_TEXT(
"desc][l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#924304")
TOPIC_TEXT(
"59443460461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][ "
"$$19,0#535800234423355290")
TOPIC_TEXT(
"39900623488521:gap][t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested][b50;2 "
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s2; Frame&][s0; [^topic`:`/`/CtrlCore`/srcdoc`/AboutFrames`$en`-us^ "
"Frames] are objects derived from CtrlFrame class that form appearance and functionality "
"of area between outer Ctrl border and its view.&][s0; &][s0; &][s10;:`:`:CtrlFrame`:`:class:* "
"[%")
TOPIC_TEXT(
"00-00* class_][%00-00 CtrlFrame]&][s6; Interface definition of frame classes.&][s0;3 "
"&][s4;:`:`:CtrlFrame`:`:CtrlFrame`(`): [%00-00 `~][%00-00* CtrlFrame][%00-00 ()]&][s6; "
"Empty virtual destructor.&][s0;3 &][s4;:`:`:CtrlFrame`:`:FrameLayout`(`:`:Rect`&`):@3 "
"[%00-00@0 virtual void_][%00-00*@0 FrameLayout][%00-00@0 (Rect`&_][%00-00* r][%00-00@0 "
")_`=_][%00-00 0]&][s6; Frame reacts to this method by d")
TOPIC_TEXT(
"efining its own layout (if needed) and reducing the size of Ctrl view rectangle.&][s1; "
"[%00-00*C@3 r]-|Reference to current Ctrl rectangle. When Ctrl recomputes its layout, "
"it starts with Rect equivalent to its external size (GetRect().Size()). Then it calls "
"FrameLayout of all its frames (starting with frame 0) and resulting Rect is the size "
"of Ctrl`'s view.&][s0;3 &][s4;:`:`:CtrlFrame`:`:FrameAdd")
TOPIC_TEXT(
"Size`(`:`:Size`&`):@3 [%00-00@0 virtual void_][%00-00*@0 FrameAddSize][%00-00@0 "
"(Size`&_][%00-00* sz][%00-00@0 )_`=_][%00-00 0]&][s6; Adds size of the frame to the "
"current external size of Ctrl. This is used to compute the external size of Ctrl "
"for given size of view.&][s1; [%00-00*C@3 sz]-|Reference to actual size of Ctrl.&][s0;3 "
"&][s4;:`:`:CtrlFrame`:`:FramePaint`(`:`:Draw`&`,const`:`:Rect`&`): ")
TOPIC_TEXT(
"[%00-00 virtual void_][%00-00* FramePaint][%00-00 (Draw`&_][%00-00*@3 w][%00-00 "
", const_Rect`&_][%00-00*@3 r][%00-00 )]&][s6; Paint the frame. Default implementation "
"is empty.&][s1; [%00-00*C@3 w]-|Draw.&][s1; [%00-00*C@3 r]-|Outer rectangle of the "
"frame (this is the same rectangle as was given in last FrameLayout).&][s0;3 &][s4;:`:`:CtrlFrame`:`:FrameAdd`(`:`:Ctrl`&`): "
"[%00-00 virtual void_][%00-")
TOPIC_TEXT(
"00* FrameAdd][%00-00 (Ctrl`&_][%00-00*@3 parent][%00-00 )]&][s6; Called when frame "
"is added to the Ctrl. Frame can use it to add its subctrls to the parent. Default "
"implementation is empty.&][s1; [%00-00*C@3 parent]-|Parent Ctrl.&][s0;3 &][s4;:`:`:CtrlFrame`:`:FrameRemove`(`): "
"[%00-00 virtual void_][%00-00* FrameRemove][%00-00 ()]&][s6; Called when frame is "
"removed from the Ctrl. Frame can use it ")
TOPIC_TEXT(
"to remove subctrls from its parent. Default implementation is empty. &][s0; &][s0; "
"&][s3; Standard static frames&][s5; U`+`+ defines several standard static frames. "
"Those frames are mostly used to define (or alter) appearance of border of Ctrls. "
"All of them are obtained as a reference to single global instance by single global "
"function and can be assigned to unlimited number of Ctrls.&][s5; Appear")
TOPIC_TEXT(
"ance of some of them can be altered by current OS look`&feel.&][s5; &][ {{3967:1202:1908:2923h1;@(204) "
"[s5; Function]:: [s5; altered by look`&feel]:: [s5; Appearance]:: [s5; Comment]::@2 "
"[s7;l64;~~~96; CtrlFrame`&_[* NullFrame]()]:: [s5;= No.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣��������������ˍ�И����蠒���\377���ĉ����������՗�品���Μ�\377���ߠӢӀ�ӢӢӢ�Ӣ�����������Ǡ�\377��͉")
TOPIC_TEXT(
"�Չ̉̉̉�̉̉̉�Չ̉̉̉�̉̉̉�Չ̉̉�ɩ藾��\377�\377הȔȔ�\377�ȔȔȔ�Ȕ�������҂ן��������Į����]:: "
"[s5;1 Default Frame for Ctrl.]:: [s7;l64; CtrlFrame`&_[* InsetFrame]()]:: [s5;= No.]:: "
"[s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣��������������щ°Ԕ���醂�����Π㥪�ԑ�������䢥ũ������ڐ��ƾ�����̄������Ć̈���������ͶǮ����ܿ���������������Ҋ������ΰ˕�ǯ��͈������̄��������")
TOPIC_TEXT(
"������̄�Љ���ط���������ӳ������ݗ���ѡ����̃������̄���������Կ�̀������ΒĮ���]:: "
"[s5; ]:: [s7;l64; CtrlFrame`&_[* OutsetFrame]()]:: [s5;= No.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣�������������ڱ������ѣ��䈺��ظ��������窖���铏���ţ�����ز����\377��ĦĦ�ĦĦĦĽ�Ħ��������ޟ�ԟ�����׭񋾦Ħ�զĦĦĦ�ĦĦĦ�զĦĦĦ�ĦĦĦ��᭷��ډ�Ѧ���զٿڔ�Ħ�ĦĦĦĵ�ĦĦ�")
TOPIC_TEXT(
"��ъ�������Ť�Į���]:: [s5; ]:: [s7;l64; CtrlFrame`&_[* ButtonFrame]()]:: [s5;= "
"No.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣�������������ء������љȝ�����ꑵ�����������䋦��������ك�������̃������������������������鏲��������徰ɔ�������ث����͈�������̄������̈�������̄������ٷ������\377��������񭣅��������̄���������ȇ�Г�����䀀������Į����]:: "
"[s5; ]:: [s7")
TOPIC_TEXT(
";l64; CtrlFrame`&_[* ThinInsetFrame]()]:: [s5;= No.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣�����������������������ʕƜ�ܓ����ĝ�������\377�����������ײ؋��������̄����☦����������ݖ������������ҡ��̄�������̈�����̄��������È�����ࠏ���ﮒ����������ҡ��̄��������̈����Ĕ��������������ĉ����]:: "
"[s5; ]:: [s7;l64; CtrlFrame`&_[* ThinOutsetFrame]()]:: [s5;= No.]:: [")
TOPIC_TEXT(
"s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣���������������������ѣ��ص�����ҏ���׌\377��������ٽ��������������������������Ϋ�����ٞ����������͉�̉̉̉�Չ̉̉̉�̉̉̉�Չ̉̉̉�̉̉�����－����߭���Ց�����������������򦊙�������������Į����]:: "
"[s5; ]:: [s7;l64; CtrlFrame`&_[* BlackFrame]()]:: [s5;= No.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣����������������")
TOPIC_TEXT(
"������Ĝ������ƕ��ͨ��\377������á����졞����̈�����̄���������ݞ�����ʔ���������ӏ���̈�������̄������Ć̈������̄����������������������̄�������̩ȅ��������瀀������Į����]:: "
"[s5; ]:: [s7;l64; CtrlFrame`&_[* FieldFrame]()]:: [s5;= Yes.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣�������������ڻ�������Ӭ�����̌��������������������͖��͎�����ɟ���̄������Ć̈��")
TOPIC_TEXT(
"�������������ח�ئƐ���ׯ��ͰǦ�����̈�����̄�������̃��������̄���������������׋£�̃�����⦍�̈����Ƈ���聂ڃ��������Į���]:: "
"[s5;1 Good for borders of all Ctrls that display somthing, like EditField or ArrayCtrl.]:: "
"[s7;l64; CtrlFrame`&_[* TopSeparatorFrame]()]:: [s5;= Yes.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣��������������э������ؠ���������ڄ�����ߠ�����������")
TOPIC_TEXT(
"隦���ݏ�����̈�������̄ј���҄�����ٞ�蘯�锂�������өϱ�њŧ�э��ӯ��å����Ć̈������̄�������̈�����̄�������ކɿ�����̃�����⦍�̈������ǉ��ʁ�˰��������Į���]:: "
"[s5; ]:: [s7;l64; CtrlFrame`&_[* BottomSeparatorFrame]()]:: [s5;= Yes.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣�������������ڱ�°���ь�������ؿ��̘���������ƨ\377���ȧ����Է��ώ�ޅ࿘���⚦�̈����������Զ�")
TOPIC_TEXT(
"����͈�������̄��б���̈�������̃�������������ʒ�؞�����ъސ������̪��ި���\377����������̄��������ț�������ا�������ΒĮ���]:: "
"[s5; ]:: [s7;l64; CtrlFrame`&_[* LeftSeparatorFrame]()]:: [s5;= Yes.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣�������������ڱ����ѣ��Ȇ��휪�č��ᄹ�ߙ�����ϕ���ǥ���ۯ��������������ʌ��������������䵞�ϣה\377ȔȔȔ�\377�ȔȔȔ\377ȔȔȔ�\377�ȔȔ�")
TOPIC_TEXT(
"�\377ȔȔȔ�ᔌޭ�ה\377ȔȔȔ�\377�ȔȔȔ���壋�߆�����������Į����]:: [s5; ]:: "
"[s7;l64; CtrlFrame`&_[* RightSeparatorFrame]()]:: [s5;= Yes.]:: [s5;= @@PNG:612&237����Ǎ���������Ȁ�Ҁ�� �����������٣�������������ڱ��������萀Ԏ�؎������׀ވ��馽�Ϋ��\377�ߏ��΄ǯ����������ʀ���ʮ���㝿��������������������������ՙ��������������ՙ�������������׃�µ������������������٥���ֿ�݀�������ĉ����]:: "
"[s5; ]}}&][s")
TOPIC_TEXT(
"0;3 &][s0;3 &][s3; Simple Ctrl frame templates&][s0;3 &][s0;3 &][s10;:`:`:FrameCtrl`:`:class: "
"[%00-00 template_<class_][%00-00*@3 T][%00-00 >]&][s10;* [%00-00* class_][%00-00 "
"FrameCtrl]&][s6; &][s6; This is the base class of simple Ctrl frames `- frames that "
"place single Ctrl at some edge of parent Ctrl.&][s6; &][s6; This class basically "
"overloads FrameAdd and FrameRemove virtual methods of [^topi")
TOPIC_TEXT(
"c`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class CtrlFrame^ CtrlFrame] "
"so that they add/remove `'this`' from parent`'s children`-list.&][s6; &][s12; [%00-00*C@4 "
"T]-|Ctrl type.&][s12; &][s9;^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class "
"CtrlFrame^ [/^^ Derived from][^^  ][%00-00^^ T][%00-00^^@0 , ][%00-00 CtrlFrame]&][s0;3 "
"&][s10; &][s10; &][s10; [%00-00 template_<class")
TOPIC_TEXT(
"_][%00-00*@4 T][%00-00 >]&][s10;:`:`:FrameLR`:`:class:* [%00-00* class_][%00-00 "
"FrameLR]&][s6; &][s6; This class extends CtrlFrame class with width attribute and "
"serves as base class to classes placing Ctrl at the left or right size of parent "
"Ctrl frame. Width is initialized to 0. 0 as width indicates that width is equal to "
"the height.&][s6; &][s12; [%00-00*C@4 T]-|Ctrl type.&][s9; [/ Derived from")
TOPIC_TEXT(
"] [%00-00^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameCtrl`:`:template `<class "
"T`> class FrameCtrl^ FrameCtrl<T>][%00-00  ]&][s0;3 &][s4;:`:`:FrameLR`:`:FrameLR`(`): "
"[%00-00 FrameLR`&_][%00-00* Width][%00-00 (int_][%00-00*@3 `_cx][%00-00 )]&][s6; "
"Sets the new width.&][s1; [%00-00*C@3 `_cx]-|Width.&][s1; [*/ Return value]-|`*this.&][s0;3 "
"&][s0;3 &][s0;3 &][s10;:`:`:FrameLeft`:`:class: [%00-00")
TOPIC_TEXT(
" template_<class_][%00-00*@4 T][%00-00 >]&][s10;* [%00-00* class_][%00-00 FrameLeft]&][s6; "
"&][s6; This class places Ctrl to the parent`'s left side as frame.&][s12; [%00-00*C@4 "
"T]-|Ctrl type.&][s9; [/ Derived from] [%00-00^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameLR`:`:template "
"`<class T`> class FrameLR^ FrameLR<T>][%00-00  ]&][s0;3 &][s0;3 &][s0;3 &][s10;:`:`:FrameRight`:`:class: "
"[%00-00")
TOPIC_TEXT(
" template_<class_][%00-00*@4 T][%00-00 >]&][s10;* [%00-00* class_][%00-00 FrameRight]&][s6; "
"&][s6; This class places Ctrl to the parent`'s right side as frame.&][s12; [%00-00*C@4 "
"T]-|Ctrl type.&][s9; [/ Derived from] [%00-00^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameLR`:`:template "
"`<class T`> class FrameLR^ FrameLR<T>][%00-00  ]&][s0; &][s0; &][s0;3 &][s10;:`:`:FrameTB`:`:class: "
"[%00-00 te")
TOPIC_TEXT(
"mplate_<class_][%00-00*@4 T][%00-00 >]&][s10;* [%00-00* class_][%00-00 FrameTB]&][s6; "
"&][s6; This class extends CtrlFrame class with height attribute and serves as base "
"class to classes placing Ctrl as the top or bottom side of parent Ctrl frame. Height "
"is initialized to 0. 0 as height indicates that height is equal to the width.&][s6; "
"&][s12; [%00-00*C@4 T]-|Ctrl type.&][s9; [/ Derived from] [%00")
TOPIC_TEXT(
"-00^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameCtrl`:`:template `<class T`> "
"class FrameCtrl^ FrameCtrl<T>][%00-00  ]&][s0;3 &][s4;:`:`:FrameTB`:`:Height`(int`): "
"[%00-00 FrameTB`&_][%00-00* Height][%00-00 (int_][%00-00*@3 `_cy][%00-00 )]&][s6; "
"Sets the new height.&][s1; [%00-00*C@3 `_cy]-|Height.&][s1; [*/ Return value]-|`*this.&][s0; "
"&][s0; &][s0;3 &][s10;:`:`:FrameTop`:`:class: [%00-00 tem")
TOPIC_TEXT(
"plate_<class_][%00-00*@4 T][%00-00 >]&][s10;* [%00-00* class_][%00-00 FrameTop]&][s6; "
"&][s6; This class places Ctrl to the parent`'s top side as frame.&][s12; [%00-00*C@4 "
"T]-|Ctrl type.&][s9; [/ Derived from] [%00-00^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameTB`:`:template "
"`<class T`> class FrameTB^ FrameTB<T>][%00-00  ]&][s0; &][s0; &][s0;3 &][s10;:`:`:FrameBottom`:`:class: "
"[%00-00 templa")
TOPIC_TEXT(
"te_<class_][%00-00*@4 T][%00-00 >]&][s10;* [%00-00* class_][%00-00 FrameBottom]&][s6; "
"&][s6; This class places Ctrl to the parent`'s bottom side as frame.&][s12; [%00-00*C@4 "
"T]-|Ctrl type.&][s9; [/ Derived from] [%00-00^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameTB`:`:template "
"`<class T`> class FrameTB^ FrameTB<T>][%00-00  ]&][s0; &][s0; &][s3; Frame utility "
"functions&][s5; Following functi")
TOPIC_TEXT(
"ons are intended as helpers to for implementation of FrameLayout method of CtrlFrame, "
"placing some Ctrl to the side of parent Ctrl. They adjust given Rect (parameter of "
"FrameLayout) and also alter position of given Ctrl.&][s0;3 &][s4;:`:`:LayoutFrameLeft`(`:`:Rect`&`,`:`:Ctrl`*`,int`): "
"[%00-00 void_][%00-00* LayoutFrameLeft][%00-00 (Rect`&_][%00-00*@3 r][%00-00 , Ctrl_`*][%00-00*@3 "
"ctrl][%00-00 , ")
TOPIC_TEXT(
"int_][%00-00*@3 cx][%00-00 )]&][s6; Places ctrl at the left side of parent Ctrl.&][s1; "
"[%00-00*C@3 r]-|Current parent Ctrl rect.&][s1; [%00-00*C@3 ctrl]-|Ctrl to be placed.&][s1; "
"[%00-00*C@3 cx]-|Required width.&][s0;3 &][s4;:`:`:LayoutFrameRight`(`:`:Rect`&`,`:`:Ctrl`*`,int`): "
"[%00-00 void_][%00-00* LayoutFrameRight][%00-00 (Rect`&_][%00-00*@3 r][%00-00 , Ctrl_`*][%00-00*@3 "
"ctrl][%00-00 , int_][%")
TOPIC_TEXT(
"00-00*@3 cx][%00-00 )]&][s6; Places ctrl at the right side of parent Ctrl.&][s1; "
"[%00-00*C@3 r]-|Current parent Ctrl rect.&][s1; [%00-00*C@3 ctrl]-|Ctrl to be placed.&][s1; "
"[%00-00*C@3 cx]-|Required width.&][s0;3 &][s4;:`:`:LayoutFrameTop`(`:`:Rect`&`,`:`:Ctrl`*`,int`): "
"[%00-00 void_][%00-00* LayoutFrameTop][%00-00 (Rect`&_][%00-00*@3 r][%00-00 , Ctrl_`*][%00-00*@3 "
"ctrl][%00-00 , int_][%00-00*@3 c")
TOPIC_TEXT(
"y][%00-00 )]&][s6; Places ctrl at the top side of parent Ctrl.&][s1; [%00-00*C@3 "
"r]-|Current parent Ctrl rect.&][s1; [%00-00*C@3 ctrl]-|Ctrl to be placed.&][s1; [%00-00*C@3 "
"cy]-|Required height.&][s0;3 &][s4;:`:`:LayoutFrameBottom`(`:`:Rect`&`,`:`:Ctrl`*`,int`): "
"[%00-00 void_][%00-00* LayoutFrameBottom][%00-00 (Rect`&_][%00-00*@3 r][%00-00 , "
"Ctrl_`*][%00-00*@3 ctrl][%00-00 , int_][%00-00*@3 cy][%0")
TOPIC_TEXT(
"0-00 )]&][s6; Places ctrl at the bottom side of parent Ctrl.&][s1; [%00-00*C@3 r]-|Current "
"parent Ctrl rect.&][s1; [%00-00*C@3 ctrl]-|Ctrl to be placed.&][s1; [%00-00*C@3 cy]-|Required "
"height.&][s0;3 ]")
