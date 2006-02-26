TITLE("Using theIDE as a Cross Development Tool")
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
"$$21,21#03324558446220344731010354752573:Par][{_}%EN-US [s0;*3 Using theIDE as a "
"Cross Development Tool&][s0;3 &][s0;3 By turning off many of the features of theIDE "
"it can be used as a cross development tool which uses pre`-defined external makefiles. "
" This is especially useful when moving applications previ")
TOPIC_TEXT(
"ously developed under other IDE`'s to theIDE.&][s0;3 &][s0;3 What follows is a description "
"of how to use theIDE with such external makefiles without having to edit such files. "
" There may be other ways of achieving such tasks and it may be possible to utilise "
"the existing comprehensive build features of theIDE.  However the aim was to minimise "
"the effort of conversion in order to get a valid compil")
TOPIC_TEXT(
"ation.&][s0;3 &][s0;3 It should be noted that the following will only work with "
"versions of theIDE from 602 onwards.  This is because it requires additional standard "
"library functions not implemented in previous versions.&][s0;3 &][s0;*3 1 Create "
"an empty package within myapps&][s0;3 &][s0;3 Start theIDE and create a new package. "
" Define the <name> of the package and select an `'empty`' package.  ")
TOPIC_TEXT(
"Immediately the package has been created load and edit the <name>.upp file in the "
"<name> directory using theIDE.  If not already present add the lines:&][s0;3  &][ "
"{{10000<544;>2176; [s0;3 mainconfig&][s0;3      `\"`\"`=`\"`\";]}}&][s0;3 &][s0;3 "
"Close and reopen the package.  This tests that the <name> package can be found and "
"that the <name>.upp file is recognised and has the correct syntax.  Without")
TOPIC_TEXT(
" the two lines above theIDE will not find the package.&][s0;3 &][s0;*3 2 Create "
"a macro file for the <name> package&][s0;3 &][s0;3 With the <name> package open under "
"theIDE create a new file with the extension usc... I suggest macro.usc.  This will "
"be the macro file where all the work will be done.  Create appropriate macros in "
"this file  in the form of the examples below.  This is using the ESC s")
TOPIC_TEXT(
"cripting language.  Note the `'Execute`' standard library function is required. "
" This is only available in versions 602 onwards of theIDE.&][s0;3 &][ {{10000 [s0; "
"macro `\"MakeA`\" Alt`+A `{&][s0; -|Execute(`\"c:/mingw/bin/mingw32`-make.exe `-C "
"d:/myapps/control `-f d:/myapps/control/makealarm all`\");&][s0; `}&][s0; macro `\"CopyLibs`\" "
"Alt`+0 `{&][s0; -|Execute (`\"cp.exe d:/myapps/edrtoslib/libed8515.")
TOPIC_TEXT(
"a d:/myapps/control/`\");&][s0; -|Execute(`\"cp d:/myapps/edrtoslib/libed8535.a "
"d:/myapps/control/`\");&][s0; `}]}}&][s0; &][s0; The first macro named `'MakeA`' "
"can be invoked by the `'Alt`+A`' keystrokes or by clicking the macro name.  When "
"invoked it calls the Execute standard function.  The parameter within this function "
"is the full path name with its parameters for a command line application exec")
TOPIC_TEXT(
"ution.  You will need to incorporate your own command lines as appropriate for your "
"cross development environment.&][s0; &][s0; The above example is for the AVR cross "
"development package WINAVR.  This utilises the mingw GCC compiler and other packages "
"along with the mingw32`-make utility.&][s0; &][s0; Note that an explicit makefile "
"has been defined and that an explicit directory has been named.  T")
TOPIC_TEXT(
"he reasons for this are twofold.  Firstly the IDE  has as its `'root`' directory "
"`'myapps`' rather than the package directory when the command is executed.  Secondly "
"the explicit directory naming results in the makefile not having to be edited.&][s0; "
"&][s0; Also for some of the above `'applications; to be found the `'autoexec.bat`' "
"file path and other environmental variable names have been set fro")
TOPIC_TEXT(
"m the previous cross development application.  This is not part of the function "
"of theIDE.&][s0; &][s0;* 3 Running the Cross`-Compile&][s0; &][s0; The `'macro`' "
"menu item will appear sometime on the menu bar.  You may have to close and reopen "
"the package for it to appear for the first time.   Click on `'macro`' and then the "
"required macro name to run it.  Alternatively use the quick key, Alt`+M in")
TOPIC_TEXT(
" this case, if it has been defined.  When selected the macro body will be executed "
"which will in this case invoke the application make facility.  Errors will be displayed "
"in the bottom window.  Any errors can be located by clicking the left mouse button "
"on the error line as with the normal theIDE operation.&][s0; &][s0;* 4. Adding Files "
"and Spacers&][s0; &][s0; Files and spacers can be added in th")
TOPIC_TEXT(
"e normal way by clicking the right mouse button in the files window at the left "
"side of the screen.  These however will have no association with the external makefiles. "
" &][s0; &][s0;* 5 Open File  or Open Edited Files Warning.&][s0; &][s0; I am unsure "
"of what will happen with open / edited files with the external makefiles.  When the "
"inbuilt `'build`' is executed any open files are updated so tha")
TOPIC_TEXT(
"t the changed will be compiled.  This does not appear to be the case when the external "
"makefile is used.  It is recommended that all edited files are written back prior "
"to invoking the macro.&][s0; &][s0;* 6 Incorporation into theIDE&][s0; &][s0; When "
"more knowledge of theIDE is known it may be possible to incorporate such external "
"make facilities such as WINAVR into its standard build features.  ")
TOPIC_TEXT(
"There are mechanisms to select other compiler builds but the extensive windows library "
"will need to be uncoupled.  This however is a future project.&][s0;3 ]")
