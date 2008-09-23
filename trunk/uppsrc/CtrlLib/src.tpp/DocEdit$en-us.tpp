topic "DocEdit";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[0 $$7,0#96390100711032703541132217272105:end]
[H6;0 $$8,0#05600065144404261032431302351956:begin]
[{_}%EN-US 
[s3; DocEdit&]
[s1;K:`:`:DocEdit`:`:class:%- [@(0.0.255) class]_[@0 DocEdit]_:_[@(0.0.255) public]_[^`:`:TextCtrl^ T
extCtrl]&]
[s2; DocEdit is form of raw text editor that interprets lines as 
paragraphs (wraps them).&]
[s2; &]
[s2; 
@@image:875&475
����̀��������������ێ۰��̉������Џ��������ʖ���Á������������׏����ӿ�����
�њ��锟��Ư�썝֗��������󆄮��̰��㑞����س����Ƴ�������ۈۄ��ݿ��ʑ���ʈ��
Չ��ئ�����Ԉà�١��ų㩨����󳮪��ǚ���ȋ��������������𪚹�ʃѯ���ìᬊ��
�Ʀ��۵���Ǒ������Ԙ���Ɲ���������͉訠�������鎧���Ԕ���ʹˬ撌�ы�ɨѤ����
�Ҩ�ʣ�ƨ����Ŀ���ލ�����Ų�ΐ�������;���̔�܄�����������п������£��
����������񤉁��В�������ሑ��И�������⾛��ȵ��¯�ޫ����ȅ�ʜ�봤�콥´����
�工������Ȭ��������Ŷ��ղ���������Ű����֬�͌�إ����М݊�����Ṍ��Թã��
ڨ�Ə�����К�ǒ��ե��܅����ن��唯�ԩ��͌��̕����훳�������؞��ǌ������߱��
��팉���������������؊�����̴�����Щ��ݒ��Ѽ�Ԥ������������ռ�ߤѐ����欃���
����Έ�܊�����ǂ����Պù���������ț�ѭ��ʰܤ����Ǆ��՝�������猓����ꬨ�ʣ����
��������Қ��锟欹�����������͈�����󆍿�꽆ȃݿǌ��懣��������ٝ�ޕ��ǴУ����
ؚ������ڷ������羭�������ǧ����҇�֣����񙱾����ዔ������ԏ�������ތҨ¿������
쓮ҫ���������面��㬶������������������滜��謂������ե������Ⱥ��������ǵ��
�������������̃������������������Ȕ����ʓ��З�������꫊��������ة�����݆���
Ɬƿ���ƫՅ��򢎴ʏҨ�ʻ����򣴿ʏ�����ݑ���〟
&]
[s0; &]
[s5;K:`:`:DocEdit`:`:After`(int`):%- [^`:`:DocEdit^ DocEdit]`&_[@0 SetFont]([^`:`:Font^ Fon
t]_[@3 f])&]
[s2; Sets the font to be used by widget.&]
[s0; &]
[s5;K:`:`:DocEdit`:`:SetFilter`(int`(`*`)`(int c`)`):%- [^`:`:DocEdit^ DocEdit]`&_[@0 Set
Filter]([@(0.0.255) int]_(`*[@3 f])([@(0.0.255) int]_c))&]
[s2; [%- Sets the character filter] [%-*@3 f]. All characters keystrokes 
are first altered by this function and used only if the result 
is not zero.&]
[s0; &]
[s5;K:`:`:DocEdit`:`:AutoHideSb`(bool`):%- [^`:`:DocEdit^ DocEdit]`&_[@0 AutoHideSb]([@(0.0.255) b
ool]_[@3 b]_`=_true)&]
[s2; In AutoHideSb mode, scrollbar is only visible when it is needed 
(text height is bigger than the height of the window). This mode 
is active by default.&]
[s0; &]
[s5;K:`:`:DocEdit`:`:UpDownLeave`(bool`):%- [^`:`:DocEdit^ DocEdit]`&_[@0 UpDownLeave]([@(0.0.255) b
ool]_[@3 u]_`=_true)&]
[s2; In UpDownLeave mode, Up key at the top of text or Down key at 
the bottom is not processed by DocEdit, which in turn means that 
it can be processed by parent Ctrl. In TopWindow, this will move 
the focus to previous / next Ctrl that IsWantFocus.&]
[s0; &]
[s5;K:`:`:DocEdit`:`:NoUpDownLeave`(`):%- [^`:`:DocEdit^ DocEdit]`&_[@0 NoUpDownLeave]()&]
[s2; Same as UpDownLeave(false).&]
[s0; &]
[s5;K:`:`:DocEdit`:`:IsUpDownLeave`(`)const:%- [@(0.0.255) bool]_[@0 IsUpDownLeave]()_[@(0.0.255) c
onst]&]
[s2; Tests UpDownLeave mode.&]
[s0; ]