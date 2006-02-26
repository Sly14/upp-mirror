TITLE("Logical positions")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param][a83;*R6 "
"$$2,5#31310162474203024125188417583966:caption][b83;* $$3,5#07864147445237544204411237157677:title][b167;a42;C2 "
"$$4,6#40027414424643823182269349404212:item][b42;a42;2 $$5,5#45413000475342174754091244180557:text][l288;a17;2 "
"$$6,6#27521748481378242620020725143825:desc][")
TOPIC_TEXT(
"l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#65142375456100023862071332075487:separator][*@(0.0.255)2 "
"$$9,0#83433469410354161042741608181528:base][t4167;C $$10,0#37138531426314131251341829483380:class][l288;a17;*1 "
"$$11,11#70004532496200323422659154056402:requirement][i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam][b167;C2 "
"$$13,13#924304594434")
TOPIC_TEXT(
"60461911108080531343:item1][i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.128) "
"$$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7;2 $$16,16#03451589433145915344929335295360:result][l321;b83;a83;*C$7;2 "
"$$17,17#07531550463529505371228428965313:result`-line][l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title][{_}%EN-US "
"[s2; Logical positions")
TOPIC_TEXT(
"&][s5; Placement of Ctrls within its parent is determined by logical positions. "
"Logical position alow more complex placement relative to parent Ctrl rectangle. There "
"are four types of horizontal logical positions:&][s5;i150;O0; [* left] `- in this "
"case, logical position determines distance from left edge of parent rectangle and "
"horizontal size.&][s5;i150;O0; [* size] `- logical position determines")
TOPIC_TEXT(
" distance from both left and right edges of parent Rect&][s5;i150;O0; [* right ]`- "
"logical position determines horizontal size and distance from right edge of Rect.&][s5;i150;O0; "
"[* center] `- logical position determines distance from center horizontal size.&][s5; "
"Vertical logical positions are equivalent.&][s5; Each time parent Ctrl changes its "
"layout (usually as result of creation or resizing), ")
TOPIC_TEXT(
"pixel positions of all Ctrls are recomputed based on logical positions.&][s5; Another "
"issue that is related to positions is layout zooming. The problem here is that application "
"can use font of different size or type than standard one used in layout designer "
"when designing layout. That is why layout logical position need to be zoomed. Zoom "
"factor is determined by ratio of text [@3 `\"OK_Cancel_Exit_")
TOPIC_TEXT(
"Retry`\"] in runtime standard font size to Size(99, 13) `- size of this text using "
"standard font.&][s5; Zooming layout positions is done using placement methods with "
"Z suffix (like LeftPosZ etc...).&][s0;3 ]")
