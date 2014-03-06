topic "ValueMap";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Title]]}}&]
[s1;@(0.0.255)3%- &]
[s1;:ValueMap`:`:class:%- [@(0.0.255)3 class][3 _][*3 ValueMap]&]
[s2; ValueMap is a map of key`-value pairs, where both key and value 
are of Value type. Key has to be rich Value type with operator`=`= 
and GetHashValue defined (actually, GetHashValue does not have 
to be defined if performance is not critical). It is fully Rich 
Value compatible.&]
[s2; &]
[s2; All key`-value pairs added to ValueMap are also assigned indices 
in order of Adding them to ValueMap and elements are also accessible 
using these indices.&]
[s2; &]
[s2; ValueMap is also compatible with ValueArray (in the similar 
way how Time is compatible with Date). Assigning Value containing 
ValueMap to ValueArray assigns it a ValueArray representing values 
of ValueMap. Assigning ValueArray to ValueMap creates map where 
keys are number 0, 1, ... GetCount() `- 1 and values contain 
elements with corresponding indices.&]
[s2; &]
[s2; Not that Value provides operator`[`] methods to directly access 
ValueArray elements contained in Value with text keys (if Value 
does not contain ValueMap or requested key, Void Value is returned).&]
[s2; &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:ValueMap`:`:ValueMap`(`):%- [* ValueMap]()&]
[s2; Default constructor.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(const ValueMap`&`):%- [* ValueMap]([@(0.0.255) const]_[* ValueMa
p][@(0.0.255) `&]_[*@3 v])&]
[s2; Copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(pick`_ Index`<Value`>`&`,pick`_ Vector`<Value`>`&`):%- [* Va
lueMap]([@(0.128.128) pick`_]_[_^Index^ Index]<[_^Value^ Value]>`&_[*@3 k], 
[@(0.128.128) pick`_]_[_^Vector^ Vector]<[_^Value^ Value]>`&_[*@3 v])&]
[s2; Creates ValueMap by picking Index a Vector of Values (efficient 
but sources are destroyed).&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(const Index`<Value`>`&`,const Vector`<Value`>`&`,int`):%- [* V
alueMap]([@(0.0.255) const]_[_^Index^ Index]<[_^Value^ Value]>`&_[*@3 k], 
[@(0.0.255) const]_[_^Vector^ Vector]<[_^Value^ Value]>`&_[*@3 v], [@(0.0.255) int]_[*@3 deep
])&]
[s2; Creates ValueMap by deep copying Index a Vector of Values.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:`~ValueMap`(`):%- [@(0.0.255) `~][* ValueMap]()&]
[s2; Destructor.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:operator`=`(const ValueMap`&`):%- [_^ValueMap^ ValueMap][@(0.0.255) `&]_
[* operator`=]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 v])&]
[s2; Assignment.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:operator Value`(`)const:%- [* operator_Value]()_[@(0.0.255) const]&]
[s2; Conversion to Value.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(const Value`&`):%- [* ValueMap]([@(0.0.255) const]_[_^Value^ Val
ue][@(0.0.255) `&]_[*@3 src])&]
[s2; Conversion from Value.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(const Nuller`&`):%- [* ValueMap]([@(0.0.255) const]_[_^Nuller^ N
uller][@(0.0.255) `&])&]
[s2; Null assignment. ValueMap is considered Null if it contains 
no elements.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s2; Same as GetCount() `=`= 0.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements from ValueMap.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns number of elements in ValueMap.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Same as GetCount() `=`= 0.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:GetKey`(int`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* G
etKey]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns key at index [%-*@3 i].&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:GetValue`(int`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_
[* GetValue]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns value at index [%-*@3 i].&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Add`(const Value`&`,const Value`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:ValueMap`:`:Add`(const String`&`,const Value`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:ValueMap`:`:Add`(const char`*`,const Value`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v
alue])&]
[s5;:ValueMap`:`:Add`(int`,const Value`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) int]_[*@3 k
ey], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s5;:ValueMap`:`:Add`(Id`,const Value`&`):%- [@(0.0.255) void]_[* Add]([_^Id^ Id]_[*@3 key], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s2;%- Adds a new key`-value pair to ValueMap. ValueMap can contain 
multiple same keys. Note: multiple overloads are required to make 
Id and SqlId work with ValueMap.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Set`(const Value`&`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:ValueMap`:`:Set`(const String`&`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:ValueMap`:`:Set`(const char`*`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 va
lue])&]
[s5;:ValueMap`:`:Set`(int`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 q
], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s5;:ValueMap`:`:Set`(Id`,const Value`&`):%- [@(0.0.255) void]_[* Set]([_^Id^ Id]_[*@3 id], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s2;%- [%% If there is ][*@3 key][%%  in ValueMap, its associated Value 
is replaced by ][*@3 value][%% . If there is none such key, key`-value 
is added. If there is more than single such key, only first one 
(the one with lowest index) is replaced. ]Note: multiple overloads 
are required to make Id and SqlId work with ValueMap.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:SetAt`(int`,const Value`&`):%- [@(0.0.255) void]_[* SetAt]([@(0.0.255) int
]_[*@3 i], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Replaces value at given index.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:SetKey`(int`,const Value`&`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:SetKey`(int`,const String`&`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:SetKey`(int`,const char`*`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) in
t]_[*@3 i], [@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 key])&]
[s5;:ValueMap`:`:SetKey`(int`,int`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) int]_[*@3 key])&]
[s5;:ValueMap`:`:SetKey`(int`,Id`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) int]_[*@3 i], 
[_^Id^ Id]_[*@3 key])&]
[s2;%- [%% Replaces key at given index. ]Note: multiple overloads are 
required to make Id and SqlId work with ValueMap.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:RemoveKey`(const Value`&`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:RemoveKey`(const String`&`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:RemoveKey`(const char`*`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) co
nst]_[@(0.0.255) char`*]_[*@3 key])&]
[s5;:ValueMap`:`:RemoveKey`(int`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) int]_[*@3 key
])&]
[s5;:ValueMap`:`:RemoveKey`(Id`):%- [@(0.0.255) int]_[* RemoveKey]([_^Id^ Id]_[*@3 key])&]
[s2; Removes all key`-value pairs with given key. Slow O(n) operation.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Remove`(int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2; Removes key`-value pair at index [%-*@3 i].&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:GetKeys`(`)const:%- [@(0.0.255) const]_[_^Index^ Index]<[_^Value^ Value]>`&
_[* GetKeys]()_[@(0.0.255) const]&]
[s2; Returns keys.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:GetValues`(`)const:%- [_^ValueArray^ ValueArray]_[* GetValues]()_[@(0.0.255) c
onst]&]
[s2; Returns values.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:operator ValueArray`(`)const:%- [* operator_ValueArray]()_[@(0.0.255) co
nst]&]
[s2; Same as GetValues.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:operator`[`]`(const Value`&`)const:%- [@(0.0.255) const]_[_^Value^ Value
][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key
])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`[`]`(const String`&`)const:%- [@(0.0.255) const]_[_^Value^ Valu
e][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s
])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`[`]`(const char`*`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`[`]`(const Id`&`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) const]_[_^Id^ Id][@(0.0.255) `&]_[*@3 key])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`[`]`(const int`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) const]_[@(0.0.255) int]_[*@3 key])_[@(0.0.255) const]&]
[s2;%- [%% Returns value for ][*@3 key][%% . If ][*@3 key][%%  is not present, 
returns ErrorValue (which is also void and Null Value). ]Note: 
multiple overloads are required to make Id and SqlId work with 
ValueMap.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:GetAndClear`(const Value`&`):%- [_^Value^ Value]_[* GetAndClear]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key])&]
[s2; If [%-*@3 key] is not present, returns void Value, otherwise returns 
value for [%-*@3 key] and sets it to void Value. The purpose of 
this processing is to optimize copying in situation when complex 
value (e.g. ValueArray) is about to be changed after retrieval, 
without clearing it in ValueMap the deep copy of complex structure 
would have to be performed when changing it.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Returns hashing value.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream
][@(0.0.255) `&]_[*@3 s])&]
[s2; Binary serialization.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Jsonize`(JsonIO`&`):%- [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 jio])&]
[s2; JSON serialization support.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Xmlize`(XmlIO`&`):%- [@(0.0.255) void]_[* Xmlize]([_^XmlIO^ XmlIO][@(0.0.255) `&
]_[*@3 xio])&]
[s2; XML serialization support.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Conversion to text.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:operator`=`=`(const ValueMap`&`)const:%- [@(0.0.255) bool]_[* operator`=
`=]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s2; Equality comparison, two ValueMap are equal when they contain 
same key`-value pairs [*/ in the same order].&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:operator`!`=`(const ValueMap`&`)const:%- [@(0.0.255) bool]_[* operator!`=
]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s2; Same as !operator`=`=(v).&]
[s3; &]
[s0; ]]