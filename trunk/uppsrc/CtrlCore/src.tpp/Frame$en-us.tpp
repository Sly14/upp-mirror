topic "Frame";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[{_}%EN-US 
[s3; Frame&]
[s0; [^topic`:`/`/CtrlCore`/srcdoc`/AboutFrames`$en`-us^ Frames] are 
objects derived from CtrlFrame class that form appearance and 
functionality of area between outer Ctrl border and its view.&]
[s0; &]
[s0; &]
[s5;K%- [@(0.0.255) class]_[@0 CtrlFrame]&]
[s2; Interface definition of frame classes.&]
[s0;3 &]
[s0;:`:`:CtrlFrame`:`:CtrlFrame`(`):%- `~[* CtrlFrame]()&]
[s2; Empty virtual destructor.&]
[s0;3 &]
[s5;K%- virtual [@(0.0.255) void]_[@0 FrameLayout]([^`:`:Rect^ Rect]`&_[@3 r])_`=_[@3 0]&]
[s2; Frame reacts to this method by defining its own layout (if needed) 
and reducing the size of Ctrl view rectangle.&]
[s4; [%-*C@3 r]-|Reference to current Ctrl rectangle. When Ctrl recomputes 
its layout, it starts with Rect equivalent to its external size 
(GetRect().Size()). Then it calls FrameLayout of all its frames 
(starting with frame 0) and resulting Rect is the size of Ctrl`'s 
view.&]
[s0;3 &]
[s5;K%- virtual [@(0.0.255) void]_[@0 FrameAddSize]([^`:`:Size^ Size]`&_[@3 sz])_`=_[@3 0]&]
[s2; Adds size of the frame to the current external size of Ctrl. 
This is used to compute the external size of Ctrl for given size 
of view.&]
[s4; [%-*C@3 sz]-|Reference to actual size of Ctrl.&]
[s0;3 &]
[s5;K%- virtual [@(0.0.255) void]_[@0 FramePaint]([^`:`:Draw^ Draw]`&_[@3 w], 
[@(0.0.255) const]_[^`:`:Rect^ Rect]`&_[@3 r])&]
[s2; Paint the frame. Default implementation is empty.&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 r]-|Outer rectangle of the frame (this is the same rectangle 
as was given in last FrameLayout).&]
[s0;3 &]
[s5;K%- virtual [@(0.0.255) void]_[@0 FrameAdd]([^`:`:Ctrl^ Ctrl]`&_[@3 parent])&]
[s2; Called when frame is added to the Ctrl. Frame can use it to 
add its subctrls to the parent. Default implementation is empty.&]
[s4; [%-*C@3 parent]-|Parent Ctrl.&]
[s0;3 &]
[s5;K%- virtual [@(0.0.255) void]_[@0 FrameRemove]()&]
[s2; Called when frame is removed from the Ctrl. Frame can use it 
to remove subctrls from its parent. Default implementation is 
empty. &]
[s0; &]
[s5;K:`:`:CtrlFrame`:`:OverPaint`(`)const:%- virtual [@(0.0.255) int]_[@0 OverPaint]()_[@(0.0.255) c
onst]&]
[s2; This method can returns non`-zero number that represents paint 
extension margin of Ctrl rectangle `- frame can paint over this 
margin despite that fact that it does not belong to the Ctrl 
rectangle. This is useful to represent some specific skinning 
effect (like glare around the EditField). Default implementation 
returns zero.  &]
[s0; &]
[s0; &]
[s0; &]
[s0; Standard static frames&]
[s0; U`+`+ defines several standard static frames. Those frames are 
mostly used to define (or alter) appearance of border of Ctrls. 
All of them are obtained as a reference to single global instance 
by single global function and can be assigned to unlimited number 
of Ctrls.&]
[s0; Appearance of some of them can be altered by current OS look`&feel.&]
[s0; &]
[ {{3967:1202:1908:2923h1;@(204) [s0; Function]
:: [s0; altered by look`&feel]
:: [s0; Appearance]
:: [s0; Comment]
::@2 [s0; CtrlFrame`&_[* NullFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
��…��������������������������������������֜����ٟ��ɥɒ˥�ʮ��ܪ������ɥ�˥�
�ʮ�ܪ������ɥ��˥�ˀ��������
]
:: [s0; [1 Default Frame for Ctrl.]]
:: [s0; CtrlFrame`&_[* InsetFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
��…�����������������������������ؐ��Ѓ���ęܹͤ���������ٷ��׵���˜�ʿ�٩ΰ�
����̥䪨�ө�����ʦҲ�����̥䪥�ʩ�����֎٩�ᗵ׬��᷷�Ҍ���ۅ���Ƚ
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* OutsetFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
��…��������������ұ�������뿃�����܂�����頻�������ߨ����ጱ���������鶫��Բ
���ʥ����Բ��ʥ����Բ��ʬ����Բ����������ך���ˀ����������
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* ButtonFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
��…��������������ѱ�������ￃ� �����ʀ�𲅄��Ӥ���ﬀ����������댲�����
�����̥�Ъ�ө�����ʦҲ�����̥䪥�ө����ʔ��鹟��������������᳿
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* ThinInsetFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
��…�������������������Ȁ���������ؠ����ӄ�������񝃀�������̹���Ϳͥ����Ԫө�
�����Ԫӵ�̥����Ԫө̥����Ԫө̥����߿���땀���������
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* ThinOutsetFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
��…��������������б�����������ف������ʁ�Ϩ��ӆ޳㺇�������Ǿ������Ԫӵ�̥����
Ԫө̥����Ԫө̥Ʋ��Ԫө�̥��ʦ���ǥ�Ղ�ˀ�����
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* BlackFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
��…���������������э��������Ă�ɼ���߻����܋��������һ��ʮ��ܪ������ɥ�˥�
�ʮ�ܪ������ɥ��˥�ʧ�ơ���������ꙣ
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* FieldFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
��…���������������������������������������ל���瞀��㷼����ݦɗʮ�ܪ������ɥ
ɒ˥�ʮ��ܪ������ɥ�˥���߮���Ѐ�ē��ہ�
]
:: [s0; [1 Good for borders of all Ctrls that display somthing, like EditField 
or ArrayCtrl.]]
:: [s0; CtrlFrame`&_[* TopSeparatorFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
��…���������������щ������������È�݄��������籉��߀�������������߸ꪮ���
�Ѹ�ˮ���ˮ����ˮ��Ҵ��ݻ������뾅�
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* BottomSeparatorFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
��…���������������щ������������È�݄��������籉��߀��������ɥ�܊ƥ���򩚗�
��ɥ����������ޥ���ݚɢ������������
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* LeftSeparatorFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
��…�����������������������������ژ�ª�Џ������蠊��灺����ժ�ˎ����ٲ�ƒ�˖�
�ز��Қ�ᖬ���ɒ�������蚃�ㄘ
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* RightSeparatorFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
��…���������������щ��̘����찈���������߅߄Ϻ����浀�����񧶣��ٲ�ҩ��˖��
ݱ�ɒ�����ٲ�ƒ����ق������Ä�
]
:: [s0; ]}}&]
[s0;3 &]
[s0;3 &]
[s0; Simple Ctrl frame templates&]
[s0;3 &]
[s0;3 &]
[s5;K%- template_<[@(0.0.255) class]_[@3 T]>__[@(0.0.255) class]_[@0 FrameCtrl]_:_[@(0.0.255) pu
blic]_[@3 T], [@(0.0.255) public]_[^`:`:CtrlFrame^ CtrlFrame]&]
[s0;%- class_[* FrameCtrl]&]
[s2; &]
[s2; This is the base class of simple Ctrl frames `- frames that 
place single Ctrl at some edge of parent Ctrl.&]
[s2; &]
[s2; This class basically overloads FrameAdd and FrameRemove virtual 
methods of [^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class CtrlFrame^ C
trlFrame] so that they add/remove `'this`' from parent`'s children`-list.&]
[s2; &]
[s4; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ]T, [^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class CtrlFrame^@(0.0.255) C
trlFrame]&]
[s0;3 &]
[s0; &]
[s0; &]
[s0;%- template_<class_[*@4 T]>&]
[s5;K%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) class]_[@0 FrameLR]_:_[@(0.0.255) publ
ic]_[^`:`:FrameCtrl^ FrameCtrl]<[@4 T]>_&]
[s2; &]
[s2; This class extends CtrlFrame class with width attribute and 
serves as base class to classes placing Ctrl at the left or right 
size of parent Ctrl frame. Width is initialized to 0. 0 as width 
indicates that width is equal to the height.&]
[s2; &]
[s4; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameCtrl`:`:template `<class T`> class FrameCtrl^ F
rameCtrl<T>] &]
[s0;3 &]
[s0;:`:`:FrameLR`:`:FrameLR`(`):%- FrameLR`&_[* Width](int_[*@3 `_cx])&]
[s2; Sets the new width.&]
[s4; [%-*C@3 `_cx]-|Width.&]
[s4; [*/ Return value]-|`*this.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetWidth]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current width.&]
[s0; &]
[s0;3 &]
[s0;3 &]
[s0;3 &]
[s5;K%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) class]_[@0 FrameLeft]_:_[@(0.0.255) pu
blic]_[^`:`:FrameLR^ FrameLR]<[@4 T]>_&]
[s0;%- class_[* FrameLeft]&]
[s2; &]
[s2; This class places Ctrl to the parent`'s left side as frame.&]
[s0; &]
[s4; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameLR`:`:template `<class T`> class FrameLR^ F
rameLR<T>] &]
[s0;3 &]
[s0;3 &]
[s0;3 &]
[s5;K%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) class]_[@0 FrameRight]_:_[@(0.0.255) p
ublic]_[^`:`:FrameLR^ FrameLR]<[@4 T]>_&]
[s0;%- class_[* FrameRight]&]
[s2; &]
[s2; This class places Ctrl to the parent`'s right side as frame.&]
[s0; [%-*C@4 T]-|Ctrl type.&]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameLR`:`:template `<class T`> class FrameLR^ F
rameLR<T>] &]
[s0; &]
[s0; &]
[s0;3 &]
[s5;K%- [@(0.0.255) template]_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) class]_[@0 FrameTB]_:_[@(0.0.255) p
ublic]_[^`:`:FrameCtrl^ FrameCtrl]<[@4 T]>_&]
[s0;%- class_[* FrameTB]&]
[s2; &]
[s2; This class extends CtrlFrame class with height attribute and 
serves as base class to classes placing Ctrl as the top or bottom 
side of parent Ctrl frame. Height is initialized to 0. 0 as height 
indicates that height is equal to the width.&]
[s2; &]
[s0; [%-*C@4 T]-|Ctrl type.&]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameCtrl`:`:template `<class T`> class FrameCtrl^ F
rameCtrl<T>] &]
[s0;3 &]
[s5;K%- [^`:`:FrameTB^ FrameTB]`&_[@0 Height]([@(0.0.255) int]_[@3 `_cy])&]
[s2; Sets the new height.&]
[s4; [%-*C@3 `_cy]-|Height.&]
[s4; [*/ Return value]-|`*this.&]
[s0; &]
[s5;K%- [@(0.0.255) int]_[@0 GetHeight]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current height.&]
[s0; &]
[s0; &]
[s0; &]
[s0;3 &]
[s5;K%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) class]_[@0 FrameTop]_:_[@(0.0.255) pub
lic]_[^`:`:FrameTB^ FrameTB]<[@4 T]>_&]
[s0;%- class_[* FrameTop]&]
[s2; &]
[s2; This class places Ctrl to the parent`'s top side as frame.&]
[s0; [%-*C@4 T]-|Ctrl type.&]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameTB`:`:template `<class T`> class FrameTB^ F
rameTB<T>] &]
[s0; &]
[s0; &]
[s0;3 &]
[s5;K%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) class]_[@0 FrameBottom]_:_[@(0.0.255) p
ublic]_[^`:`:FrameTB^ FrameTB]<[@4 T]>_&]
[s0;%- class_[* FrameBottom]&]
[s2; &]
[s2; This class places Ctrl to the parent`'s bottom side as frame.&]
[s0; [%-*C@4 T]-|Ctrl type.&]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameTB`:`:template `<class T`> class FrameTB^ F
rameTB<T>] &]
[s0; &]
[s0; &]
[s0; Frame utility functions&]
[s0; Following functions are intended as helpers to for implementation 
of FrameLayout method of CtrlFrame, placing some Ctrl to the 
side of parent Ctrl. They adjust given Rect (parameter of FrameLayout) 
and also alter position of given Ctrl.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 LayoutFrameLeft]([^`:`:Rect^ Rect]`&_[@3 r], 
[^`:`:Ctrl^ Ctrl]_`*[@3 ctrl], [@(0.0.255) int]_[@3 cx])&]
[s2; Places ctrl at the left side of parent Ctrl.&]
[s4; [%-*C@3 r]-|Current parent Ctrl rect.&]
[s4; [%-*C@3 ctrl]-|Ctrl to be placed.&]
[s4; [%-*C@3 cx]-|Required width.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 LayoutFrameRight]([^`:`:Rect^ Rect]`&_[@3 r], 
[^`:`:Ctrl^ Ctrl]_`*[@3 ctrl], [@(0.0.255) int]_[@3 cx])&]
[s2; Places ctrl at the right side of parent Ctrl.&]
[s4; [%-*C@3 r]-|Current parent Ctrl rect.&]
[s4; [%-*C@3 ctrl]-|Ctrl to be placed.&]
[s4; [%-*C@3 cx]-|Required width.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 LayoutFrameTop]([^`:`:Rect^ Rect]`&_[@3 r], 
[^`:`:Ctrl^ Ctrl]_`*[@3 ctrl], [@(0.0.255) int]_[@3 cy])&]
[s2; Places ctrl at the top side of parent Ctrl.&]
[s4; [%-*C@3 r]-|Current parent Ctrl rect.&]
[s4; [%-*C@3 ctrl]-|Ctrl to be placed.&]
[s4; [%-*C@3 cy]-|Required height.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 LayoutFrameBottom]([^`:`:Rect^ Rect]`&_[@3 r], 
[^`:`:Ctrl^ Ctrl]_`*[@3 ctrl], [@(0.0.255) int]_[@3 cy])&]
[s2; Places ctrl at the bottom side of parent Ctrl.&]
[s4; [%-*C@3 r]-|Current parent Ctrl rect.&]
[s4; [%-*C@3 ctrl]-|Ctrl to be placed.&]
[s4; [%-*C@3 cy]-|Required height.&]
[s0; ]