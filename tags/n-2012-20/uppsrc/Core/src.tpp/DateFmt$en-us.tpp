topic "Date formatting and scanning";
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
[s0;%- [*@3;4 Date formatting and scanning]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Global functions]]}}&]
[s3;%- &]
[s5;:StrToDate`(Date`&`,const char`*`):%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* StrToDa
te]([_^Date^ Date][@(0.0.255) `&]_[*@3 d], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; Scans a string for a Date. Order of day, month and year is specified 
using SetDateScan. In place of month both number and text is 
accepted `- text must match abbreviated or full name of month.&]
[s7; [%-*C@3 d]-|Found date.&]
[s7; [%-*C@3 s]-|String to scan.&]
[s7; [*/ Return value]-|NULL if no date is found in s or pointer to 
character right after the date.&]
[s3; &]
[s4;%- &]
[s5;:Format`(Date`):%- [_^String^ String]_[* Format]([_^Date^ Date]_[*@3 date])&]
[s2; Formats date. Date is formated using the standard [^topic`:`/`/Core`/src`/Format`$en`-us^ F
ormat] function, where the string set by SetDateFormat is used 
as formating string, and the three supplied integer value arguments 
are year, month and day.&]
[s7; [%-*C@3 date]-|Date to format.&]
[s7; [*/ Return value]-|Formatted date.&]
[s3; &]
[s4;%- &]
[s5;:CharFilterDate`(int`):%- [@(0.0.255) int]_[* CharFilterDate]([@(0.0.255) int]_[*@3 c])&]
[s2; Character filter for dates. Its behaviour is specified by SetDateFilter 
function.&]
[s7; [%-*C@3 c]-|Character to filter.&]
[s7; [*/ Return value]-|Filtered character.&]
[s3; &]
[s4;%- &]
[s5;:DayOfWeek`(Date`):%- [@(0.0.255) int]_[* DayOfWeek]([_^Date^ Date]_[*@3 date])&]
[s2; Day [%-*@3 date] number of day of week, beginning from 0.&]
[s3; &]
[s4;%- &]
[s5;:LastDayOfMonth`(Date`):%- [_^Date^ Date]_[* LastDayOfMonth]([_^Date^ Date]_[*@3 d])&]
[s2; Returns day [%-*@3 d] last day of the month.&]
[s0;l288; Example:&]
[s0;l288; [*C@3       ][*C LastDayOfMonth(Date(2011, 9, 23)) `= `"30/9/2011`"]&]
[s3; &]
[s4;%- &]
[s5;:FirstDayOfMonth`(Date`):%- [_^Date^ Date]_[* FirstDayOfMonth]([_^Date^ Date]_[*@3 d])&]
[s2; Returns day [%-*@3 d] first day of the month.&]
[s0;l288; Example:&]
[s0;l288; [*C@3       ][*C LastDayOfYear(Date(2011, 9, 23)) `= `"1/9/2011`"]&]
[s3; &]
[s4;%- &]
[s5;:LastDayOfYear`(Date`):%- [_^Date^ Date]_[* LastDayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2; Returns day [%-*@3 d] last day of the year.&]
[s0;l288; Example:&]
[s0;l288; [*C@3       ][*C LastDayOfYear(Date(2011, 9, 23)) `= `"31/12/2011`"]&]
[s3; &]
[s4;%- &]
[s5;:FirstDayOfYear`(Date`):%- [_^Date^ Date]_[* FirstDayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2; Returns day [%-*@3 d ]first day of the year.&]
[s0;l288; Example:&]
[s0;l288; [*C@3       ][*C FirstDayOfYear(Date(2011, 9, 23)) `= `"1/1/2011`"]&]
[s3; &]
[s4;%- &]
[s5;:DayOfYear`(Date`):%- [@(0.0.255) int]_[* DayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2; Returns day [%-*@3 d ]number of day of year, beginning from 1.&]
[s0;l288; Example:&]
[s0;l288; [*C@3       ][*C DayOfYear(Date(2011, 1, 2)) `= 2]&]
[s3; &]
[s4;%- &]
[s5;:SetDateFormat`(const char`*`):%- [@(0.0.255) void]_[* SetDateFormat]([@(0.0.255) const
]_[@(0.0.255) char]_`*[*@3 fmt])&]
[s2; Sets formating string for dates. When used for formatting, there 
are 3 integer arguments passed to [^topic`:`/`/Core`/src`/Format`$en`-us^ Format] 
with this string. First is year, second is month and third is 
day.&]
[s3; &]
[s4;%- &]
[s5;:SetDateScan`(const char`*`):%- [@(0.0.255) void]_[* SetDateScan]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 scan])&]
[s2; Sets date scan string `- this string represents order of [* d]ay, 
[* m]onth and [* y]ear for StrToDate function. Letters `'[* d]`', `'[* m]`' 
and `'[* y]`' are used in [%-*@3 scan] to designate the order.&]
[s2; Example:&]
[s2; [*C@3       ][*C `"mdy`"]&]
[s7; The month is first, day second and year third.&]
[s3; &]
[s4;%- &]
[s5;:SetDateFilter`(const char`*`):%- [@(0.0.255) void]_[* SetDateFilter]([@(0.0.255) const
]_[@(0.0.255) char]_`*[*@3 seps])&]
[s2; Specifies CharFilterDate behaviour. Digits are always allowed 
in CharFilterDate. If there is `"a`" at the beginning of seps 
string, letters are allowed. If there is `"A`", letters are allowed 
and converted to upper`-case. After this optional character, 
list of characters allowed as separator follows. `"`\r`" after 
character designates `"replace`" character `- if used, all characters 
up to next `"amend`" character or `'`\0`' are replaced by it.&]
[s2; Example:&]
[s2; [*C@3       ][*C `"a.][*C@3 `\r][*C@5 ,][*C@3 `\r][*C /][*C@3 `\r][*C@5 :;][*C `"]&]
[s2; CharFilterDate would allow letters but not convert them to uppercase, 
it will allow characters `'[* .]`' and `'/`' and it will convert 
`'[*@5 ,]`' to `'[* .]`' and `'[*@5 :]`' or `'[*@5 ;]`' to `'[* /]`'.&]
[s3; &]
[s0; ]