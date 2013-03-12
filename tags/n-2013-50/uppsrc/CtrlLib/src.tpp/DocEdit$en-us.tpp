topic "DocEdit";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[ {{10000@3 [s0; [*@(229)4 DocEdit]]}}&]
[s3; &]
[s1;:DocEdit`:`:class:%- [@(0.0.255)3 class][3 _][*3 DocEdit][3 _:_][@(0.0.255)3 public][3 _][*@3;3 T
extCtrl]&]
[s9; DocEdit is form of raw text editor that interprets lines as 
paragraphs (wraps them).&]
[s0; &]
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
[s0; [/ Derived from] [^topic`:`/`/CtrlLib`/src`/TextCtrl`$en`-us`#TextCtrl`:`:class^ Tex
tCtrl]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:DocEdit`:`:SetFont`(Font`):%- [_^DocEdit^ DocEdit][@(0.0.255) `&]_[* SetFont]([_^Font^ F
ont]_[*@3 f])&]
[s2; Sets the font to be used by widget to [%-*@3 f].&]
[s3; &]
[s4;%- &]
[s5;:DocEdit`:`:SetFilter`(int`(`*`)`(int c`)`):%- [_^DocEdit^ DocEdit][@(0.0.255) `&]_[* S
etFilter]([@(0.0.255) int]_(`*[*@3 f])([@(0.0.255) int]_c))&]
[s2; [%- Sets the character filter] [%-*@3 f]. All characters keystrokes 
are first altered by this function and used only if the result 
is not zero.&]
[s3; &]
[s4;%- &]
[s5;:DocEdit`:`:AutoHideSb`(bool`):%- [_^DocEdit^ DocEdit][@(0.0.255) `&]_[* AutoHideSb]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; In AutoHideSb mode, scrollbar is only visible when it is needed 
(text height is bigger than the height of the window). This mode 
is active by default.&]
[s3; &]
[s4;%- &]
[s5;:DocEdit`:`:UpDownLeave`(bool`):%- [_^DocEdit^ DocEdit][@(0.0.255) `&]_[* UpDownLeave](
[@(0.0.255) bool]_[*@3 u]_`=_[@(0.0.255) true])&]
[s2; In UpDownLeave mode, Up key at the top of text or Down key at 
the bottom is not processed by DocEdit, which in turn means that 
it can be processed by parent Ctrl. In TopWindow, this will move 
the focus to previous / next Ctrl that IsWantFocus.&]
[s3; &]
[s4;%- &]
[s5;:DocEdit`:`:NoUpDownLeave`(`):%- [_^DocEdit^ DocEdit][@(0.0.255) `&]_[* NoUpDownLeave](
)&]
[s2; Same as UpDownLeave(false).&]
[s3; &]
[s4;%- &]
[s5;:DocEdit`:`:IsUpDownLeave`(`)const:%- [@(0.0.255) bool]_[* IsUpDownLeave]()_[@(0.0.255) c
onst]&]
[s2; Tests UpDownLeave mode.&]
[s3; &]
[s0; ]