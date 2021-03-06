topic "TheIDE macros";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[i288;b167;a42;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[{_}%EN-US 
[s2; TheIDE macros&]
[s0; TheIDE macros are written in [^topic`:`/`/ide`/app`/esc`$en`-us^ Esc 
scripting language] and placed in .usc files. .usc files can 
be either part of regular packages or are placed in Common or 
Local directory.&]
[s0; &]
[s0; Macro definition in .usc file starts with header in form&]
[s0; &]
[s7; [* macro] [*/@4 Macro`_menu][* :][*/@4 Macro`_item] [*/@4 Key]&]
[s0; &]
[s0; where [*/@4 Macro`_menu] and [*/@4 Macro`_item] are text literals 
(with C syntax) and [*/@4 Key] is key description similar to those 
shown in menus. [*/@4 Macro`_menu] with [@(0.0.255) :] and [*/@4 Key] 
can be omitted. Macro header is followed by [^topic`:`/`/Esc`/srcdoc`/Esc`$en`-us^ E
sc] body.&]
[s0; &]
[s0; Examples:&]
[s0; &]
[s7; macro `"Navigation`":`"Find next `\`"TopWindow`\`"`" Ctrl`+T 
`{&]
[s7; -|.Find(`"TopWindow`");&]
[s7; `}&]
[s7; &]
[s7; macro `"Insert current date`" `{&]
[s7; -|s `= `[`];&]
[s7; -|tm `= GetSysTime();&]
[s7; -|s << to`_string(tm.year) << `"`-`" << to`_string(tm.month) 
<< `"`-`" << to`_string(tm.day);&]
[s7; -|.Insert(s);&]
[s7; `}&]
[s0; &]
[s0; Formally, from Esc perspective, macro is a `"method`" of editor 
`- all editor commands are written in `"method`" notation (starting 
with dot).&]
[s0; &]
[s0; Additionally, helper functions can be defined using the [*/@5 fn] 
keyword:&]
[s7; fn Duplicate(str) `{&]
[s7; -|return str `+ str;&]
[s7; `}&]
[s0; &]
[s0; Functions defined like this can be simply called in the code 
of macros as a global functions.&]
[s0; &]
[s0; &]
[s2; [4 Text editing methods]&]
[s0; Most of text editing methods are provided in two (`"overloaded`") 
forms `- implicit that works with cursor and selection or explicit 
with given positions.&]
[s0; &]
[ {{3667:6333h1;@(204) [s0; Method]
:: [s0; Description]
::^@2 [s0; [*C+75 GetLength()]]
:: [s0; Length of text.]
:: [s0; [*C+75 GetLineCount()]]
:: [s0; Number of lines in text.]
:: [s0; [*C+75 GetLinePos(][/C+75 line][*C+75 )]]
:: [s0; Position (offset from the beginning of the file) of the first 
character of [/ line].]
:: [s0; [*C+75 GetCursor()]]
:: [s0; Cursor offset.]
:: [s0; [*C+75 GetLine(][/C+75 position][*C+75 )]]
:: [s0; Line for given [/ position].]
:: [s0; [*C+75 GetColumn(][/C+75 position][*C+75 )]]
:: [s0; Position in line for given [/ position].]
:: [s0; [*C+75 GetSelBegin()]]
:: [s0; Start of selection.]
:: [s0; [*C+75 GetSelCount()]]
:: [s0; Number of characters in selection. If zero, GetSelBegin() is 
equal to GetCursor().]
:: [s0; [*C+75 SetCursor(][/C+75 position][*C+75 )]]
:: [s0; Sets cursor to given [/ position].]
:: [s0; [*C+75 SetSelection(][/C+75 position][*C+75 , ][/C+75 count][*C+75 )]]
:: [s0; Sets selection starting with [/ position] with [/ count] characters.]
:: [s0; [*C+75 ClearSelection()]]
:: [s0; Cancels selection.]
:: [s0; [*C+75 Get(][/C+75 position][*C+75 , ][/C+75 count][*C+75 )]]
:: [s0; Returns array of [/ count] characters, starting at [/ position.]]
:: [s0; [*C+75 Get(][/C+75 position][*C+75 )]]
:: [s0; Same as Get(position, 1).]
:: [s0; [*C+75 Remove(][/C+75 position][*C+75 , ][/C+75 count][*C+75 )]]
:: [s0; Removes [/ count] characters at [/ position.]]
:: [s0; [*C+75 Remove(][/C+75 count][*C+75 )]]
:: [s0; Same as Remove(GetCursor(), [/ count]) `- removes [/ count] characters 
at cursor position.]
:: [s0; [*C+75 Remove()]]
:: [s0; Same as Remove(GetSelBegin(), GetSelCount()) `- removes selection.]
:: [s0; [*C+75 Insert(][/C+75 position][*C+75 , ][/C+75 text][*C+75 )]]
:: [s0; Inserts array of characters [/ text] at the [/ position.]]
:: [s0; [*C+75 Insert(][/C+75 text][*C+75 )]]
:: [s0; Same as Insert(GetCursor(), [/ text]) `- inserts [/ text] at cursor 
position.]
:: [s0; [*C+75 Find(][/C+75 text][*C+75 , ][/C+75 down][*C+75 , ][/C+75 whole`_word][*C+75 , 
][/C+75 ignore`_case][*C+75 , ][/C+75 wildcards][*C+75 )]]
:: [s0; Finds [/ text], using give options. Options can be omitted `- 
in that case [/ down] is considered true and rest of options false. 
If text is found, function returns 1, otherwise 0.]
:: [s0; [*C+75 Replace(][/C+75 text][*C+75 , ][/C+75 replace`_with][*C+75 , ][/C+75 whole`_word][*C+75 ,
 ][/C+75 ignore`_case][*C+75 , ][/C+75 widcards][*C+75 )]]
:: [s0; Block replace, using given options. Options ca be omitted `- 
in that case they are considered false. Returns number of strings 
replaced.]
:: [s0; [*C+75 MoveLeft(...)]&]
[s0; [*C+75 MoveRight(...)]&]
[s0; [*C+75 MoveWordLeft(...)]&]
[s0; [*C+75 MoveWordRight(...)]&]
[s0; [*C+75 MoveUp(...)]&]
[s0; [*C+75 MoveDown(...)]&]
[s0; [*C+75 MoveHome(...)]&]
[s0; [*C+75 MoveEnd(...)]&]
[s0; [*C+75 MovePageUp(...)]&]
[s0; [*C+75 MovePageDown(...)]&]
[s0; [*C+75 MoveTextBegin(...)]&]
[s0; [*C+75 MoveTextEnd(...)]&]
[s0; [*C+75 MoveLeft(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MoveRight(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MoveWordLeft(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MoveWordRight(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MoveUp(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MoveDown(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MoveHome(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MoveEnd(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MovePageUp(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MovePageDown(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MoveTextBegin(][/C+75 sel][*C+75 )]&]
[s0; [*C+75 MoveTextEnd(sel)]]
:: [s0; Moves cursor in given direction. Variant with [/ sel] makes selection 
(like when corresponding movement while pressing Shift key) if 
[/ sel] is 1.]}}&]
[s0; &]
[s2; [4 File methods]&]
[s0; &]
[ {{3667:6333h1;@(204) [s0; Method]
:: [s0; Description]
::^@2 [s0; [*C+75 FileName()]]
:: [s0; Returns the name of currently edited file.]
:: [s0; [*C+75 EditFile(][/C+75 path][*C+75 )]&]
[s0; [*C+75 EditFile(][/C+75 filename, pkg][*C+75 )]]
:: [s0; Opens the specified file in editor.]
:: [s0; [*C+75 SaveCurrentFile()]]
:: [s0; Saves file in active tab.]
:: [s0; [*C+75 CloseFile()]]
:: [s0; Closes active tab.]}}&]
[s0; &]
[s2; [4 User interaction methods]&]
[s0; From time to time, it is necessary to communicate some information 
from user to the macro or vice versa, which is exactly what these 
methods do.&]
[s0; &]
[ {{3667:6333h1;@(204) [s0; Method]
:: [s0; Description]
::^@2 [s0; [*C+75 Input(][/C+75 label, ...][*C+75 )]]
:: [s0; Simple input dialog. Provides as many input text fields as is 
number of parameters specifying labels for these fields. If user 
chooses Cancel, returns void, otherwise returns single string 
if there is just one edit field or array of strings for multiple 
edit fields.]
:: [s0; [*C+75 ClearConsole()]]
:: [s0; Clear the output console.]
:: [s0; [*C+75 Echo(][/C+75 ...][*C+75 )]]
:: [s0; Prints all arguments to the output console, one per line.]}}&]
[s0; &]
[s2; [4 Build and execute methods]&]
[s0; The macros can be used to extend the capabilities of TheIDE. 
For that, there is a few methods that allow executing other programs 
and also to trigger building U`+`+ packages.&]
[s0; &]
[ {{3667:6333h1;@(204) [s0; Method]
:: [s0; Description]
::^@2 [s0; [*C+75 Execute(][/C+75 cmdline][*C+75 )]]
:: [s0; Executes cmdline, capturing the output to the console.]
:: [s0; [*C+75 Launch(][/C+75 cmdline][*C+75 )]]
:: [s0; Launches the application.]
:: [s0; [*C+75 Build(][/C+75 `[flags`[, outfile`]`]][*C+75 )]]
:: [s0; Builds open main package. Optional parameters can be used to 
specify the flags (e.g. `"GUI MT`") and target path for the compiled 
executable.]
:: [s0; [*C+75 BuildProject(][/C+75 uppfile, flags`[, outfile`]][*C+75 )]]
:: [s0; Same as previous, but works with any package specified by passing 
path to it`'s .upp file as a first parameter.]}}&]
[s0; &]
[s2; [4 Informative methods]&]
[s0; The methods providing information about packages and build settings. 
If the optional parameter [/ pkg] is not specified, the currently 
selected package is used.&]
[s0; &]
[ {{3667:6333h1;@(204) [s0; Method]
:: [s0; Description]
::^@2 [s0; [*C+75 ActivePackage()]]
:: [s0; The name of package in which the currently edited file belongs 
to.]
:: [s0; [*C+75 MainPackage()]]
:: [s0; The name of main package.]
:: [s0; [*C+75 AllPackages()]]
:: [s0; Array of all used packages.]
:: [s0; [*C+75 PackageDir(][/C+75 `[pkg`]][*C+75 )]]
:: [s0; The directory where the package resides.]
:: [s0; [*C+75 PackageFiles(][/C+75 `[pkg`]][*C+75 )]]
:: [s0; List of all files in the package.]
:: [s0; [*C+75 Assembly()]]
:: [s0; Name of the assembly.]
:: [s0; [*C+75 Flags()]]
:: [s0; Array of currently set build flags.]
:: [s0; [*C+75 BuildMethod()]]
:: [s0; Currently selected build method.]
:: [s0; [*C+75 BuildMode()]]
:: [s0; Currently selected build mode (0 `= Debug, 1 `= Optimal, 2 `= 
Speed, 3 `= Size).]}}&]
[s0; ]]