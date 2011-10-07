topic "Scatter Reference";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Scatter]]}}&]
[s3; &]
[s1;:Scatter`:`:class: [@(0.0.255)3 class][3 _][*3 Scatter][3 _:_][@(0.0.255)3 public][3 _][*@3;3 St
aticRect]&]
[s9; A class to plot 2D graphs.&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s4; &]
[s5;:Scatter`:`:class: [@(0.0.255) void]_[* FitToData]([@(0.0.255) bool]_[*@3 Y]_`=_[@(0.0.255) f
alse])&]
[s0;l288;%% Changes X axis zoom to fit visible all data in Ctrl.&]
[s0;l288;i448;a25;kKO9;%% If [%-*@3 Y ]is true, Y axis zoom is also 
changed to fit the data.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SaveToClipboard`(bool`): [@(0.0.255) void]_[* SaveToClipboard]([@(0.0.255) b
ool]_[*@3 saveAsMetafile]_`=_[@(0.0.255) false])&]
[s2;%% Saves graph to clipboard as a bitmap.&]
[s2;%% In windows if [%-*@3 saveAsMetafile ]is true, a metafile is 
sent to clipboard.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SaveToImage`(String`): [@(0.0.255) void]_[* SaveToImage]([_^String^ String]_
[*@3 fileName]_`=_Null)&]
[s2;%% Saves graph to file [%-*@3 fileName.] Supported formats are 
.PNG and .JPG.&]
[s2;%% If [%-*@3 fileName] is null, a window is opened asking the user 
to enter a file name.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetFastViewX`(bool`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* SetFastViewX](
[@(0.0.255) bool]_[*@3 set]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 set ]is true, it will be viewed a point per horizontal 
pixel. This point will be the average of all data that is in 
that pixel.&]
[s2;%% This speeds up the graph painting.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetTitleFont`(`): [_^Font^ Font][@(0.0.255) `&]_[* GetTitleFont]()&]
[s2;%% Returns title font.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetXByPoint`(const int`): [@(0.0.255) double]_[* GetXByPoint]([@(0.0.255) c
onst]_[@(0.0.255) int]_[*@3 x])&]
[s2;%% Gets the X coordinate in data units from value [%-*@3 x ]in 
screen pixels inside Ctrl.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetYByPoint`(const int`): [@(0.0.255) double]_[* GetYByPoint]([@(0.0.255) c
onst]_[@(0.0.255) int]_[*@3 y])&]
[s2;%% Gets the Y coordinate in data units from value [%-*@3 y ]in 
screen pixels inside Ctrl.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetY2ByPoint`(const int`): [@(0.0.255) double]_[* GetY2ByPoint]([@(0.0.255) c
onst]_[@(0.0.255) int]_[*@3 y])&]
[s2;%% Gets the Y2 coordinate in data units from value [%-*@3 y ]in 
screen pixels inside Ctrl.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetXPointByValue`(const double`): [@(0.0.255) double]_[* GetXPointByValue
]([@(0.0.255) const]_[@(0.0.255) double]_[*@3 x])&]
[s2;%% Gets the horizontal position in pixels  inside Ctrl for a 
value [%-*@3 x ]in data units.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetYPointByValue`(const double`): [@(0.0.255) double]_[* GetYPointByValue
]([@(0.0.255) const]_[@(0.0.255) double]_[*@3 y])&]
[s2;%% Gets the vertical position in pixels inside Ctrl for a value 
[%-*@3 y ]in data units.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:ShowInfo`(bool`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* ShowInfo]([@(0.0.255) b
ool]_[*@3 show][@(0.0.255) `=true])&]
[s2;%% Defines to show or not with [%-*@3 show ]the pop`-up window 
that indicates in data units the cursor location and, if dragged, 
the size of the window.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:ShowContextMenu`(const bool`&`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* Sho
wContextMenu]([@(0.0.255) const]_[@(0.0.255) bool`&]_[*@3 show][@(0.0.255) `=true])&]
[s2;%% Defines to show or not with [%-*@3 show ]the context menu with 
options to save the plot to clipboard or file. Formats supported 
are .PNG and .JPG &]
[s2;%% &]
[s2;%% 
@@image:875&375
��������������������ӓ�ޘ����íМ���������㉭ӱ������������Ԫむ�����고�����
���ᢢ��΀��������ƚ�ɶ���䷥��绻������������������򡃻��䴗���Ņ��������
��̙ͮ�����͓�ʍ�ۂ֦������Ӝ��ŋ�����Ը������Ȯ�ˮӀ�ի�Ս帼�ȯ�����������
���������ڳ�������ʧ�ѹ��Ĩ����ܹ������������˥����ځ��ʨǦ����������ߗ���
�������՛���ڐ�������Ղ��ͮ����������ٙ�����������ԉ˓�����ϯ�����������������
���＾����������ğ��탮�����˵�ۉ�������ɀ��є����۷���������λ��ژ���������Ψ
�ב��ڿ���Ę������������������������כ�Ƃٵ���˖��ۇ���ū����ҭʹ��Ϥ��������
ޯ�Ȩ�����Ճ���֚����׺�������󺣷����˕ޣ�����ǆ��ч����ד��ر���������޶�����
����Ĭαϭ���ɍ����愻��̼�������ё������Ҍ����쫰π����̨�ۨ���Ѕ���΋�ꐕ�
����ܪ�뚊�������đ�����ޥ�������걘�����ö��ѝ���ŵ�٭������������Ӟ���ͷ�
Շ�ϙ�����ſ���ݛ�����ƀ����������������������ϙַ�Ք�Γ�ܞ̓�ġ��͋����ؿ�ھ
�ⲯᲙ�����唔ߨκη�⧊����������������򌨳���������񹹤�������ο�����焇���
򈊗��݊ǰ���ߗ݌�힄������Λ����ń�⿨��옚͑ˏ�����ݞ���Ŕ��ȫ��Ƿ�����
�����������������������٨�����ѕ�ݱ�������׏���������Ȍ�٪������䶹�������
��ƌ���Ҙ̫������ǂ����ɵ������������ؠ��蒪��ߙ����������ґ��䒕�����֥󂞅��
ؼ���ê������������������Ў�����񂢣��������̃�����Ƭ��ڌ�Ž�ز�Ʉȧ��󅣥Ϧ�
���������������Ԫќ����������ک�����͋�ʌ�ȇئ��Ͷ���������所��΁�޲����Ҥ�
����������������䳺�������ګ������������虐�圀�Û�������ϸ���ȣ����Ѱ唩�ѯ�
ڔ����т���ш֘����ޟ��˛��л�����͔����ܘ����Ճ����ç�����ŝѳ�孕�����
И���瞌����������܁����������񰴄����������ǈ�񯑺�猇������߶ŕ�������業���
������������������㖕������������������٢�����쌺�����឵Ȥ������������ﯕ�
�񻪦���ͷ�������澎����������ܒ��������×��ۯ������ʜ�������Ľ��ՙć������֩�
Ǌ�����֐����������빔�턗�����Ԃ���Ƶ�ӽ���������ʫ��ۂ�����������ڪ����̒�
������俽���͜�����������������������������������������ݖ܄��������ϕ���
����Ή��ű⿸�옗�Ɯ����ϨƤ�Ҏ����׮�������������������������������
��౏���̓�����ր��������������������������������΅Đ��Ў�޷秈����˼���°����
����������ƴڀ���������ǃ�������萃�����������숃����¨�����ؒ�Ӷ��ڱ�����
����
&]
[s2;%% &]
[s2;%% If SetMouseHandling() is true in X and/or Y axis, scroll and 
zoom are also included.&]
[s2;%% &]
[s2;%% 
@@image:881&1237
����ƀ��������������۔�ǧ��������������ݕ���Ś�����������č��㱱②���������
����������ȼ��ђ��Ḇ�������������纫����������ӏ��������������������������
��͌�Ģ���������������☰����µ����������򶦹�㉪��׭悘���즬������щ����
����ӕ坅��������������•���ᣚ��̂�����ʔ����Ŕȁ�����������ё������Ɣ��
�����������������Ј�������������������ⷁ�����𔛛�����������ӻЕ������������
����Ͼ�͚����؍�ⅆ��������꪿��ƺ�����вƦ�����������������Ϸ�֟������������
��������˖�ٹ󧀦�������ػ���Հ���������೶��ʛ�������ߩ����������������˻���
�����������׆�ѕ������������؍���𓫌��������݈�������ڝ������ז��Ժ����Ŕ
ܓ�����������������乵������������Ů��������������������������ܘ��
��ҫ�ᘂ���ޮ颴�����熭��ض����ʠ��Ý��ǉ����É��������Ůٻ��������쀧�����
�ؑ�߀�����Ԃ��˭�Φۆ����Ħ��������ܽ����������̱�搵�������������´����
��������È���̃������������ԙ��ѳ����阯ׄ���Њ̞�������۾�������ʀ��̔���؅��
�����������˗�������♆Ӡ������˲ۢ��ߚ����Ͽ���Μ��ʴ������������չ��������ڝ
�ۏ�ܻ������������Ͻ���������Ӣ����ʕ�ߢ����ӨϪټ��ϼ���Ѣ�������ĥ���Ԫ��
�����ш������������ᢘ��թ�΋���������􎜎���ᡖ�����������ڊ����蕰����
�嘠��У��暔Ġ�ӱ�ɹ����쳀�����ț��ڪ֗���Ä��������̆�͏�Ӳ��𗶦���ϓ
���������������͉�ЩǤ��煄�Ȭ�𶢝�����΄ʿ����̦�����󓼟�ܾ���淇�������
���ښԘ���������������Ⱥ������Ί��������ꀛ��ˑ���Ͻʤ�ť��̐���ʕ��������˽��
�؁��������꥗Է���ŷ���������đ��¸܈����������¨��ؚ���ѽ����������������̾
���؁���錥�Յ�Ǔ���ب�������᳋����ύߌ��Ӌ���������������ؘ싄�
����گ��ر��Л����و��Ⰰ����ɰވ���ק�Ł�����������������Т��ܴ������
�Ą���ķ��������������ɰ������̀��Ϟ���ć������ߚ������֯�����އ����܊������ͅ�
Ǔ����ר�����Ť�����������������������Î�������ڍ�֒س�ܨވ������ͬ��򡔴���
Ș��榖�������̌���ɧ�Ɲ�̼���՛�И�貜�ٓ�Ϗ���Ȳ���������������Н�ϔ����͆�㼱
��˜���ć�ʤʍ̄��݈�҉�������ȹԡǑ�������Ȅ���ı������Ї�������ᔿ���ޟ�����
��Ζ���ϗ�њ��ı��պ���������잷�����Ó�����������ȡ��Ц�����������ہ���Ҽ
��ܙ���橘������橫ᵀ�֤���ާ���ŋ����������������������ڧ��ƌ���������
���������̌�����ɟ�ԇ�����м���鸧�����������胖���櫶������������������
�����Ĉ��Ϟҩ����ɔ�����Đ�˄�����ɞ෤���ϧ粕�����ބ���󖎯��������مΉ��
���������ЯҊ�������拻��������Ʋ���蟉�ᵼϬ·�����ޘ�ɑ��۟���������Ť۩��
�Ҕ��道�������������쥎�������̴�������ٷ�ڽ�Ǻ���������Ą����ķ�����ڈͪ����
���򥷩�י�͖��޺����������������ɿ�Չ�����ȹ����ۅ����˽��������ۄ������奉��
���ڽɮ�­�����Ġ��������ً�ᐟ��첯ꀨ�܈������쫼û���ʓ����۱�����лÈ��
̥����Ĉ��䐡������Ĉ���ԅ�¸��詍������ԕ�Ą��������ı�����������ي���Ω�ܑ̹
���κ��������ӑ�э�������ϻ�������ҥ���շ����Ȏ���դݨ�������Ž��͹��ɗ����
�������˝���̻��ף�����������������Қ�ۜ��ݴ����������������������������
��כХ��ވ������󯧨��汥И������ݮ��ĭ��ᰥᛶĥ�ߔ������ʖ���ɥ��א����ٴ
̈́���������������Ģ��慶��֦�ͣ���������³����܇����ژ������조��ｧ�¤х
�ȸ�����ɘ�ٍ����٧�������ģ��߷����������Ņ��ˠ���ٔ������̞����ʤ�����ρ
�������͸�ݶ��ԓ�������������������������ؤ��������������Տ�����������ЦƝ
��������ı�����������ܻ��������̼ה������ɺ������������λ�����Έݑ���Ւޏ�
�؅���卝���ە������Ƚ�������䌩�Ǵ�ϰ�ܺ������ŋ���ڷ��И��ʼ������͗��Ɋ������
�ܟ������ꨊ������������������ۋ�����ɾ�Ѥ���������塅���Ĉ��������������
����͛ۮ����ۍ�������վ��ߕ�͕���ֽ��������ˮ���ڗ���ﻭ�ݾ���쓓ö㮾�ǣ�
���۝��ϭ���յ�曎ܽ�ì�������˷�ެ���赹������～��Ƚ��܀���ձ���缼���
���ª���ݡҎ���Ս����֠������񼰞�»��䔝��ǟ���ó�����ĳî����������������
��ޏ��������ȱ��ƹ������Ӳ�������Ͽֳ�������߄���𤼃��������ͣ����������
�ꂦ��������貭�՚�չ��Ëܲ�ю�̪������������������������Ӊ�����������¶�
�͙������ܲ���ŷ���������ﲟք���㑆̛���혎ȶ���³��ِ̻ٝؑ��ܻϵ���ݿ�޲����
��ͩ�����ی����ք�Ɣ��������ک����ӵ�᪷��˴��������������魀�����؉Ƥ����֞��
�����ꋯ�Ш��ޱ�ҹ���񤨼�㺘����ͨ����Ƌ������ێ����ؿ�����������������
��΄���Ł���懼��������έ����и���ݎ����������Ɗ���¯�������Ûଁ��џ��݋�����
����������Ѯ�������������Ә����������������󴭯͡Ҏ���˲˾���ٮ�ܚ֝���
��םֶ���ʹ���������������穷�����Ǎ�����������������䪗��۰������Ώ�̰�ݹ���
���Д����������Ѝ�����Ք՝����児���Ĉ⢠����¥АȠ��Ͽ赶��ު���ͺ���䮊򔍯�
���ʲ������ͦ��߆Ե��ذ����̽�ܫ���������������ɍɀ������򂪙����������������
���Ż�
&]
[s2;%% &]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:FitToData`(bool`): [@(0.0.255) void]_[* FitToData]([@(0.0.255) bool]_[*@3 Y]_
`=_[@(0.0.255) false])&]
[s2;%% Scales and scrolls the plots so that all data is included 
in X axis. If [%-*@3 Y] is true, Y axis is also fitted so that 
all plot points are visible.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:Zoom`(double`,bool`,bool`): [@(0.0.255) void]_[* Zoom]([@(0.0.255) double]_
[*@3 scale], [@(0.0.255) bool]_[*@3 hor]_`=_[@(0.0.255) true], [@(0.0.255) bool]_[*@3 ver]_`=
_[@(0.0.255) true])&]
[s2;%% Zooms graph by [%-*@3 scale] factor. The axis scaled are X if 
[%-*@3 hor] is true and Y if [%-*@3 ver ]is true.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:Scroll`(double`,double`): [@(0.0.255) void]_[* Scroll]([@(0.0.255) double]_
[*@3 factorX], [@(0.0.255) double]_[*@3 factorY])&]
[s2;%% Scrolls graph by [%-*@3 factorX] in X axis and [%-*@3 factorY 
]in Y axis.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetMouseHandling`(bool`,bool`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* SetM
ouseHandling]([@(0.0.255) bool]_[*@3 valx]_`=_[@(0.0.255) true], [@(0.0.255) bool]_[*@3 val
y]_`=_[@(0.0.255) false])&]
[s2;%% Activates plot scrolling and zoom in X axis if [%-*@3 valx] 
is true and in Y axis if [%-*@3 valy ]is true.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetRange`(double`,double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* S
etRange]([@(0.0.255) double]_[*@3 rx], [@(0.0.255) double]_[*@3 ry], 
[@(0.0.255) double]_[*@3 ry2]_`=_`-[@3 1])&]
[s2;%% Sets visible data range for X axis in [%-*@3 rx], Y axis in 
[%-*@3 ry] and secondary Y axis in [%-*@3 ry2].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetXRange`(`)const: [@(0.0.255) double]_[* GetXRange]()[@(0.0.255) const]&]
[s2;%% Gets X axis visible data range.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetYRange`(`)const: [@(0.0.255) double]_[* GetYRange]()[@(0.0.255) const]&]
[s2;%% Gets Y axis visible data range.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetY2Range`(`)const: [@(0.0.255) double]_[* GetY2Range]()[@(0.0.255) const]&]
[s2;%% Gets secondary Y axis visible data range.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetMajorUnits`(double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* Set
MajorUnits]([@(0.0.255) double]_[*@3 ux], [@(0.0.255) double]_[*@3 uy])&]
[s2;%% Sets the space between grid lines, horizontal grid in [%-*@3 ux 
]and vertical grid in [%-*@3 uy].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetMajorUnitsX`(`): [@(0.0.255) double]_[* GetMajorUnitsX]()&]
[s2;%% Gets the space between horizontal grid lines.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetMajorUnitsY`(`): [@(0.0.255) double]_[* GetMajorUnitsY]()&]
[s2;%% Gets the space between vertical grid lines.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetMinUnits`(double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* SetMi
nUnits]([@(0.0.255) double]_[*@3 ux], [@(0.0.255) double]_[*@3 uy])&]
[s2;%% Sets the distance from the leftmost vertical grid to origin 
([%-*@3 ux]) and from bottommost horizontal grid to origin ([%-*@3 uy]).&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetXYMin`(double`,double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* S
etXYMin]([@(0.0.255) double]_[*@3 xmin],[@(0.0.255) double]_[*@3 ymin],[@(0.0.255) double]_
[*@3 ymin2]_`=_[@3 0])&]
[s2;%% Sets the coordinate of the first visible point in the leftmost, 
bottommost corner of control in X axis coordinates as [%-*@3 xmin], 
Y axis coordinates as [%-*@3 ymin] and secondary Y axis coordinates 
as [%-*@3 ymin2].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetXMin`(`)const: [@(0.0.255) double]_[* GetXMin]_()_[@(0.0.255) const]&]
[s2;%% Gets X axis coordinate of the first visible point in the leftmost 
side of control.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetYMin`(`)const: [@(0.0.255) double]_[* GetYMin]_()_[@(0.0.255) const]&]
[s2;%% Gets Y axis coordinate of the first visible point in the bottommost 
side of control.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetYMin2`(`)const: [@(0.0.255) double]_[* GetYMin2]_()_[@(0.0.255) const]&]
[s2;%% Gets secondary Y axis coordinate of the first visible point 
in the bottommost side of control.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetPopText`(const String`,const String`,const String`): [@(0.0.255) voi
d]_[* SetPopText]([@(0.0.255) const]_[_^String^ String]_[*@3 x], [@(0.0.255) const]_[_^String^ S
tring]_[*@3 y], [@(0.0.255) const]_[_^String^ String]_[*@3 y2])&]
[s2;%% Sets the text that will define the values in axis [%-*@3 x], 
[%-*@3 y] and [%-*@3 y2].&]
[s2;%% It is used in the pop text that appears when selecting a point 
or a window.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:AddSeries`(Vector`<XY`>`&`,const String`&`,const bool`&`,const Color`&`,const int`&`,const int`&`): [_^Scatter^ S
catter]_`&[* AddSeries]([_^Vector^ Vector]<[_^XY^ XY]>_`&_[*@3 points],[@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 legend][@(0.0.255) `=]`"`", [@(0.0.255) const]_[@(0.0.255) bool`&
]_[*@3 join][@(0.0.255) `=false],[@(0.0.255) const]_class`::Color[@(0.0.255) `&]_[*@3 pcolor
][@(0.0.255) `=]LtBlue,[@(0.0.255) const]_[@(0.0.255) int`&]_[*@3 width][@(0.0.255) `=][@3 30
],[@(0.0.255) const]_[@(0.0.255) int`&]_[*@3 thickness][@(0.0.255) `=][@3 6])&]
[s2;%% Adds a new data series located in [%-*@3 points], with [%-*@3 legend].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:Stroke`(int`,Color`,const String`): [_^Scatter^ Scatter]_`&[* Stroke]([@(0.0.255) i
nt]_[*@3 width]_`=_[@3 30], class`::Color_[*@3 color]_`=_Null, [@(0.0.255) const]_[_^String^ S
tring]_[*@3 pattern]_`=_LINE`_SOLID)&]
[s2;%% Sets the line [%-*@3 width], [%-*@3 color] and line [%-*@3 pattern] 
of the latest added series.&]
[s2;%% If [%-*@3 color] is Null, it is selected automatically a different 
color depending of the series id.&]
[s2;%% Available [%-*@3 pattern] values are:&]
[s2;i150;O0;%% LINE`_DOTTED -|-|[C `"o...`"]&]
[s2;i150;O0;%% LINE`_DOTTED`_SEP-|[C `"o...........`"]&]
[s2;i150;O0;%% LINE`_DASHED -|-|[C `"oooooo......`"]&]
[s2;i150;O0;%% LINE`_DASH`_DOT -|-|[C `"ooooo...o...`"]&]
[s2;i150;O0;%% LINE`_SOLID -|-|[C `"oooooooooooo`"]&]
[s2;%% However this value is user configurable just entering a String 
with the pattern defining line elements as `"o`" and space elements 
as `".`".&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:Mark`(int`,Color`,MarkStyle`): [_^Scatter^ Scatter]_`&[* Mark]([@(0.0.255) i
nt]_[*@3 thickness]_`=_[@3 6], class`::Color_[*@3 color]_`=_Null, MarkStyle_[*@3 style]_`=
_CIRCLE)&]
[s2;%% Sets the mark [%-*@3 thickness], [%-*@3 color] and [%-*@3 style] 
of the latest added series.&]
[s2;%% If [%-*@3 color] is Null, it is selected automatically a different 
color depending of the series id.&]
[s2;%% Available mark [%-*@3 style] values are CIRCLE, RECTANGLE, SQUARE, 
TRIANGLE, CROSS, X, RHOMB.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:HideMark`(`): [_^Scatter^ Scatter]_`&[* HideMark]()&]
[s2;%% Hides the of the latest added series.marks.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetSequentialX`(const int`&`,const bool`&`): [@(0.0.255) void]_[* SetSequ
entialX]([@(0.0.255) const]_[@(0.0.255) int`&]_[*@3 j], [@(0.0.255) const]_[@(0.0.255) bool
`&]_[*@3 sequential]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 sequential] is true, it Indicates that the [%-*@3 j] 
graph data has been sequentially inserted following X axis.&]
[s2;%% This speeds up the graph painting.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetSequentialX`(const bool`&`): [_^Scatter^ Scatter]_`&[* SetSequentialX](
[@(0.0.255) const]_[@(0.0.255) bool`&]_[*@3 sequential]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 sequential] is true, it Indicates that the last inserted 
graph data has been sequentially inserted following X axis.&]
[s2;%% This speeds up the graph painting.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetSequentialXAll`(const bool`&`): [_^Scatter^ Scatter]_`&[* SetSequentia
lXAll]([@(0.0.255) const]_[@(0.0.255) bool`&]_[*@3 sequential]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 sequential] is true, it Indicates that all graph data 
has been sequentially inserted following X axis.&]
[s2;%% This speeds up the graph painting.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:RemoveSeries`(const int`&`): [@(0.0.255) void]_[* RemoveSeries]([@(0.0.255) c
onst]_[@(0.0.255) int`&]_[*@3 j])&]
[s2;%% Remove [%-*@3 j] data series.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:RemoveAllSeries`(`): [@(0.0.255) void]_[* RemoveAllSeries]()&]
[s2;%% Remove all data series, cleaning the graph.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetMouseBehavior`(Scatter`:`:MouseBehaviour`*`): [@(0.0.255) bool]_[* Set
MouseBehavior]([_^Scatter`:`:MouseBehaviour^ MouseBehaviour]_`*[*@3 mouseBehavior])&]
[s2;%% Sets with [%-*@3 mouseBehavior] the array of MouseBehavior conditions 
and actions to be considered when handling the mouse over the 
control.&]
[s2;%% This array has to be ended with an item with action `=`= NO`_ACTION.&]
[s2;%% Returns false if [%-*@3 mouseBehavior] array is not well defined.&]
[s3; &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 Scatter`::MouseBehavior]]}}&]
[s3;%% &]
[s1;:Scatter`:`:MouseBehaviour`:`:struct: [@(0.0.255)3 struct][3 _][*3 MouseBehavior]&]
[s9;%% This structure is used to describe the behavior of the mouse 
when used in Scatter. It includes the keyboard and mouse conditions 
that, when complied, will launch the indicated action.&]
[s9;%% It is used by SetMouseBehavior(MouseBehaviour `*`_mouseBehavior) 
function to set an array of MouseBehavior items that will be 
used to launch actions as zooming or scrolling when mouse is 
used.&]
[s9;%% The default array is in Scatter`::MouseBehaviour defaultMouse.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s4;%% &]
[s5;:Scatter`:`:MouseBehaviour`:`:ctrl: [@(0.0.255) bool]_[* ctrl]&]
[s2;%% Set to true if Ctrl has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:alt: [@(0.0.255) bool]_[* alt]&]
[s2;%% Set to true if Alt has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:shift: [@(0.0.255) bool]_[* shift]&]
[s2;%% Set to true if Shift has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:left: [@(0.0.255) bool]_[* left]&]
[s2;%% Set to true if mouse left button has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:middle: [@(0.0.255) bool]_[* middle]&]
[s2;%% Set to true if mouse middle button has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:middleWheel: [@(0.0.255) int]_[* middleWheel]&]
[s2;%% Set to true if mouse middle wheel has to be rolled.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:right: [@(0.0.255) bool]_[* right]&]
[s2;%% Set to true if mouse right button has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:action: MouseAction_[* action]&]
[s2;%% Is the action to be launched if the previous conditions are 
complied. It can be:&]
[s2;i150;O0;~~~1248;%% NO`_ACTION-|No action. It serves to mark the 
end of MouseBehavior array.&]
[s2;i150;O0;~~~1248;%% SCROLL-|Scrolls the graphs.&]
[s2;i150;O0;~~~1248;%% ZOOM`_H`_ENL-|Zooms horizontally enlarging 
the graphs. &]
[s2;i150;O0;~~~1248;%% ZOOM`_H`_RED-|Zooms horizontally reducing the 
graphs.&]
[s2;i150;O0;~~~1248;%% ZOOM`_V`_ENL-|Zooms vertically enlarging the 
graphs.&]
[s2;i150;O0;~~~1248;%% ZOOM`_V`_RED-|Zooms horizontally reducing the 
graphs.&]
[s2;i150;O0;~~~1248;%% SHOW`_INFO-|Shows an info label including mouse 
real X and Y coordinates.&]
[s3;%% &]
[s0; ]